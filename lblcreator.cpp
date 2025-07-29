#include "lblcreator.h"

LblCreator::LblCreator(QObject *parent) : QObject(parent)
{
    Rels::instance()->refreshPolPix();
    QSqlQuery query;
    query.prepare("select gd.snam, gd.adr, gd.otk, gd.otk_title "
                  "from general_data gd where gd.dat = (select max(dat) from general_data)");
    if (query.exec()){
        while(query.next()){
            orgNam=query.value(0).toString();
            adres=query.value(1).toString();
            otk=query.value(2).toString();
            otkTit=query.value(3).toString();
        }
    } else {
        QMessageBox::critical(nullptr,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
    query.clear();
    query.prepare("select id, nam from zvd_doc_type");
    if (query.exec()){
        while(query.next()){
            mapDocTypes.insert(query.value(0).toInt(),query.value(1).toString());
        }
    } else {
        QMessageBox::critical(nullptr,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}

bool LblCreator::createLbl(int id_part, bool shortAmp)
{
    dataPart data=getDataPart(id_part);
    return createLbl(data.id_el,data.id_diam,data.ibco,data.datePart,shortAmp, false, data.id_var);
}

bool LblCreator::createLbl(int id_el, int id_diam, QString ibco, QDate date, bool shortAmp, bool order, int id_var)
{
    RtfWriter rtfWriter;
    RTF_DOCUMENT_FORMAT *df = rtfWriter.get_documentformat();
    RTF_SECTION_FORMAT *sf = rtfWriter.get_sectionformat();
    RTF_PARAGRAPH_FORMAT *ph = rtfWriter.get_paragraphformat();

    double scale = order ? 1.44 : 1.0;

    const int sz1=RtfWriter::fromMmSize(16*scale);
    const int sz2=RtfWriter::fromMmSize(37*scale);
    const int sz3=RtfWriter::fromMmSize(58*scale);
    const int sz4=RtfWriter::fromMmSize(79*scale);
    const int sz5=RtfWriter::fromMmSize(113*scale);
    const int sz6=RtfWriter::fromMmSize(180*scale);
    const int szImg=RtfWriter::fromMmSize(12*scale);

    const int fs1=14*scale;
    const int fs2=20*scale;
    const int fs3=24*scale;

    RTF_TABLEROW_FORMAT *tf = rtfWriter.get_tablerowformat();
    tf->marginTop=RtfWriter::fromMmSize(0.5*scale);
    tf->marginBottom=RtfWriter::fromMmSize(0.5*scale);
    tf->marginLeft=RtfWriter::fromMmSize(1*scale);
    tf->marginRight=RtfWriter::fromMmSize(2*scale);

    RTF_TABLECELL_FORMAT borderFormat = *rtfWriter.get_tablecellformat();
    borderFormat.borderBottom.border=true;
    borderFormat.borderTop.border=true;
    borderFormat.borderLeft.border=true;
    borderFormat.borderRight.border=true;

    if (order){
        df->paperHeight=RtfWriter::fromMmSize(297);//16838
        df->paperWidth=RtfWriter::fromMmSize(210);//11907
        df->landscape=true;

        sf->pageWidth=RtfWriter::fromMmSize(297);//16838
        sf->pageHeight=RtfWriter::fromMmSize(210);//11907
        sf->landscape=true;

    } else {
        df->paperHeight=RtfWriter::fromMmSize(210);//11907
        df->paperWidth=RtfWriter::fromMmSize(297);//16838
        df->landscape=false;

        sf->pageWidth=RtfWriter::fromMmSize(210);//11907
        sf->pageHeight=RtfWriter::fromMmSize(297);//16838
        sf->landscape=false;
    }


    sf->pageMarginBottom=RtfWriter::fromMmSize(15);
    sf->pageMarginTop=RtfWriter::fromMmSize(15);
    sf->pageMarginLeft=RtfWriter::fromMmSize(15);
    sf->pageMarginRight=RtfWriter::fromMmSize(15);

    rtfWriter.start_doc();

    QString tulist=getTuList(id_el,id_diam,date,id_var);
    dataLbl data=getData(id_el,id_diam,id_var);
    QString srtStr=getSrtStr(id_el,id_diam,date,id_var);

    if (!ibco.isEmpty()){
        data.znam=ibco;
    }
    ph->tableText=false;
    ph->paragraphAligment=4;
    ph->CHARACTER.fontSize=fs2;

    if (order){
        rtfWriter.start_paragraph(QString::fromUtf8("Размер 180х60"),false);
    }

    rtfWriter.start_paragraph("",true);
    ph->tableText=true;
    ph->paragraphAligment=1;
    ph->CHARACTER.fontSize=fs2;
    rtfWriter.set_tablecellformat(borderFormat);
    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(sz1,RtfWriter::BEGIN_MERGE);
    ph->paragraphAligment=3;
    ph->CHARACTER.fontSize=fs3;
    ph->CHARACTER.boldCharacter=true;
    rtfWriter.start_paragraph(data.marka,false);
    rtfWriter.end_tablecell();//1

    rtfWriter.start_tablecell(sz2,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//2

    ph->CHARACTER.fontSize=fs1;
    ph->CHARACTER.boldCharacter=false;
    rtfWriter.start_tablecell(sz3,RtfWriter::BEGIN_MERGE);
    rtfWriter.start_paragraph(tulist,false);
    rtfWriter.end_tablecell();//3

    rtfWriter.start_tablecell(sz4,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//4

    rtfWriter.start_tablecell(sz5);
    if (!data.isotype.isEmpty()&&data.isotype!=QString("-")){
        rtfWriter.start_paragraph(data.isotype,false);
    }
    if (!data.awstype.isEmpty()&&data.awstype!=QString("-")){
        rtfWriter.start_paragraph(data.awstype,(!data.isotype.isEmpty()&&data.isotype!=QString("-")));
    }
    rtfWriter.end_tablecell();//5

    rtfWriter.start_tablecell(sz6);
    ph->paragraphAligment=1;
    ph->CHARACTER.fontSize=fs2;
    ph->CHARACTER.boldCharacter=true;
    ph->CHARACTER.underlineCharacter=true;

    QString ch=getCh(data);
    rtfWriter.start_paragraph(ch,false);
    ph->CHARACTER.underlineCharacter=false;
    if (!data.znam.isEmpty()&& data.znam!=QString("-")){
        rtfWriter.start_paragraph(data.znam,true);
    }

    rtfWriter.end_tablecell();//6
    rtfWriter.end_tablerow();

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(sz1);
    QImage img(":/images/"+QString::number(data.id_pix)+".png");
    rtfWriter.insert_image(img,szImg,szImg);
    rtfWriter.end_tablecell();//1

    rtfWriter.start_tablecell(sz2,RtfWriter::BEGIN_MERGE);
    ph->paragraphAligment=3;
    ph->CHARACTER.fontSize=fs1;
    ph->CHARACTER.boldCharacter=false;
    rtfWriter.start_paragraph(data.descr,false);
    rtfWriter.end_tablecell();//2

    rtfWriter.start_tablecell(sz3,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//3

    rtfWriter.start_tablecell(sz4,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//4

    rtfWriter.start_tablecell(sz5,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//5

    rtfWriter.start_tablecell(sz6,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//6
    rtfWriter.end_tablerow();

    ph->paragraphAligment=1;

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(sz1,RtfWriter::NO_MERGE,RtfWriter::BEGIN_MERGE);
    rtfWriter.start_paragraph(QString::fromUtf8("Диаметр мм"),false);
    rtfWriter.end_tablecell();//1

    rtfWriter.start_tablecell(sz2,RtfWriter::BEGIN_MERGE);
    rtfWriter.start_paragraph(QString::fromUtf8("Рекомендуемое значение тока (А)"),false);
    rtfWriter.end_tablecell();//2

    rtfWriter.start_tablecell(sz3,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//3

    rtfWriter.start_tablecell(sz4,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//4

    ph->paragraphAligment=4;

    rtfWriter.start_tablecell(sz5,RtfWriter::NO_MERGE,RtfWriter::BEGIN_MERGE);
    rtfWriter.start_paragraph(srtStr,false);
    rtfWriter.end_tablecell();//5

    ph->paragraphAligment=1;

    rtfWriter.start_tablecell(sz6,RtfWriter::NO_MERGE,RtfWriter::BEGIN_MERGE);
    rtfWriter.end_tablecell();//6
    rtfWriter.end_tablerow();

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(sz1,RtfWriter::NO_MERGE,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//1

    rtfWriter.start_tablecell(sz2,RtfWriter::BEGIN_MERGE);
    rtfWriter.start_paragraph(QString::fromUtf8("Положение шва"),false);
    rtfWriter.end_tablecell();//2

    rtfWriter.start_tablecell(sz3,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//3

    rtfWriter.start_tablecell(sz4,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//4

    rtfWriter.start_tablecell(sz5,RtfWriter::NO_MERGE,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//5

    rtfWriter.start_tablecell(sz6,RtfWriter::NO_MERGE,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//6
    rtfWriter.end_tablerow();

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(sz1,RtfWriter::NO_MERGE,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//1

    rtfWriter.start_tablecell(sz2);
    rtfWriter.start_paragraph(QString::fromUtf8("нижнее"),false);
    rtfWriter.end_tablecell();//2

    rtfWriter.start_tablecell(sz3);
    rtfWriter.start_paragraph(QString::fromUtf8("вертикальное"),false);
    rtfWriter.end_tablecell();//3

    rtfWriter.start_tablecell(sz4);
    rtfWriter.start_paragraph(QString::fromUtf8("потолочное"),false);
    rtfWriter.end_tablecell();//4

    rtfWriter.start_tablecell(sz5,RtfWriter::NO_MERGE,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//5

    rtfWriter.start_tablecell(sz6,RtfWriter::NO_MERGE,RtfWriter::MERGE);
    rtfWriter.end_tablecell();//6
    rtfWriter.end_tablerow();

    QVector<dataAmp> amps=getAmp(id_el,id_diam,shortAmp,false,id_var);
    foreach (dataAmp amp, amps) {
        rtfWriter.start_tablerow();
        rtfWriter.start_tablecell(sz1);
        rtfWriter.start_paragraph(amp.diam,false);
        rtfWriter.end_tablecell();//1

        rtfWriter.start_tablecell(sz2);
        rtfWriter.start_paragraph(amp.bottom,false);
        rtfWriter.end_tablecell();//2

        rtfWriter.start_tablecell(sz3);
        rtfWriter.start_paragraph(amp.vert,false);
        rtfWriter.end_tablecell();//3

        rtfWriter.start_tablecell(sz4);
        rtfWriter.start_paragraph(amp.top,false);
        rtfWriter.end_tablecell();//4

        rtfWriter.start_tablecell(sz5,RtfWriter::NO_MERGE,RtfWriter::MERGE);
        rtfWriter.end_tablecell();//5

        rtfWriter.start_tablecell(sz6,RtfWriter::NO_MERGE,RtfWriter::MERGE);
        rtfWriter.end_tablecell();//6
        rtfWriter.end_tablerow();
    }

    ph->paragraphAligment=3;

    QString proc=getProc(data);

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(sz6);
    rtfWriter.start_paragraph(proc,false);
    rtfWriter.end_tablecell();//1

    rtfWriter.end_tablerow();

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(sz6);
    rtfWriter.start_paragraph(getIzg(),false);
    rtfWriter.end_tablecell();//1

    rtfWriter.end_tablerow();

    if (order){
        ph->tableText=false;
        ph->paragraphAligment=4;
        ph->CHARACTER.fontSize=fs1;
        rtfWriter.start_paragraph("",true);
        rtfWriter.start_paragraph(QString::fromUtf8("СОГЛАСОВАНО:"),false);
        rtfWriter.start_paragraph("",true);
        rtfWriter.start_paragraph(otkTit+QString::fromUtf8("_________________________")+otk,true);
    }

    rtfWriter.end_doc();
    QDir dir(QDir::homePath()+"/.szsm");
    if (!dir.exists()){
        dir.mkdir(dir.path());
    }

    QString fname=QString(dir.path()+"/lbl.rtf");
    if (rtfWriter.saveDoc(fname)){
        sysCommand(fname);
    }

    return true;
}

void LblCreator::sysCommand(QString fname)
{
    QFileInfo fileInfo(fname);
    QDesktopServices::openUrl((QUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()))));
}

QString LblCreator::getTuList(int id_el, int id_diam, QDate date, int id_var)
{
    QSqlQuery tuQuery;
    QString tulist;
    tuQuery.prepare("select nam from zvd_get_tu_var( :date, :id_el, :id_diam, :id_var)");
    tuQuery.bindValue(":date",date);
    tuQuery.bindValue(":id_el",id_el);
    tuQuery.bindValue(":id_diam",id_diam);
    tuQuery.bindValue(":id_var",id_var);

    if (tuQuery.exec()){
        while (tuQuery.next()){
            if (!tulist.isEmpty()){
                tulist+="\n";
            }
            tulist+=tuQuery.value(0).toString();
        }
    } else {
        QMessageBox::critical(nullptr,tr("Error"),tuQuery.lastError().text(),QMessageBox::Ok);
    }
    return tulist;
}

QString LblCreator::getSrtStr(int id_el, int id_diam, QDate date, int id_var)
{
    QString srtStr;
    QMultiMap <int, QString> srt;

    QSqlQuery vedQuery;
    vedQuery.prepare("select z.id_doc_t, z.ved_short, z.grade_nam "
                     "from zvd_get_sert_var(:date, :id_el, :id_diam, :id_var) as z where z.en=true order by z.id_doc_t, z.ved_short");
    vedQuery.bindValue(":date",date);
    vedQuery.bindValue(":id_el",id_el);
    vedQuery.bindValue(":id_diam",id_diam);
    vedQuery.bindValue(":id_var",id_var);
    if (vedQuery.exec()){
        while (vedQuery.next()){
            int id_doc_t=vedQuery.value(0).toInt();
            QString ved=vedQuery.value(1).toString();
            QString grade=vedQuery.value(2).toString();

            QString s=ved;
            if (!grade.isEmpty()){
                s+=QString::fromUtf8(" категория ")+grade;
            }

            if (srt.contains(id_doc_t, ved) && (ved!=s)){
                srt.remove(id_doc_t,ved);
            }

            QStringList list(srt.values(id_doc_t));

            if (list.indexOf(QRegularExpression(QString("^"+s+".*")))==-1){
                srt.insert(id_doc_t,s);
            }
        }
    } else {
        QMessageBox::critical(nullptr,tr("Error"),vedQuery.lastError().text(),QMessageBox::Ok);
    }

    QList<int> keys = srt.uniqueKeys();

    for (int i=0; i<keys.size(); ++i){
        if (!srtStr.isEmpty()){
            srtStr+="\n";
        }
        srtStr+=mapDocTypes.value(keys.at(i))+":";
        QList<QString> v = srt.values(keys.at(i));
        std::sort(v.begin(),v.end());
        for (QString st:v){
            if (!srtStr.isEmpty()){
                srtStr+="\n";
            }
            srtStr+=st;
        }
    }
    return srtStr;
}

dataLbl LblCreator::getData(int id_el, int id_diam, int id_var)
{
    dataLbl data;
    QSqlQuery query;
    query.prepare("select coalesce(e.marka_sert, e.marka), (select di.diam from diam as di where di.id = :id_diam), g.nam, pu.nam, ev.znam, i.nam, a.nam, e.vl, ev.proc, e.id_pic, ev.descr "
                  "from elrtr as e "
                  "inner join gost_types as g on e.id_gost_type=g.id "
                  "inner join purpose as pu on e.id_purpose=pu.id "
                  "inner join iso_types as i on e.id_iso_type=i.id "
                  "inner join aws_types as a on e.id_aws_type=a.id "
                  "left join el_var ev on ev.id_el = e.id and ev.id_var = :id_var "
                  "where e.id = :id");
    query.bindValue(":id_diam",id_diam);
    query.bindValue(":id",id_el);
    query.bindValue(":id_var",id_var);
    if (!query.exec()){
        QMessageBox::critical(nullptr,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }
    while (query.next()){
        data.marka=query.value(0).toString();
        data.diam=QLocale().toString(query.value(1).toDouble());
        data.gosttype=query.value(2).toString();
        data.bukv=query.value(3).toString();
        data.znam=query.value(4).toString();
        data.isotype=query.value(5).toString();
        data.awstype=query.value(6).toString();
        data.vl=query.value(7).toString();
        data.pr=query.value(8).toString();
        data.id_pix=query.value(9).toInt();
        data.descr=query.value(10).toString();
    }
    return data;
}

QString LblCreator::getCh(dataLbl &data)
{
    QString ch;
    if (!data.gosttype.isEmpty()&& data.gosttype!=QString("-")){
        ch+=data.gosttype+QString("-");
    }

    ch+=data.marka+QString("-Ø");
    if (!data.bukv.isEmpty()&&data.bukv!=QString("-")){
        ch+=QString("-")+data.bukv;
    }
    return ch;
}

QString LblCreator::getProc(dataLbl &data)
{
    QString proc=QString::fromUtf8("Допустимое содержание влаги в покрытии перед использованием - %1\%.").arg(data.vl);
    if (!data.pr.isEmpty()){
        if (!data.pr.isEmpty()){
            proc+=QString::fromUtf8(" Режим повторной прокалки ")+data.pr+".";
        }
    }
    return proc;
}

QString LblCreator::getIzg()
{
    return QString::fromUtf8("Изготовитель ")+orgNam+", "+adres;
}

QVector<dataAmp> LblCreator::getAmp(int id_el, int id_diam, bool shortAmp, bool single, int id_var)
{
    QVector<dataAmp> amps;
    QSqlQuery queryAmp;
    if (!single){
        if (!shortAmp){
            queryAmp.prepare("select d.diam, a.bot, a.vert, a.ceil from amp as a "
                             "inner join diam as d on a.id_diam=d.id "
                             "where a.id_el = :id and a.id_var = :id_var order by d.diam");
            queryAmp.bindValue(":id",id_el);
            queryAmp.bindValue(":id_var",id_var);
        } else {
            queryAmp.prepare("(select distinct d.diam, a.bot, a.vert, a.ceil "
                             "from amp as a "
                             "inner join diam as d on a.id_diam = d.id "
                             "where a.id_el = :id_el1 and a.id_var = :id_var1 and a.id_diam = :id_diam1 order by d.diam) "
                             "union "
                             "(select distinct d.diam, a.bot, a.vert, a.ceil "
                             "from amp as a "
                             "inner join diam as d on a.id_diam = d.id "
                             "where a.id_el = :id_el2 and a.id_var = :id_var2 and d.diam < (select diam from diam where id = :id_diam2 ) order by d.diam DESC LIMIT 3) "
                             "union "
                             "(select distinct d.diam, a.bot, a.vert, a.ceil "
                             "from amp as a "
                             "inner join diam as d on a.id_diam = d.id "
                             "where a.id_el = :id_el3 and a.id_var = :id_var3 and d.diam > (select diam from diam where id = :id_diam3 ) order by d.diam LIMIT 3) "
                             "order by diam LIMIT 6");
            for (int i=0; i<3; i++){
                queryAmp.bindValue(":id_el"+QString::number(i+1),id_el);
                queryAmp.bindValue(":id_diam"+QString::number(i+1),id_diam);
                queryAmp.bindValue(":id_var"+QString::number(i+1),id_var);
            }
        }
    } else {
        queryAmp.prepare("select d.diam, a.bot, a.vert, a.ceil from amp as a "
                         "inner join diam as d on a.id_diam=d.id "
                         "where a.id_el = :id_el and a.id_diam = :id_diam and a.id_var = :id_var");
        queryAmp.bindValue(":id_el",id_el);
        queryAmp.bindValue(":id_diam",id_diam);
        queryAmp.bindValue(":id_var",id_var);
    }

    if (queryAmp.exec()){
        while (queryAmp.next()){
            dataAmp data;
            data.diam=QLocale().toString(queryAmp.value(0).toDouble(),'f',1);
            data.bottom=queryAmp.value(1).toString();
            data.vert=queryAmp.value(2).toString();
            data.top=queryAmp.value(3).toString();
            amps.push_back(data);
        }
    } else {
        QMessageBox::critical(nullptr,tr("Ошибка"),queryAmp.lastError().text(),QMessageBox::Ok);
    }
    return amps;
}

dataPart LblCreator::getDataPart(int id_part)
{
    dataPart data;
    QSqlQuery queryPart;
    queryPart.prepare("select p.id_el, (select d.id from diam as d where p.diam=d.diam), p.dat_part, p.ibco, p.id_var "
                      "from parti as p where p.id= :id");
    queryPart.bindValue(":id",id_part);
    bool ok=queryPart.exec();
    if (ok){
        while (queryPart.next()){
            data.id_el=queryPart.value(0).toInt();
            data.id_diam=queryPart.value(1).toInt();
            data.datePart=queryPart.value(2).toDate();
            data.ibco=queryPart.value(3).toString();
            data.id_var=queryPart.value(4).toInt();
        }
    } else {
        QMessageBox::critical(nullptr,tr("Error"),queryPart.lastError().text(),QMessageBox::Ok);
    }
    return data;
}
