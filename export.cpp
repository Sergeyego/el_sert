#include "export.h"

Export::Export(QObject *parent) : QObject(parent)
{

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
        while (query.next()){
            kat.appendChild(getMark(query.value(0).toInt(),&doc));
        }
    } else {
        QMessageBox::critical(NULL,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }
    root.appendChild(kat);

    QDomElement sert = doc.createElement(QString::fromUtf8("Свидетельства_и_документы"));
    QSqlQuery querySert;
    querySert.prepare("select s.id from zvd_sert as s "
                      "where COALESCE(s.dat_end,'3000-01-01')>= :date "
                      "order by dat_doc");
    querySert.bindValue(":date",QDate::currentDate());
    if (querySert.exec()){
        while (querySert.next()){
            sert.appendChild(getSert(querySert.value(0).toInt(),&doc));
        }
    } else {
        QMessageBox::critical(NULL,tr("Ошибка"),querySert.lastError().text(),QMessageBox::Ok);
    }
    root.appendChild(sert);

    QString filename = QFileDialog::getSaveFileName(nullptr,QString::fromUtf8("Сохранить документ"),
                                                            QDate::currentDate().toString("yyyy-MM-dd")+".xml",
                                                            QString::fromUtf8("Documents (*.xml)") );
    QFile file(filename);
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream.setCodec(QTextCodec::codecForName("UTF-8"));
        doc.save(stream,1);
        file.close();
    }
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
                  "i.nam, a.nam, e.id_pic, p.descr, e.descr, e.descr_spec, e.descr_feature, e.pr "
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
        QString temp=pr.left(3);
        QString dop=pr.mid(3,2);
        QString ch=pr.mid(5,1);
        QString ok=pr.mid(6,2);
        dry=QString("%1±%2°C %3 час%4").arg(temp).arg(dop).arg(ch).arg(ok);
    }
    mark.appendChild(newElement(QString::fromUtf8("Прокалка_перед_сваркой"),dry,doc));

    mark.appendChild(getTu(id_el,doc));

    mark.appendChild(getAmp(id_el,doc));

    mark.appendChild(getPlav(id_el,doc));

    mark.appendChild(getChem(id_el,doc));

    mark.appendChild(getMech(id_el,doc));

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

    QDomElement type=newElement(QString::fromUtf8("Тип_документа"),typeDoc,doc);
    type.setAttribute("id",id_typeDoc);
    sert.appendChild(type);

    QDomElement vd=newElement(QString::fromUtf8("Ведомство"),ved,doc);
    vd.setAttribute("id",id_ved);
    sert.appendChild(vd);

    sert.appendChild(newElement(QString::fromUtf8("Начало_дейстия"),fromDate(dat_beg),doc));

    sert.appendChild(newElement(QString::fromUtf8("Конец_дейстия"),fromDate(dat_end),doc));

    sert.appendChild(newElement(QString::fromUtf8("Номер_документа"),nom_doc,doc));

    sert.appendChild(newElement(QString::fromUtf8("Дата_выдачи"),fromDate(dat_doc),doc));

    sert.appendChild(newElement(QString::fromUtf8("Группы_технических_устройств"),gr_tech_ust,doc));

    sert.appendChild(getSertMark(id_sert,doc));

    return sert;
}

QDomElement Export::getSertMark(int id_sert, QDomDocument *doc)
{
    QDomElement mar = doc->createElement(QString::fromUtf8("Сертифицированные_марки"));
    QSqlQuery markQuery;
    markQuery.prepare("select e.id_el, COALESCE(m.marka_sert,m.marka) from zvd_els as e "
                      "inner join elrtr as m on m.id=e.id_el "
                      "where e.id_sert= :id1 "
                      "union "
                      "select d.id_el, COALESCE(el.marka_sert,el.marka) from zvd_eldim as ed "
                      "inner join dry_els as d on d.ide=ed.id_eldr "
                      "inner join elrtr as el on el.id=d.id_el "
                      "where ed.id_sert= :id2 ");
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
    tuQuery.prepare("select nam from zvd_get_tu( :date, :id_el, :id_diam)");
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

QDomElement Export::getAmp(int id_el, QDomDocument *doc)
{
    QDomElement amp = doc->createElement(QString::fromUtf8("Рекомендуемое_значение_тока"));
    QSqlQuery queryAmp;
    queryAmp.prepare("select d.diam, a.bot, a.vert, a.ceil from amp as a "
                     "inner join diam as d on a.id_diam=d.id "
                     "where a.id_el = :id order by d.diam");
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
                      "where c.id_el = :id order by t.sig");
    queryChem.bindValue(":id",id_el);
    if (queryChem.exec()){
        while (queryChem.next()){
            QDomElement ch = doc->createElement(QString::fromUtf8("Элемент"));
            ch.appendChild(newElement(QString::fromUtf8("Название"),queryChem.value(0).toString(),doc));
            ch.appendChild(newElement(QString::fromUtf8("Обозначение"),queryChem.value(1).toString(),doc));
            ch.appendChild(newElement(QString::fromUtf8("Мининум"),fromDouble(queryChem.value(2),3),doc));
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
                      "where id_el=17");
    queryMech.bindValue(":id",id_el);
    if (queryMech.exec()){
        while (queryMech.next()){
            QDomElement ch = doc->createElement(QString::fromUtf8("Параметр"));
            ch.appendChild(newElement(QString::fromUtf8("Название_html"),queryMech.value(0).toString(),doc));
            ch.appendChild(newElement(QString::fromUtf8("Обозначение_html"),queryMech.value(1).toString(),doc));
            ch.appendChild(newElement(QString::fromUtf8("Мининум"),fromDouble(queryMech.value(2),3),doc));
            ch.appendChild(newElement(QString::fromUtf8("Максимум"),fromDouble(queryMech.value(3),3),doc));
            mech.appendChild(ch);
        }
    } else {
        //QMessageBox::critical(NULL,tr("Error"),queryMech.lastError().text(),QMessageBox::Ok);
        qDebug()<<queryMech.lastError().text();
    }
    return mech;
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