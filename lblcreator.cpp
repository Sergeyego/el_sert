#include "lblcreator.h"

LblCreator::LblCreator(QObject *parent) : QObject(parent)
{
    QSqlQuery query;
    query.prepare("select nam_lbl, adr, otk, kach from hoz where id=1");
    if (query.exec()){
        while(query.next()){
            orgNam=query.value(0).toString();
            adres=query.value(1).toString();
            otk=query.value(2).toString();
            kach=query.value(3).toString();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}

bool LblCreator::createLbl(int id_part, bool shortAmp)
{
    QSqlQuery queryPart;
    int id_el;
    int id_diam;
    QDate datePart;
    QString ibco;
    queryPart.prepare("select p.id_el, (select d.id from diam as d where p.diam=d.diam), p.dat_part, p.ibco "
                      "from parti as p where p.id= :id");
    queryPart.bindValue(":id",id_part);
    bool ok=queryPart.exec();
    if (ok){
        while (queryPart.next()){
            id_el=queryPart.value(0).toInt();
            id_diam=queryPart.value(1).toInt();
            datePart=queryPart.value(2).toDate();
            ibco=queryPart.value(3).toString();
        }
        ok=createLbl(id_el,id_diam,ibco,datePart,shortAmp, false);
    } else {
        QMessageBox::critical(NULL,tr("Error"),queryPart.lastError().text(),QMessageBox::Ok);
    }
    return ok;
}

bool LblCreator::createLbl(int id_el, int id_diam, QString ibco, QDate date, bool shortAmp, bool order)
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

    QString tulist;
    QString marka;
    QString diam;
    QString isotype;
    QString awstype;
    QString gosttype;
    QString bukv;
    QString znam;
    QString descr;
    QString vl;
    QString pr;
    int id_pix=-1;

    QSqlQuery tuQuery;
    tuQuery.prepare("select nam from zvd_get_tu( :date, :id_el, :id_diam)");
    tuQuery.bindValue(":date",date);
    tuQuery.bindValue(":id_el",id_el);
    tuQuery.bindValue(":id_diam",id_diam);

    if (tuQuery.exec()){
        while (tuQuery.next()){
            if (!tulist.isEmpty()){
                tulist+="\n";
            }
            tulist+=tuQuery.value(0).toString();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),tuQuery.lastError().text(),QMessageBox::Ok);
    }


    QString srtStr;
    QMultiMap <int, QString> srt;

    QSqlQuery vedQuery;
    vedQuery.prepare("select z.id_doc_t, z.ved_short, z.grade_nam "
                     "from zvd_get_sert(:date, :id_el, :id_diam) as z order by z.id_doc_t, z.ved_short");
    vedQuery.bindValue(":date",date);
    vedQuery.bindValue(":id_el",id_el);
    vedQuery.bindValue(":id_diam",id_diam);
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

            if (list.indexOf(QRegExp(QString("^"+s+".*")))==-1){
                srt.insert(id_doc_t,s);
            }
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),vedQuery.lastError().text(),QMessageBox::Ok);
    }

    QList<int> keys = srt.uniqueKeys();

    for (int i=0; i<keys.size(); ++i){
        if (!srtStr.isEmpty()){
            srtStr+="\n";
        }
        srtStr+=Rels::instance()->relDocType->data(QString::number(keys.at(i))).toString()+":";
        QList<QString> v = srt.values(keys.at(i));
        qSort(v.begin(),v.end());
        for (QString st:v){
            if (!srtStr.isEmpty()){
                srtStr+="\n";
            }
            srtStr+=st;
        }
    }

    QSqlQuery query;
    query.prepare("select coalesce(e.marka_sert, e.marka), (select di.diam from diam as di where di.id = :id_diam), g.nam, pu.nam, d.nam, i.nam, a.nam, e.vl, e.pr, e.id_pic, e.descr "
                  "from elrtr as e "
                  "inner join gost_types as g on e.id_gost_type=g.id "
                  "inner join purpose as pu on e.id_purpose=pu.id "
                  "inner join denominator as d on e.id_denominator=d.id "
                  "inner join iso_types as i on e.id_iso_type=i.id "
                  "inner join aws_types as a on e.id_aws_type=a.id "
                  "where e.id = :id");
    query.bindValue(":id_diam",id_diam);
    query.bindValue(":id",id_el);
    if (!query.exec()){
        QMessageBox::critical(NULL,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }
    while (query.next()){
        marka=query.value(0).toString();
        diam=QLocale().toString(query.value(1).toDouble());
        gosttype=query.value(2).toString();
        bukv=query.value(3).toString();
        znam=query.value(4).toString();
        isotype=query.value(5).toString();
        awstype=query.value(6).toString();
        vl=query.value(7).toString();
        pr=query.value(8).toString();
        id_pix=query.value(9).toInt();
        descr=query.value(10).toString();
    }

    if (!ibco.isEmpty()){
        znam=ibco;
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
    rtfWriter.start_paragraph(marka,false);
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
    if (!isotype.isEmpty()&&isotype!=QString("-")){
        rtfWriter.start_paragraph(isotype,false);
    }
    if (!awstype.isEmpty()&&awstype!=QString("-")){
        rtfWriter.start_paragraph(awstype,(!isotype.isEmpty()&&isotype!=QString("-")));
    }
    rtfWriter.end_tablecell();//5

    rtfWriter.start_tablecell(sz6);
    ph->paragraphAligment=1;
    ph->CHARACTER.fontSize=fs2;
    ph->CHARACTER.boldCharacter=true;
    ph->CHARACTER.underlineCharacter=true;
    QString ch;
    if (!gosttype.isEmpty()&& gosttype!=QString("-")){
        ch+=gosttype+QString("-");
    }

    ch+=marka+QString("-Ø")/*+diam*/;
    if (!bukv.isEmpty()&&bukv!=QString("-")){
        ch+=QString("-")+bukv;
    }
    rtfWriter.start_paragraph(ch,false);
    ph->CHARACTER.underlineCharacter=false;
    if (!znam.isEmpty()&& znam!=QString("-")){
        rtfWriter.start_paragraph(znam,true);
    }

    rtfWriter.end_tablecell();//6
    rtfWriter.end_tablerow();

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(sz1);
    QImage img;
    img.loadFromData(Rels::instance()->relPosPix->data(QString::number(id_pix)).toByteArray());
    rtfWriter.insert_image(img,szImg,szImg);
    rtfWriter.end_tablecell();//1

    rtfWriter.start_tablecell(sz2,RtfWriter::BEGIN_MERGE);
    ph->paragraphAligment=3;
    ph->CHARACTER.fontSize=fs1;
    ph->CHARACTER.boldCharacter=false;
    rtfWriter.start_paragraph(descr,false);
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

    QSqlQuery queryAmp;
    if (!shortAmp){
        queryAmp.prepare("select d.diam, a.bot, a.vert, a.ceil from amp as a "
                         "inner join diam as d on a.id_diam=d.id "
                         "where a.id_el = :id order by d.diam");
        queryAmp.bindValue(":id",id_el);
    } else {
        queryAmp.prepare("(select distinct d.diam, a.bot, a.vert, a.ceil "
                         "from amp as a "
                         "inner join diam as d on a.id_diam = d.id "
                         "where a.id_el = :id_el1 and a.id_diam = :id_diam1 order by d.diam) "
                         "union "
                         "(select distinct d.diam, a.bot, a.vert, a.ceil "
                         "from amp as a "
                         "inner join diam as d on a.id_diam = d.id "
                         "where a.id_el = :id_el2 and d.diam < (select diam from diam where id = :id_diam2 ) order by d.diam DESC LIMIT 2) "
                         "union "
                         "(select distinct d.diam, a.bot, a.vert, a.ceil "
                         "from amp as a "
                         "inner join diam as d on a.id_diam = d.id "
                         "where a.id_el = :id_el3 and d.diam > (select diam from diam where id = :id_diam3 ) order by d.diam LIMIT 2) "
                         "order by diam LIMIT 4");
        for (int i=0; i<3; i++){
            queryAmp.bindValue(":id_el"+QString::number(i+1),id_el);
            queryAmp.bindValue(":id_diam"+QString::number(i+1),id_diam);
        }
    }

    if (queryAmp.exec()){
        while (queryAmp.next()){
            rtfWriter.start_tablerow();
            rtfWriter.start_tablecell(sz1);
            rtfWriter.start_paragraph(QLocale().toString(queryAmp.value(0).toDouble(),'f',1),false);
            rtfWriter.end_tablecell();//1

            rtfWriter.start_tablecell(sz2);
            rtfWriter.start_paragraph(queryAmp.value(1).toString(),false);
            rtfWriter.end_tablecell();//2

            rtfWriter.start_tablecell(sz3);
            rtfWriter.start_paragraph(queryAmp.value(2).toString(),false);
            rtfWriter.end_tablecell();//3

            rtfWriter.start_tablecell(sz4);
            rtfWriter.start_paragraph(queryAmp.value(3).toString(),false);
            rtfWriter.end_tablecell();//4

            rtfWriter.start_tablecell(sz5,RtfWriter::NO_MERGE,RtfWriter::MERGE);
            rtfWriter.end_tablecell();//5

            rtfWriter.start_tablecell(sz6,RtfWriter::NO_MERGE,RtfWriter::MERGE);
            rtfWriter.end_tablecell();//6
            rtfWriter.end_tablerow();
        }
    } else {
        QMessageBox::critical(NULL,tr("Ошибка"),queryAmp.lastError().text(),QMessageBox::Ok);
    }

    ph->paragraphAligment=3;

    QString proc;
    if (!pr.isEmpty()){
        QString temp=pr.left(3);
        QString dop=pr.mid(3,2);
        QString ch=pr.mid(5,1);
        QString ok=pr.mid(6,2);
        proc=QString::fromUtf8(" Режим повторной прокалки ")+temp+QString::fromUtf8("±")+dop+QString::fromUtf8("°C ")+ch+QString::fromUtf8(" час")+ok+".";
    }

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(sz6);
    rtfWriter.start_paragraph(QString::fromUtf8("Допустимое содержание влаги в покрытии перед использованием - %1\%.").arg(vl)+proc,false);
    rtfWriter.end_tablecell();//1

    rtfWriter.end_tablerow();

    rtfWriter.start_tablerow();
    rtfWriter.start_tablecell(sz6);
    rtfWriter.start_paragraph(QString::fromUtf8("Изготовитель ")+orgNam+", "+adres,false);
    rtfWriter.end_tablecell();//1

    rtfWriter.end_tablerow();

    if (order){
        ph->tableText=false;
        ph->paragraphAligment=4;
        ph->CHARACTER.fontSize=fs1;
        rtfWriter.start_paragraph("",true);
        rtfWriter.start_paragraph(QString::fromUtf8("СОГЛАСОВАНО:"),false);
        rtfWriter.start_paragraph("",true);
        rtfWriter.start_paragraph(QString::fromUtf8("Начальник ОТК_________________________")+otk,true);
        rtfWriter.start_paragraph("",true);
        rtfWriter.start_paragraph(QString::fromUtf8("Директор по качеству__________________")+kach,true);
    }

    rtfWriter.end_doc();
    QString fname=QString("lbl.rtf");
    if (rtfWriter.saveDoc(fname)){
        sysCommand(fname);
    }

    return true;
}

void LblCreator::sysCommand(QString fname)
{
    QString totalName=fname;
#if defined(Q_OS_WIN)
    totalName=totalName.replace(QChar('/'),QString("\\"));
    QString cmd=QString("start office.bat ")+QString("\"")+totalName+QString("\"");
#else
    QString cmd=QString("./office.sh ")+QString("'")+totalName+QString("' &");
#endif
    system(cmd.toLocal8Bit().data());
    return;
}
