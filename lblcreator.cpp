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
    dataPart data=getDataPart(id_part);
    return createLbl(data.id_el,data.id_diam,data.ibco,data.datePart,shortAmp, false);
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

    QString tulist=getTuList(id_el,id_diam,date);
    dataLbl data=getData(id_el,id_diam);
    QString srtStr=getSrtStr(id_el,id_diam,date);

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
    QImage img;
    img.loadFromData(Rels::instance()->relPosPix->data(QString::number(data.id_pix)).toByteArray());
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

    QVector<dataAmp> amps=getAmp(id_el,id_diam,shortAmp);
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

bool LblCreator::createLblGlabels(int id_el, int id_diam, QString ibco, QDate date)
{
    GlabelsLbl lbl;
    bool ok=false;
    if (lbl.createLbl(QString("SZSM-03"),true)){
        dataLbl data=getData(id_el,id_diam);
        if (!ibco.isEmpty()){
            data.znam=ibco;
        }

        lbl.newRect(1.5,1.5,140,60);
        lbl.newLine(1.5,11.5,140,0);
        lbl.newLine(36.5,11.5,0,-10);
        lbl.newLine(82.5,11.5,0,-10);
        lbl.newLine(1.5,58,140,0);
        lbl.newLine(1.5,54.5,140,0);
        lbl.newLine(1.5,30,140,0);
        lbl.newLine(82.5,54.5,0,-24.5);
        lbl.newLine(1.5,51,81,0);
        lbl.newLine(1.5,47.5,81,0);
        lbl.newLine(1.5,44,81,0);
        lbl.newLine(1.5,40.5,81,0);
        lbl.newLine(20,54.5,0,-43);
        lbl.newLine(20,37,62.5,0);
        lbl.newLine(20,33.5,62.5,0);
        lbl.newLine(40.5,37,0,17.5);
        lbl.newLine(61,37,0,17.5);

        QString ch=getCh(data);
        lbl.newText(2.5,2.5,33,8,data.marka,11,true);
        lbl.newText(82.5,2.5,59,4,ch,9,true,(Qt::AlignCenter | Qt::AlignVCenter ));
        if (!data.znam.isEmpty()&& data.znam!=QString("-")){
            if (ch.size()<19){
                lbl.newLine(98.5,6.5,27,0);
            } else {
                lbl.newLine(88.5,6.5,47,0);
            }
            lbl.newText(83.5,6.5,57,4,data.znam,9,true,(Qt::AlignCenter | Qt::AlignVCenter ));
        }

        QString tuList=getTuList(id_el,id_diam,date);
        int ftusize = (tuList.split("\n").size()>3) ? 6 : 7;
        lbl.newText(37.5,2.5,44.5,8.5,tuList,ftusize,false);
        int fdsize= (data.descr.size()<500) ? 7 : 6;
        lbl.newText(20.5,12,120.5,17.5,data.descr,fdsize,false);
        lbl.newImage(3,13,15,15,QDir::currentPath()+"/images/"+QString::number(data.id_pix)+".png");
        lbl.newText(85,32,55,21,getSrtStr(id_el,id_diam,date),7,false);

        lbl.newText(2.5,31,16,9,QString::fromUtf8("Диаметр\nмм"),7,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        lbl.newText(20,30,62.5,3.5,QString::fromUtf8("Рекомендуемое значение тока (А)"),7,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        lbl.newText(20,33.5,62.5,3.5,QString::fromUtf8("Положение шва"),7,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        lbl.newText(20,37,20.5,3.5,QString::fromUtf8("нижнее"),7,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        lbl.newText(40.5,37,20.5,3.5,QString::fromUtf8("вертикальное"),7,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        lbl.newText(61,37,21.5,3.5,QString::fromUtf8("потолочное"),7,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        QVector<dataAmp> amps=getAmp(id_el,id_diam,true);
        for (int i=0; i<amps.size(); i++){
            if (i>3){
                break;
            }
            lbl.newText(2.5,40.5+(3.5*i),16,3.5,amps.at(i).diam,7,false,(Qt::AlignCenter | Qt::AlignVCenter ));
            lbl.newText(20,40.5+(3.5*i),20.5,3.5,amps.at(i).bottom,7,false,(Qt::AlignCenter | Qt::AlignVCenter ));
            lbl.newText(40.5,40.5+(3.5*i),20.5,3.5,amps.at(i).vert,7,false,(Qt::AlignCenter | Qt::AlignVCenter ));
            lbl.newText(61,40.5+(3.5*i),21.5,3.5,amps.at(i).top,7,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        }

        lbl.newText(2.5,54.5,138,3.5,getProc(data),6,false);
        lbl.newText(2.5,58,138,3.5,getIzg(),5,false);
        QString fname="small.glabels";
        ok=lbl.saveFile(fname);
        if (ok){
            sysCommand(fname);
        }
    }
    return ok;
}

bool LblCreator::createLblGlabels(int id_part)
{
    dataPart data=getDataPart(id_part);
    return createLblGlabels(data.id_el,data.id_diam,data.ibco,data.datePart);
}

bool LblCreator::createLblGlabels2(int id_el, int id_diam, QString ibco, QDate date)
{
    GlabelsLbl lbl;
    bool ok=false;
    if (lbl.createLbl(QString("SZSM-04"),true)){
        dataLbl data=getData(id_el,id_diam);
        if (!ibco.isEmpty()){
            data.znam=ibco;
        }
        lbl.newRect(1,1,133,44);

        lbl.newLine(1,12,133,0);
        lbl.newLine(33,12,0,-11);
        lbl.newLine(67,12,0,-11);
        lbl.newLine(1,42,133,0);
        lbl.newLine(1,39,133,0);
        lbl.newLine(11,12,0,27);
        lbl.newLine(80,12,0,27);
        lbl.newLine(47,27,0,12);
        lbl.newLine(1,27,79,0);
        lbl.newLine(1,36,46,0);
        lbl.newLine(11,30,36,0);
        lbl.newLine(11,33,36,0);
        lbl.newLine(23,39,0,-6);
        lbl.newLine(35,39,0,-6);

        QString ch=getCh(data);
        lbl.newText(2,2,30,9,data.marka,11,true);
        lbl.newText(68,2,65,4,ch,9,true,(Qt::AlignCenter | Qt::AlignVCenter ));
        if (!data.znam.isEmpty()&& data.znam!=QString("-")){
            if (ch.size()<19){
                lbl.newLine(87,6.5,27,0);
            } else {
                lbl.newLine(77,6.5,47,0);
            }
            lbl.newText(68,7,65,4,data.znam,9,true,(Qt::AlignCenter | Qt::AlignVCenter ));
        }

        QString tuList=getTuList(id_el,id_diam,date);
        lbl.newText(34,2,32,9,tuList,6,false);
        int fdsize= (data.descr.size()<400) ? 7 : 6;
        lbl.newText(11,12,69,15,data.descr,fdsize,false,(Qt::AlignLeft| Qt::AlignVCenter),0.7);

        lbl.newImage(2,13,8,8,QDir::currentPath()+"/images/"+QString::number(data.id_pix)+".png");
        lbl.newText(47,27,33,12,getSrtStr(id_el,id_diam,date),5,false,(Qt::AlignLeft| Qt::AlignVCenter),0.7);

        lbl.newText(2,28,8,7,QString::fromUtf8("Диаметр\nмм"),6,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        lbl.newText(11,27,36,3,QString::fromUtf8("Рекомендуемое значение тока (А)"),6,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        lbl.newText(11,30,36,3,QString::fromUtf8("Положение шва"),6,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        lbl.newText(11,33,12,3,QString::fromUtf8("нижнее"),5,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        lbl.newText(23,33,12,3,QString::fromUtf8("вертикальное"),5,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        lbl.newText(35,33,12,3,QString::fromUtf8("потолочное"),5,false,(Qt::AlignCenter | Qt::AlignVCenter ));

        QVector<dataAmp> amps=getAmp(id_el,id_diam,true,true);
        for (int i=0; i<amps.size(); i++){
            if (i>0){
                break;
            }
            lbl.newText(1,36+(3*i),10,3,amps.at(i).diam,6,false,(Qt::AlignCenter | Qt::AlignVCenter ));
            lbl.newText(11,36+(3*i),12,3,amps.at(i).bottom,6,false,(Qt::AlignCenter | Qt::AlignVCenter ));
            lbl.newText(23,36+(3*i),12,3,amps.at(i).vert,6,false,(Qt::AlignCenter | Qt::AlignVCenter ));
            lbl.newText(35,36+(3*i),12,3,amps.at(i).top,6,false,(Qt::AlignCenter | Qt::AlignVCenter ));
        }

        lbl.newText(1,39,133,3,getProc(data),6,false);
        lbl.newText(1,42,133,3,getIzg(),5,false);

        QString fname="small2.glabels";
        ok=lbl.saveFile(fname);
        if (ok){
            sysCommand(fname);
        }
    }
    return ok;
}

void LblCreator::sysCommand(QString fname)
{
    QFileInfo fileInfo(fname);
    QDesktopServices::openUrl((QUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()))));
}

QString LblCreator::getTuList(int id_el, int id_diam, QDate date)
{
    QSqlQuery tuQuery;
    QString tulist;
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
    return tulist;
}

QString LblCreator::getSrtStr(int id_el, int id_diam, QDate date)
{
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
    return srtStr;
}

dataLbl LblCreator::getData(int id_el, int id_diam)
{
    dataLbl data;
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
        QString temp=data.pr.left(3);
        QString dop=data.pr.mid(3,2);
        QString ch=data.pr.mid(5,1);
        QString ok=data.pr.mid(6,2);
        proc+=QString::fromUtf8(" Режим повторной прокалки ")+temp+QString::fromUtf8("±")+dop+QString::fromUtf8("°C ")+ch+QString::fromUtf8(" час")+ok+".";
    }
    return proc;
}

QString LblCreator::getIzg()
{
    return QString::fromUtf8("Изготовитель ")+orgNam+", "+adres;
}

QVector<dataAmp> LblCreator::getAmp(int id_el, int id_diam, bool shortAmp, bool single)
{
    QVector<dataAmp> amps;
    QSqlQuery queryAmp;
    if (!single){
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
    } else {
        queryAmp.prepare("select d.diam, a.bot, a.vert, a.ceil from amp as a "
                         "inner join diam as d on a.id_diam=d.id "
                         "where a.id_el = :id_el and a.id_diam = :id_diam");
        queryAmp.bindValue(":id_el",id_el);
        queryAmp.bindValue(":id_diam",id_diam);
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
        QMessageBox::critical(NULL,tr("Ошибка"),queryAmp.lastError().text(),QMessageBox::Ok);
    }
    return amps;
}

dataPart LblCreator::getDataPart(int id_part)
{
    dataPart data;
    QSqlQuery queryPart;
    queryPart.prepare("select p.id_el, (select d.id from diam as d where p.diam=d.diam), p.dat_part, p.ibco "
                      "from parti as p where p.id= :id");
    queryPart.bindValue(":id",id_part);
    bool ok=queryPart.exec();
    if (ok){
        while (queryPart.next()){
            data.id_el=queryPart.value(0).toInt();
            data.id_diam=queryPart.value(1).toInt();
            data.datePart=queryPart.value(2).toDate();
            data.ibco=queryPart.value(3).toString();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),queryPart.lastError().text(),QMessageBox::Ok);
    }
    return data;
}
