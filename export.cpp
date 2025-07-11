#include "export.h"

Export::Export(QObject *parent) : QObject(parent)
{    
    updateFtpInfo();
    ftpClient = new QFtp(this);
    connect(ftpClient,SIGNAL(commandStarted(int)),this,SLOT(ftpCommandStart(int)));
    connect(ftpClient, SIGNAL(commandFinished(int,bool)),this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftpClient, SIGNAL(listInfo(QUrlInfo)),this, SLOT(addToList(QUrlInfo)));
    progress = new QProgressDialog();
    progress->setWindowTitle(QString::fromUtf8("Формирование выгрузки"));
    progress->setAutoClose(false);
    progress->setCancelButton(NULL);
    progress->setMinimumDuration(0);
    progress->setMinimum(0);
}

Export::~Export()
{
    delete progress;
}

void Export::createXml()
{        
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
    QDomElement root = doc.createElement(QString::fromUtf8("root"));
    doc.appendChild(root);

    root.appendChild(newElement(QString::fromUtf8("Дата_выгрузки"),fromDate(QDate::currentDate()),&doc));

    QDomElement kat = doc.createElement(QString::fromUtf8("Каталог_электродов"));
    QSqlQuery query;
    query.prepare("select e.id from elrtr as e where e.katalog=true order by e.marka");
    if (query.exec()){
        int n=1;
        progress->setLabelText(QString::fromUtf8("Формирование каталога электродов"));
        progress->setMaximum(query.size());
        while (query.next()){
            kat.appendChild(getMark(query.value(0).toInt(),&doc));
            QApplication::processEvents();
            progress->setValue(n);
            n++;
        }
    } else {
        QMessageBox::critical(NULL,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }
    root.appendChild(kat);

    QDomElement katWire = doc.createElement(QString::fromUtf8("Каталог_проволоки"));
    QSqlQuery queryWire;
    queryWire.prepare("select p.id from provol as p where p.katalog=true order by p.nam");
    if (queryWire.exec()){
        int n=1;
        progress->setLabelText(QString::fromUtf8("Формирование каталога проволоки"));
        progress->setMaximum(queryWire.size());
        while (queryWire.next()){
            katWire.appendChild(getWire(queryWire.value(0).toInt(),&doc));
            QApplication::processEvents();
            progress->setValue(n);
            n++;
        }
    } else {
        QMessageBox::critical(NULL,tr("Ошибка"),queryWire.lastError().text(),QMessageBox::Ok);
    }
    root.appendChild(katWire);

    QDomElement sert = doc.createElement(QString::fromUtf8("Свидетельства_и_документы"));
    QSqlQuery querySert;
    querySert.prepare("select s.id from zvd_sert as s "
                      "where COALESCE(s.dat_end,'3000-01-01')>= :date "
                      "order by dat_doc");
    querySert.bindValue(":date",QDate::currentDate());
    if (querySert.exec()){
        int n=1;
        progress->setLabelText(QString::fromUtf8("Формирование свидетельств"));
        progress->setMaximum(querySert.size());
        while (querySert.next()){
            sert.appendChild(getSert(querySert.value(0).toInt(),&doc));
            QApplication::processEvents();
            progress->setValue(n);
            n++;
        }
    } else {
        QMessageBox::critical(NULL,tr("Ошибка"),querySert.lastError().text(),QMessageBox::Ok);
    }
    root.appendChild(sert);

    QDomElement spool = doc.createElement(QString::fromUtf8("Носители_проволоки"));
    QSqlQuery querySpool;
    querySpool.prepare("select distinct c.id_diam, d.diam from wire_cena as c "
                       "inner join diam as d on c.id_diam=d.id "
                       "inner join wire_pack_kind as k on c.id_pack=k.id "
                       "where c.dat=(select max(dat) from wire_cena) "
                       "order by d.diam");
    if (querySpool.exec()){
        int n=1;
        progress->setLabelText(QString::fromUtf8("Формирование носителей проволоки"));
        progress->setMaximum(querySpool.size());
        while (querySpool.next()){
            QDomElement diam=doc.createElement(QString::fromUtf8("Носители_для_диаметра"));
            diam.appendChild(newElement(QString::fromUtf8("Диаметр"),fromDouble(querySpool.value(1).toDouble()),&doc));
            diam.appendChild(getSpool(querySpool.value(0).toInt(),&doc));
            spool.appendChild(diam);
            QApplication::processEvents();
            progress->setValue(n);
            n++;
        }
    } else {
        QMessageBox::critical(NULL,tr("Ошибка"),querySpool.lastError().text(),QMessageBox::Ok);
    }

    root.appendChild(spool);

    progress->close();    

    int l= QMessageBox::question(NULL,tr("Вопрос"),tr("Файл выгрузки будет загружен на FTP сервер. Сохранить файл локально?"),QMessageBox::Yes,QMessageBox::No);
    if (l==QMessageBox::Yes){
        QString filename = QFileDialog::getSaveFileName(nullptr,QString::fromUtf8("Сохранить документ"),
                                                        QDate::currentDate().toString("yyyy-MM-dd")+".xml",
                                                        QString::fromUtf8("Documents (*.xml)") );
        QFile file(filename);
        if ( file.open( QIODevice::WriteOnly ) ) {
            QTextStream stream( &file );
            doc.save(stream,1);
            file.close();
        }
    }

    QTextStream dataStream(&xmldata);
    doc.save(dataStream,1);
    ftpClient->put(xmldata,QString("sync.xml"));
    //emit finished();
}

void Export::start()
{
    updateList();
}

QDomElement Export::getMark(int id_el, QDomDocument *doc)
{
    QString marka;
    QString isotype;
    QString awstype;
    QString gosttype;
    QString bukv;
    QString znam;
    QString pr;
    int id_pix=-1;
    int id_vid=-1;
    QString vid;
    QString pix;
    QString descr;
    QString descr_spec;
    QString descr_feature;

    QSqlQuery query;
    query.prepare("select coalesce(e.marka_sert, e.marka), e.id_vid, et.snam, g.nam, pu.nam, d.nam, "
                  "i.nam, a.nam, e.id_pic, p.descr, e.descr, e.descr_spec, e.descr_feature, e.pr2 "
                  "from elrtr as e "
                  "inner join el_types as et on e.id_vid=et.id "
                  "inner join gost_types as g on e.id_gost_type=g.id "
                  "inner join purpose as pu on e.id_purpose=pu.id "
                  "inner join denominator as d on e.id_denominator=d.id "
                  "inner join iso_types as i on e.id_iso_type=i.id "
                  "inner join aws_types as a on e.id_aws_type=a.id "
                  "inner join pics as p on e.id_pic=p.id "
                  "where e.id = :id");
    query.bindValue(":id",id_el);
    if (!query.exec()){
        //QMessageBox::critical(NULL,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
        qDebug()<<query.lastError().text();
    }
    while (query.next()){
        marka=query.value(0).toString();
        id_vid=query.value(1).toInt();
        vid=query.value(2).toString();
        gosttype=query.value(3).toString();
        bukv=query.value(4).toString();
        znam=query.value(5).toString();
        isotype=query.value(6).toString();
        awstype=query.value(7).toString();
        id_pix=query.value(8).toInt();
        pix=query.value(9).toString();
        descr=query.value(10).toString();
        descr_spec=query.value(11).toString();
        descr_feature=query.value(12).toString();
        pr=query.value(13).toString();
    }

    QDomElement mark = doc->createElement(QString::fromUtf8("Марка_электрода"));
    mark.setAttribute("id",id_el);

    QDomElement nazn=newElement(QString::fromUtf8("Назначение"),vid,doc);
    nazn.setAttribute("id",id_vid);
    mark.appendChild(nazn);

    mark.appendChild(newElement(QString::fromUtf8("Название"),marka,doc));
    mark.appendChild(newElement(QString::fromUtf8("Тип_по_гост"),testStr(gosttype),doc));
    mark.appendChild(newElement(QString::fromUtf8("Суффикс"),testStr(bukv),doc));
    mark.appendChild(newElement(QString::fromUtf8("Знаменатель"),testStr(znam),doc));
    mark.appendChild(newElement(QString::fromUtf8("Тип_по_iso"),testStr(isotype),doc));
    mark.appendChild(newElement(QString::fromUtf8("Тип_по_aws"),testStr(awstype),doc));

    QDomElement pol=newElement(QString::fromUtf8("Положение_при_сварке"),pix,doc);
    pol.setAttribute("id",id_pix);
    mark.appendChild(pol);

    mark.appendChild(newElement(QString::fromUtf8("Описание"),descr,doc));
    mark.appendChild(newElement(QString::fromUtf8("Особые_свойства"),descr_spec,doc));
    mark.appendChild(newElement(QString::fromUtf8("Технологические_особенности_сварки"),descr_feature,doc));

    QString dry;
    if (!pr.isEmpty()){
        QStringList list=pr.split(":");
        if (list.size()==4){
            dry=QString("%1±%2°C %3 %4").arg(list.at(0)).arg(list.at(1)).arg(list.at(2)).arg(list.at(3));
        }
    }
    mark.appendChild(newElement(QString::fromUtf8("Прокалка_перед_сваркой"),dry,doc));

    mark.appendChild(getTu(id_el,doc));

    mark.appendChild(getAmp(id_el,doc));

    mark.appendChild(getPlav(id_el,doc));

    mark.appendChild(getChem(id_el,doc));

    mark.appendChild(getMech(id_el,doc));

    mark.appendChild(getDiams(id_el,doc));

    return mark;
}

QDomElement Export::getWire(int id_pr, QDomDocument *doc)
{
    QDomElement mark = doc->createElement(QString::fromUtf8("Марка_проволоки"));
    mark.setAttribute("id",id_pr);

    int id_type, id_base;

    QString nam, descr, tnam, bnam;

    QSqlQuery query;
    query.prepare("select p.nam, p.description, p.id_type, t.nam, COALESCE(p.id_base, p.id), COALESCE(pp.nam, p.nam) from provol as p "
                  "inner join provol_type as t on t.id = p.id_type "
                  "left join provol as pp on pp.id=p.id_base "
                  "where p.id = :id ");
    query.bindValue(":id",id_pr);
    if (query.exec()){
        while (query.next()){
            nam=query.value(0).toString();
            descr=query.value(1).toString();
            id_type=query.value(2).toInt();
            tnam=query.value(3).toString();
            id_base=query.value(4).toInt();
            bnam=query.value(5).toString();
        }
    } else {
        qDebug()<<query.lastError().text();
    }

    mark.appendChild(newElement(QString::fromUtf8("Название"),nam,doc));

    QDomElement type=newElement(QString::fromUtf8("Тип_проволоки"),tnam,doc);
    type.setAttribute("id",id_type);

    QDomElement base=newElement(QString::fromUtf8("Базовая_марка"),bnam,doc);
    base.setAttribute("id",id_base);

    mark.appendChild(type);
    mark.appendChild(base);
    mark.appendChild(getWireTu(id_pr,doc));
    mark.appendChild(newElement(QString::fromUtf8("Описание"),descr,doc));
    mark.appendChild(getWireChem(id_base,doc));
    mark.appendChild(getWireDiams(id_pr,doc));

    return mark;
}

QDomElement Export::getSert(int id_sert, QDomDocument *doc)
{
    int id_typeDoc=-1;
    QString typeDoc;
    int id_ved=-1;
    QString ved;
    QDate dat_beg;
    QDate dat_end;
    QString nom_doc;
    QDate dat_doc;
    QString gr_tech_ust;

    QSqlQuery query;
    query.prepare("select s.id, s.id_doc, d.fnam, s.id_ved, v.fnam, "
                  "s.dat_beg, s.dat_end, s.nom_doc, s.dat_doc, s.gr_tech_ust "
                  "from zvd_sert as s "
                  "inner join zvd_doc as d on d.id=s.id_doc "
                  "inner join zvd_ved as v on v.id=s.id_ved "
                  "where s.id = :id ");
    query.bindValue(":id",id_sert);
    if (!query.exec()){
        //QMessageBox::critical(NULL,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
        qDebug()<<query.lastError().text();
    }
    while (query.next()){
        id_typeDoc=query.value(1).toInt();
        typeDoc=query.value(2).toString();
        id_ved=query.value(3).toInt();
        ved=query.value(4).toString();
        dat_beg=query.value(5).toDate();
        dat_end=query.value(6).toDate();
        nom_doc=query.value(7).toString();
        dat_doc=query.value(8).toDate();
        gr_tech_ust=query.value(9).toString();
    }
    QDomElement sert = doc->createElement(QString::fromUtf8("Документ"));
    sert.setAttribute("id",id_sert);

    QDomElement link=newElement(QString::fromUtf8("Ссылка_для_загрузки"),docMap.value(id_sert),doc);
    sert.appendChild(link);

    QString jpg;
    if (docMap.contains(id_sert)){
        QString val=docMap.value(id_sert);
        if (ftpFiles.contains(val+"-1.jpg")){
            jpg=val+"-1.jpg";
        }
    }

    QDomElement jlink=newElement(QString::fromUtf8("Ссылка_jpg"),jpg,doc);
    sert.appendChild(jlink);

    QDomElement type=newElement(QString::fromUtf8("Тип_документа"),typeDoc,doc);
    type.setAttribute("id",id_typeDoc);
    sert.appendChild(type);

    QDomElement vd=newElement(QString::fromUtf8("Ведомство"),ved,doc);
    vd.setAttribute("id",id_ved);
    sert.appendChild(vd);

    sert.appendChild(newElement(QString::fromUtf8("Начало_действия"),fromDate(dat_beg),doc));

    sert.appendChild(newElement(QString::fromUtf8("Конец_действия"),fromDate(dat_end),doc));

    sert.appendChild(newElement(QString::fromUtf8("Номер_документа"),nom_doc,doc));

    sert.appendChild(newElement(QString::fromUtf8("Дата_выдачи"),fromDate(dat_doc),doc));

    sert.appendChild(newElement(QString::fromUtf8("Группы_технических_устройств"),gr_tech_ust,doc));

    sert.appendChild(getSertMark(id_sert,doc));
    sert.appendChild(getSertWire(id_sert,doc));

    return sert;
}

QDomElement Export::getSertMark(int id_sert, QDomDocument *doc)
{
    QDomElement mar = doc->createElement(QString::fromUtf8("Сертифицированные_марки_электродов"));
    QSqlQuery markQuery;
    markQuery.prepare("select e.id_el, COALESCE(m.marka_sert,m.marka) from zvd_els as e "
                      "inner join elrtr as m on m.id=e.id_el "
                      "where e.id_sert= :id1 and m.katalog=true "
                      "union "
                      "select d.id_el, COALESCE(el.marka_sert,el.marka) from zvd_eldim as ed "
                      "inner join dry_els as d on d.ide=ed.id_eldr "
                      "inner join elrtr as el on el.id=d.id_el "
                      "where ed.id_sert= :id2 and el.katalog=true");
    markQuery.bindValue(":id1",id_sert);
    markQuery.bindValue(":id2",id_sert);
    if (markQuery.exec()){
        while (markQuery.next()){
            QDomElement mr=doc->createElement(QString::fromUtf8("Марка_электродов"));
            mr.setAttribute("id",markQuery.value(0).toInt());
            mr.appendChild(newElement(QString::fromUtf8("Название"),markQuery.value(1).toString(),doc));
            QSqlQuery query;
            query.prepare("select d.dim from zvd_eldim as ed "
                          "inner join dry_els as d on d.ide=ed.id_eldr "
                          "where ed.id_sert= :id_sert and d.id_el= :id_el ");
            query.bindValue(":id_sert",id_sert);
            query.bindValue(":id_el",markQuery.value(0).toInt());
            QDomElement diam = doc->createElement(QString::fromUtf8("Только_диаметры"));
            if (query.exec()){
                while (query.next()){
                    diam.appendChild(newElement(QString::fromUtf8("Диаметр"),fromDouble(query.value(0),1),doc));
                }
            } else {
                //QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
                qDebug()<<query.lastError().text();
            }
            mr.appendChild(diam);
            mar.appendChild(mr);
        }
    } else {
        //QMessageBox::critical(NULL,tr("Error"),markQuery.lastError().text(),QMessageBox::Ok);
        qDebug()<<markQuery.lastError().text();
    }
    return mar;
}

QDomElement Export::getSertWire(int id_sert, QDomDocument *doc)
{
    QDomElement mar = doc->createElement(QString::fromUtf8("Сертифицированные_марки_проволоки"));
    QSqlQuery markWireQuery;
    markWireQuery.prepare("select distinct z.id_provol, p.nam, p.id_base, pb.nam from zvd_wire_diam_sert as z "
                          "inner join provol as p on p.id=z.id_provol "
                          "left join provol as pb on p.id_base=pb.id "
                          "where z.id_sert= :id1 and p.katalog=true "
                          "union "
                          "select z.id_provol, p.nam, p.id_base, pb.nam from zvd_wire_sert as z "
                          "inner join provol as p on p.id=z.id_provol "
                          "left join provol as pb on p.id_base=pb.id "
                          "where z.id_sert= :id2 and p.katalog=true");
    markWireQuery.bindValue(":id1",id_sert);
    markWireQuery.bindValue(":id2",id_sert);
    if (markWireQuery.exec()){
        while (markWireQuery.next()){
            QDomElement mr=doc->createElement(QString::fromUtf8("Марка_проволоки"));
            mr.setAttribute("id",markWireQuery.value(0).toInt());
            mr.appendChild(newElement(QString::fromUtf8("Название"),markWireQuery.value(1).toString(),doc));
            QDomElement mrb=doc->createElement(QString::fromUtf8("Базовая_марка"));
            if (!markWireQuery.value(2).isNull()){
                mrb.setAttribute("id",markWireQuery.value(2).toInt());
                mrb.appendChild(newElement(QString::fromUtf8("Название"),markWireQuery.value(3).toString(),doc));
            }
            mr.appendChild(mrb);
            QSqlQuery query;
            query.prepare("select distinct z.id_diam, d.diam from zvd_wire_diam_sert as z "
                          "inner join diam as d on z.id_diam=d.id "
                          "where z.id_sert= :id_sert and z.id_provol= :id_pr "
                          "order by d.diam ");
            query.bindValue(":id_sert",id_sert);
            query.bindValue(":id_pr",markWireQuery.value(0).toInt());
            QDomElement diam = doc->createElement(QString::fromUtf8("Только_диаметры"));
            if (query.exec()){
                while (query.next()){
                    diam.appendChild(newElement(QString::fromUtf8("Диаметр"),fromDouble(query.value(1),1),doc));
                }
            } else {
                //QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
                qDebug()<<query.lastError().text();
            }
            mr.appendChild(diam);
            mar.appendChild(mr);
        }
    } else {
        //QMessageBox::critical(NULL,tr("Error"),markQuery.lastError().text(),QMessageBox::Ok);
        qDebug()<<markWireQuery.lastError().text();
    }
    return mar;
}

QDomElement Export::newElement(QString nam, QString val, QDomDocument *doc)
{
    QDomElement l = doc->createElement(nam);
    if (!val.isEmpty()){
        l.appendChild(doc->createTextNode(val));
    }
    return l;
}

QDomElement Export::getTu(int id_el, QDomDocument *doc)
{
    QDomElement tu = doc->createElement(QString::fromUtf8("Нормативная_документация"));
    QSqlQuery tuQuery;
    tuQuery.prepare("select nam from zvd_get_tu_var( :date, :id_el, :id_diam, 1)");
    tuQuery.bindValue(":date",QDate::currentDate());
    tuQuery.bindValue(":id_el",id_el);
    tuQuery.bindValue(":id_diam",4);
    if (tuQuery.exec()){
        while (tuQuery.next()){
            tu.appendChild(newElement(QString::fromUtf8("Документ"),tuQuery.value(0).toString(),doc));
        }
    } else {
        //QMessageBox::critical(NULL,tr("Error"),tuQuery.lastError().text(),QMessageBox::Ok);
        qDebug()<<tuQuery.lastError().text();
    }
    return tu;
}

QDomElement Export::getWireTu(int id_pr, QDomDocument *doc)
{
    QDomElement tu = doc->createElement(QString::fromUtf8("Нормативная_документация"));
    QSqlQuery tuQuery;
    tuQuery.prepare("select g.nam from wire_gost as w "
                    "inner join gost_new as g on w.id_gost=g.id "
                    "where w.id_provol= :id_pr order by g.nam");
    tuQuery.bindValue(":id_pr",id_pr);
    if (tuQuery.exec()){
        while (tuQuery.next()){
            tu.appendChild(newElement(QString::fromUtf8("Документ"),tuQuery.value(0).toString(),doc));
        }
    } else {
        //QMessageBox::critical(NULL,tr("Error"),tuQuery.lastError().text(),QMessageBox::Ok);
        qDebug()<<tuQuery.lastError().text();
    }
    return tu;
}

QDomElement Export::getAmp(int id_el, QDomDocument *doc)
{
    QDomElement amp = doc->createElement(QString::fromUtf8("Рекомендуемое_значение_тока"));
    QSqlQuery queryAmp;
    queryAmp.prepare("select d.diam, a.bot, a.vert, a.ceil from amp as a "
                     "inner join diam as d on a.id_diam=d.id "
                     "where a.id_el = :id and a.id_var=1 order by d.diam");
    queryAmp.bindValue(":id",id_el);
    if (queryAmp.exec()){
        while (queryAmp.next()){
            QDomElement tt = doc->createElement(QString::fromUtf8("Строка_таблицы"));
            tt.appendChild(newElement(QString::fromUtf8("Диаметр"),fromDouble(queryAmp.value(0),1),doc));
            tt.appendChild(newElement(QString::fromUtf8("Нижнее"),queryAmp.value(1).toString(),doc));
            tt.appendChild(newElement(QString::fromUtf8("Вертикальное"),queryAmp.value(2).toString(),doc));
            tt.appendChild(newElement(QString::fromUtf8("Потолочное"),queryAmp.value(3).toString(),doc));
            amp.appendChild(tt);
        }
    } else {
        //QMessageBox::critical(NULL,tr("Error"),queryAmp.lastError().text(),QMessageBox::Ok);
        qDebug()<<queryAmp.lastError().text();
    }
    return amp;
}

QDomElement Export::getPlav(int id_el, QDomDocument *doc)
{
    QDomElement plav = doc->createElement(QString::fromUtf8("Характеристики_плавления"));
    QSqlQuery queryPlav;
    queryPlav.prepare("select n.nam, e.value from el_plav as e "
                     "inner join el_plav_nams as n on e.id_plav=n.id "
                     "where e.id_el = :id order by e.id_plav");
    queryPlav.bindValue(":id",id_el);
    if (queryPlav.exec()){
        while (queryPlav.next()){
            QDomElement pl = doc->createElement(QString::fromUtf8("Характеристика"));
            pl.appendChild(newElement(QString::fromUtf8("Название"),queryPlav.value(0).toString(),doc));
            pl.appendChild(newElement(QString::fromUtf8("Значение"),fromDouble(queryPlav.value(1),1),doc));
            plav.appendChild(pl);
        }
    } else {
        //QMessageBox::critical(NULL,tr("Error"),queryPlav.lastError().text(),QMessageBox::Ok);
        qDebug()<<queryPlav.lastError().text();
    }
    return plav;
}

QDomElement Export::getChem(int id_el, QDomDocument *doc)
{
    QDomElement chem = doc->createElement(QString::fromUtf8("Химический_состав_наплавленного_металла"));
    QSqlQuery queryChem;
    queryChem.prepare("select t.nam, t.sig, c.min, c.max from chem_tu as c "
                      "inner join chem_tbl as t on t.id=c.id_chem "
                      "where c.id_el = :id and c.id_var=1 order by t.sig");
    queryChem.bindValue(":id",id_el);
    if (queryChem.exec()){
        while (queryChem.next()){
            QDomElement ch = doc->createElement(QString::fromUtf8("Элемент"));
            ch.appendChild(newElement(QString::fromUtf8("Название"),queryChem.value(0).toString(),doc));
            ch.appendChild(newElement(QString::fromUtf8("Обозначение"),queryChem.value(1).toString(),doc));
            ch.appendChild(newElement(QString::fromUtf8("Минимум"),fromDouble(queryChem.value(2),3),doc));
            ch.appendChild(newElement(QString::fromUtf8("Максимум"),fromDouble(queryChem.value(3),3),doc));
            chem.appendChild(ch);
        }
    } else {
        //QMessageBox::critical(NULL,tr("Error"),queryChem.lastError().text(),QMessageBox::Ok);
        qDebug()<<queryChem.lastError().text();
    }
    return chem;
}

QDomElement Export::getWireChem(int id_pr, QDomDocument *doc)
{
    QDomElement chem = doc->createElement(QString::fromUtf8("Химический_состав_проволоки"));
    QSqlQuery queryChem;
    queryChem.prepare("select t.nam, t.sig, c.min, c.max from wire_chem_tu as c "
                      "inner join chem_tbl as t on t.id=c.id_chem "
                      "where c.id_provol = :id order by t.sig");
    queryChem.bindValue(":id",id_pr);
    if (queryChem.exec()){
        while (queryChem.next()){
            QDomElement ch = doc->createElement(QString::fromUtf8("Элемент"));
            ch.appendChild(newElement(QString::fromUtf8("Название"),queryChem.value(0).toString(),doc));
            ch.appendChild(newElement(QString::fromUtf8("Обозначение"),queryChem.value(1).toString(),doc));
            ch.appendChild(newElement(QString::fromUtf8("Минимум"),fromDouble(queryChem.value(2),3),doc));
            ch.appendChild(newElement(QString::fromUtf8("Максимум"),fromDouble(queryChem.value(3),3),doc));
            chem.appendChild(ch);
        }
    } else {
        //QMessageBox::critical(NULL,tr("Error"),queryChem.lastError().text(),QMessageBox::Ok);
        qDebug()<<queryChem.lastError().text();
    }
    return chem;
}

QDomElement Export::getMech(int id_el, QDomDocument *doc)
{
    QDomElement mech = doc->createElement(QString::fromUtf8("Механические_свойства_металла_шва_и_наплавленного_металла"));
    QSqlQuery queryMech;
    queryMech.prepare("select t.nam_html, t.sig_html, m.min, m.max from mech_tu as m "
                      "inner join mech_tbl as t on t.id=m.id_mech "
                      "where m.id_el= :id and m.id_var=1 order by t.nam_html");
    queryMech.bindValue(":id",id_el);
    if (queryMech.exec()){
        while (queryMech.next()){
            QDomElement ch = doc->createElement(QString::fromUtf8("Параметр"));
            ch.appendChild(newElement(QString::fromUtf8("Название_html"),queryMech.value(0).toString(),doc));
            ch.appendChild(newElement(QString::fromUtf8("Обозначение_html"),queryMech.value(1).toString(),doc));
            ch.appendChild(newElement(QString::fromUtf8("Минимум"),fromDouble(queryMech.value(2),3),doc));
            ch.appendChild(newElement(QString::fromUtf8("Максимум"),fromDouble(queryMech.value(3),3),doc));
            mech.appendChild(ch);
        }
    } else {
        //QMessageBox::critical(NULL,tr("Error"),queryMech.lastError().text(),QMessageBox::Ok);
        qDebug()<<queryMech.lastError().text();
    }
    return mech;
}

QDomElement Export::getDiams(int id_el, QDomDocument *doc)
{
    QDomElement diams = doc->createElement(QString::fromUtf8("Ассортимент"));
    QSqlQuery queryDiam;
    queryDiam.prepare("select * from( "
                      "(select d.diam as diam from amp as a "
                      "inner join diam as d on a.id_diam=d.id "
                      "where a.id_el = :id1) "
                      "union "
                      "(select d.diam as diam from cena as c "
                      "inner join diam as d on c.id_diam=d.id "
                      "where c.dat=(select max(dat) from cena) and c.id_el=:id2) "
                      ") as de order by de.diam");
    queryDiam.bindValue(":id1",id_el);
    queryDiam.bindValue(":id2",id_el);
    if (queryDiam.exec()){
        while (queryDiam.next()){
            diams.appendChild(newElement(QString::fromUtf8("Диаметр"),fromDouble(queryDiam.value(0),1),doc));
        }
    } else {
        qDebug()<<queryDiam.lastError().text();
    }
    return diams;
}

QDomElement Export::getWireDiams(int id_pr, QDomDocument *doc)
{
    QDomElement diams = doc->createElement(QString::fromUtf8("Ассортимент"));
    QSqlQuery queryDiam;
    queryDiam.prepare("select * from( "
                      "select dm.diam as diam from wire_diams as wd "
                      "inner join diam as dm on wd.id_diam=dm.id "
                      "where wd.id_wire = :id1 "
                      "union "
                      "select distinct d.diam as diam from wire_cena as c "
                      "inner join diam as d on c.id_diam=d.id "
                      "where c.dat=(select max(dat) from wire_cena) and c.id_provol = :id2 "
                      ") as dg order by dg.diam");
    queryDiam.bindValue(":id1",id_pr);
    queryDiam.bindValue(":id2",id_pr);
    if (queryDiam.exec()){
        while (queryDiam.next()){
            diams.appendChild(newElement(QString::fromUtf8("Диаметр"),fromDouble(queryDiam.value(0),1),doc));
        }
    } else {
        qDebug()<<queryDiam.lastError().text();
    }
    return diams;
}

QDomElement Export::getSpool(int id_diam, QDomDocument *doc)
{
    QDomElement spool = doc->createElement(QString::fromUtf8("Носители"));
    QSqlQuery querySpool;
    querySpool.prepare("select distinct k.short from wire_cena as c "
                       "inner join wire_pack_kind as k on c.id_pack=k.id "
                       "where c.dat=(select max(dat) from wire_cena) and c.id_diam = :id and k.katalog=true "
                       "order by k.short");
    querySpool.bindValue(":id",id_diam);
    if (querySpool.exec()){
        while (querySpool.next()){
            spool.appendChild(newElement(QString::fromUtf8("Тип_носителя"),querySpool.value(0).toString(),doc));
        }
    } else {
        qDebug()<<querySpool.lastError().text();
    }
    return spool;
}

QString Export::testStr(const QString &s)
{
    return s!="-" ? s : QString();
}

QString Export::fromDouble(const QVariant &v, int d)
{
    return v.isNull() ? QString() : QLocale().toString(v.toDouble(),'f',d);
}

QString Export::fromDate(const QDate &d)
{
    return d.isNull() ? QString() : d.toString("dd.MM.yyyy");
}

void Export::ftpConnect()
{
    ftpClient->connectToHost(ftphost);
}

void Export::updateList()
{
    if (ftpClient->state()==QFtp::LoggedIn){
        ftpClient->list();
    } else {
        ftpConnect();
    }
}

void Export::ftpCommandFinished(int /*commandId*/, bool error)
{
    if (error) {
        QMessageBox::critical(NULL, tr("FTP"),ftpClient->errorString());
    } else {
        if (ftpClient->currentCommand() == QFtp::ConnectToHost) {
            //qDebug()<<"connect ok";
            ftpClient->login(ftpuserrw,ftppasswordrw);
        } else if (ftpClient->currentCommand() == QFtp::Login){
            ftpClient->cd(ftppath);
        } else if (ftpClient->currentCommand() == QFtp::List) {
            //qDebug()<<"list finished";
            createXml();
        } else if (ftpClient->currentCommand()==QFtp::Cd){
            ftpClient->list();
        } else if (ftpClient->currentCommand()==QFtp::Remove){
            ftpClient->list();
        }
    }
    if (ftpClient->currentCommand()==QFtp::Put){
        emit finished();
    }
}

void Export::ftpCommandStart(int commandId)
{
    if (ftpClient->currentCommand()==QFtp::List){
        docMap.clear();
        ftpFiles.clear();
        //qDebug()<<"Start list";
    }
}

void Export::addToList(const QUrlInfo &urlInfo)
{
    if (urlInfo.isFile()){
        QString ftpUrl="ftp://"+ftpuser+":"+ftppassword+"@"+ftphost+ftppath+"/"+urlInfo.name();
        ftpFiles.append(ftpUrl);
        QString name=urlInfo.name();
        name.truncate(name.length()-4);
        bool ok;
        int id=name.toInt(&ok);
        if (ok){
            docMap.insert(id,ftpUrl);
        }
    }
}

void Export::updateFtpInfo()
{
    QSqlQuery query;
    query.prepare("select host_ext, user_ro, pass_ro, path_sert, user_rw, pass_rw from ftp_info where id = 1");
    if (query.exec()){
        while (query.next()){
            ftphost=query.value(0).toString();
            ftpuser=query.value(1).toString();
            ftppassword=query.value(2).toString();
            ftppath=query.value(3).toString();
            ftpuserrw=query.value(4).toString();
            ftppasswordrw=query.value(5).toString();
        }
    } else {
        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),query.lastError().text());
    }
}
