#include "sertbuild.h"

SertBuild::SertBuild(QObject *parent) :
    QTextDocument(parent)
{
    data=new DataSert(this);
    prn=false;
}

QString SertBuild::getNomPart()
{
    return data->nomPart;
}

QString SertBuild::getYearSert()
{
    return QString::number(data->dateVidSert.year());
}

QString SertBuild::getYearPart()
{
    return QString::number(data->yearPart);
}

QString SertBuild::getNomSert()
{
    return data->nomSert;
}

bool SertBuild::getPrn()
{
    return prn;
}

void SertBuild::build(int id, bool is_ship)
{
    data->refresh(id, is_ship);
    current_id=id;
    current_is_ship=is_ship;

    this->clear();

    QTextBlockFormat formatRirht;
    formatRirht.setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    QTextBlockFormat formatLeft;
    formatLeft.setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    QTextBlockFormat formatCenter;
    formatCenter.setAlignment(Qt::AlignCenter|Qt::AlignVCenter);

    QTextCharFormat textAdrFormat;
    QFont serifFont("Cursive",10,QFont::Normal);
    textAdrFormat.setFont(serifFont);

    QTextCharFormat textTitleFormat;
    QFont titleFont("Droid Serif",13,QFont::Normal);
    textTitleFormat.setFont(titleFont);

    QTextCharFormat textNormalFormat;
    QFont normalFont("Droid Sans",10,QFont::Normal);
    textNormalFormat.setFont(normalFont);
    textNormalFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);

    QTextCharFormat textBoldFormat;
    QFont boldFont("Droid Sans",10,QFont::Bold);
    textBoldFormat.setFont(boldFont);
    textBoldFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);

    QTextCharFormat textTableFormat;
    QFont tableFont("Droid Sans",9,QFont::Normal);
    textTableFormat.setFont(tableFont);

    QTextCursor cursor(this);
    cursor.movePosition(QTextCursor::Start);
    //cursor.setCharFormat(textNormalFormat);
    /*cursor.setBlockFormat(formatRirht);
    cursor.insertText(tr("СЕРТИФИКАТ КАЧЕСТВА (3.1) - Химический состав\n"),textNormalFormat);
    cursor.insertText(tr("ОТЧЕТ ПО ИСПЫТАНИЯМ (2.2) - Механические свойства"),textNormalFormat);*/

    cursor.insertBlock(formatCenter,textNormalFormat);

    addResource(QTextDocument::ImageResource, QUrl("logo"), data->logo);
    QTextImageFormat f;
    f.setName("logo");
    f.setHeight(70);
    cursor.insertImage(f);

    cursor.insertBlock(formatRirht,textAdrFormat);
    cursor.insertText(data->adres,textAdrFormat);
    cursor.insertBlock(formatRirht,textAdrFormat);
    cursor.insertText(data->contact,textAdrFormat);
    cursor.insertBlock(formatLeft);

    QVector<int> id_ved;
    QStringList odobr;
    for (int i=0; i<data->sertModel->rowCount(); i++){
        int ved=data->sertModel->data(data->sertModel->index(i,0),Qt::EditRole).toInt();

        if (!id_ved.contains(ved)){
            id_ved.push_back(ved);
        }

        int id_doc_t=data->sertModel->data(data->sertModel->index(i,6),Qt::EditRole).toInt();
        if (id_doc_t==3){
            QString str("Одобрено ");
            str+=data->sertModel->data(data->sertModel->index(i,7),Qt::EditRole).toString();
            QString cat=data->sertModel->data(data->sertModel->index(i,8),Qt::EditRole).toString();
            QString sh=str+QString(".");
            if (!cat.isEmpty()){
                str+=QString(" по категории ")+cat;
            }
            str+=QString(".");
            if (odobr.contains(sh) && (str!=sh)){
                int ind=odobr.indexOf(sh);
                odobr.removeAt(ind);
            }
            if (odobr.indexOf(QRegExp("^"+str+".*"))==-1) {
                odobr.push_back(str);
            }
        }
    }

    for (int i=0; i<id_ved.size(); i++){
        QImage img;
        img.loadFromData(Rels::instance()->relVedPix->data(QString::number(id_ved.at(i))).toByteArray());
        if (!img.isNull()) {
            addResource(QTextDocument::ImageResource, QUrl("vedimage"+QString::number(id_ved.at(i))), img);
            QTextImageFormat f;
            f.setName("vedimage"+QString::number(id_ved.at(i)));
            f.setHeight(45);
            cursor.insertImage(f);
        }
    }

    cursor.insertBlock(formatCenter,textTitleFormat);
    cursor.insertText(tr("СЕРТИФИКАТ КАЧЕСТВА №")+data->nomPart+"-"+QString::number(data->yearPart),textTitleFormat);
    if (is_ship){
        cursor.insertText("/"+data->nomSert,textTitleFormat);
    }
    cursor.insertBlock(formatLeft);
    cursor.insertText(tr("Нормативная документация: "),textBoldFormat);
    for (int i=0; i<data->tuList.size(); i++){
        cursor.insertText(data->tuList.at(i),textNormalFormat);
        if (i!=data->tuList.size()-1)
            cursor.insertText(tr(", "),textNormalFormat);
    }
    cursor.insertBlock();

    QTextTableFormat tableFormat;
    tableFormat.setAlignment(Qt::AlignHCenter);
    QBrush brush = tableFormat.borderBrush();
    brush.setColor(QColor(Qt::black));
    tableFormat.setBorderBrush(brush);
    tableFormat.setCellPadding(3);
    tableFormat.setCellSpacing(1);

    QTextTableFormat tableMechFormat(tableFormat);
    QVector<QTextLength> v;
    v.push_back(QTextLength(QTextLength::PercentageLength,80));
    v.push_back(QTextLength(QTextLength::VariableLength,10));
    v.push_back(QTextLength(QTextLength::VariableLength,10));
    tableMechFormat.setColumnWidthConstraints(v);

    QTextTableFormat tableMainFormat(tableFormat);
    QVector<QTextLength> v1;
    v1.push_back(QTextLength(QTextLength::FixedLength,300));
    v1.push_back(QTextLength(QTextLength::VariableLength,10));
    v1.push_back(QTextLength(QTextLength::VariableLength,10));
    v1.push_back(QTextLength(QTextLength::VariableLength,10));
    v1.push_back(QTextLength(QTextLength::VariableLength,10));
    tableMainFormat.setColumnWidthConstraints(v1);

    QTextTable *mainTable = cursor.insertTable(1, 5, tableMainFormat);

    cursor=mainTable->cellAt(0,0).firstCursorPosition();  
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(tr("Условное обозначение электрода"),textBoldFormat);

    cursor=mainTable->cellAt(0,1).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(tr("Проволока по ГОСТ 2246-70"),textBoldFormat);

    cursor=mainTable->cellAt(0,2).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(tr("Номер партии"),textBoldFormat);

    cursor=mainTable->cellAt(0,3).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(tr("Дата \nпроизводства"),textBoldFormat);

    cursor=mainTable->cellAt(0,4).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(is_ship ? tr("Объем \nотгрузки, кг") : tr("Объем \nвыпуска, кг"),textBoldFormat);

    mainTable->insertRows(mainTable->rows(),1);
    cursor=mainTable->cellAt(1,0).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    QString ob= (data->tip1.isNull() || data->tip2.isNull() || data->znam.isNull() || data->tip1==QString("-"))?
                (data->marka+"-"+tr("∅")+QLocale().toString(data->diam)) : (data->tip1+"-"+data->marka+"-"+tr("∅")+QLocale().toString(data->diam)+"-"+data->tip2+"\n"+data->znam);
    cursor.insertText(ob,textTableFormat);
    cursor=mainTable->cellAt(1,1).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(data->prov,textTableFormat);
    cursor=mainTable->cellAt(1,2).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(data->nomPart,textTableFormat);
    cursor=mainTable->cellAt(1,3).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(data->datePart.toString("dd.MM.yy"),textTableFormat);
    cursor=mainTable->cellAt(1,4).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(QLocale().toString(data->netto,'f',1),textTableFormat);

    cursor.movePosition(QTextCursor::End);
    cursor.setBlockFormat(formatCenter);
    cursor.insertBlock();
    cursor.insertBlock();
    double value=0;
    QString head;
    int row=data->chemModel->rowCount();
    if (row) {
        QTextTable *chemTable = cursor.insertTable(3, row, tableFormat);
        cursor=chemTable->cellAt(0,0).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.insertText(tr("Химический состав наплавленного металла, %\n"),textBoldFormat);
        cursor.insertText(tr("Испытания: согласно EN 10204 - 3.1"),textBoldFormat);
        chemTable->mergeCells(0,0,1,row);
        for (int i=0; i<row; i++)
        {
            head=data->chemModel->data(data->chemModel->index(i,0)).toString();
            value=data->chemModel->data(data->chemModel->index(i,1)).toDouble();
            if (value!=0){
                cursor=chemTable->cellAt(2,i).firstCursorPosition();
                cursor.setBlockFormat(formatCenter);
                cursor.insertText(" "+QLocale().toString(value,'f',3)+" ",textTableFormat);
                cursor=chemTable->cellAt(1,i).firstCursorPosition();
                cursor.setBlockFormat(formatCenter);
                cursor.insertText(head,textTableFormat);
            }
        }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.setBlockFormat(formatCenter);
    cursor.insertBlock();
    cursor.insertBlock();

    QVector<int> meh;
    for (int j=0; j<data->mechModel->rowCount(); j++){
        int mCat=data->mechModel->data(data->mechModel->index(j,0)).toInt();
        if (!meh.contains(mCat)){
            meh.push_back(mCat);
        }
    }
    QTextTable *mechTable0 = cursor.insertTable(data->mechModel->rowCount()+meh.size(),3,tableMechFormat);
    QString sig, prefix, val;
    int pos=0;
    for (int n=0; n<meh.size(); n++){
            mechTable0->cellAt(pos,0).firstCursorPosition().setBlockFormat(formatCenter);
            QString nameCat=data->mechCategory->data(data->mechCategory->index(meh.at(n),1)).toString();
            if (n==0) {
                nameCat+=tr("\n Испытания: согласно EN 10204 - 3.1");
            }
            mechTable0->cellAt(pos,0).firstCursorPosition().insertText(nameCat,textBoldFormat);
            mechTable0->mergeCells(pos,0,1,3);
            pos++;
            for (int i=0; i<data->mechModel->rowCount(); i++){
                if (data->mechModel->data(data->mechModel->index(i,0)).toInt()==meh.at(n)){
                    head=data->mechModel->data(data->mechModel->index(i,1)).toString();
                    sig=data->mechModel->data(data->mechModel->index(i,2)).toString();
                    prefix=data->mechModel->data(data->mechModel->index(i,3)).toString();
                    val=data->mechModel->data(data->mechModel->index(i,4)).isNull() ? "" : QLocale().toString(data->mechModel->data(data->mechModel->index(i,4)).toDouble(),'f',2);

                    mechTable0->cellAt(pos,0).firstCursorPosition().setBlockFormat(formatLeft);
                    mechTable0->cellAt(pos,0).firstCursorPosition().setCharFormat(textTableFormat);
                    QTextDocument hd;
                    hd.setHtml(head);
                    QTextCursor chd(&hd);
                    chd.select(QTextCursor::Document);
                    chd.mergeCharFormat(textTableFormat);
                    mechTable0->cellAt(pos,0).firstCursorPosition().insertFragment(chd.selection());

                    mechTable0->cellAt(pos,1).firstCursorPosition().setBlockFormat(formatCenter);
                    mechTable0->cellAt(pos,1).firstCursorPosition().setCharFormat(textTableFormat);                   
                    QTextDocument sd;
                    sd.setHtml(sig);
                    QTextCursor csd(&sd);
                    csd.select(QTextCursor::Document);
                    csd.mergeCharFormat(textTableFormat);
                    mechTable0->cellAt(pos,1).firstCursorPosition().insertFragment(csd.selection());

                    mechTable0->cellAt(pos,2).firstCursorPosition().setBlockFormat(formatRirht);
                    mechTable0->cellAt(pos,2).firstCursorPosition().insertText(prefix+" "+val,textTableFormat);
                    pos++;
                }
            }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.insertBlock();

    if (data->sertModel->rowCount()){
        cursor.setBlockFormat(formatCenter);
        cursor.insertText(tr("Аттестация и сертификация"),textTitleFormat);
        cursor.insertBlock();
        QTextTable *sertTable = cursor.insertTable(data->sertModel->rowCount()+1,4,tableFormat);
        sertTable->cellAt(0,0).firstCursorPosition().setBlockFormat(formatCenter);
        sertTable->cellAt(0,0).firstCursorPosition().insertText(tr("Вид документа"),textBoldFormat);
        sertTable->cellAt(0,1).firstCursorPosition().setBlockFormat(formatCenter);
        sertTable->cellAt(0,1).firstCursorPosition().insertText(tr("Сертифицирующий орган"),textBoldFormat);
        sertTable->cellAt(0,2).firstCursorPosition().setBlockFormat(formatCenter);
        sertTable->cellAt(0,2).firstCursorPosition().insertText(tr("Номер документа"),textBoldFormat);
        sertTable->cellAt(0,3).firstCursorPosition().setBlockFormat(formatCenter);
        sertTable->cellAt(0,3).firstCursorPosition().insertText(tr("Дата"),textBoldFormat);
        QString vid, organ, nom, dat;
        for(int i=0; i<data->sertModel->rowCount(); i++){
            vid=data->sertModel->data(data->sertModel->index(i,1)).toString();
            vid=vid.simplified();
            organ=data->sertModel->data(data->sertModel->index(i,2)).toString();
            organ=organ.simplified();
            nom=data->sertModel->data(data->sertModel->index(i,3)).toString();
            nom=nom.simplified();
            dat=data->sertModel->data(data->sertModel->index(i,4)).toDate().toString("dd.MM.yy");
            sertTable->cellAt(i+1,0).firstCursorPosition().setBlockFormat(formatCenter);
            sertTable->cellAt(i+1,0).firstCursorPosition().insertText(vid,textTableFormat);
            sertTable->cellAt(i+1,1).firstCursorPosition().setBlockFormat(formatCenter);
            sertTable->cellAt(i+1,1).firstCursorPosition().insertText(organ,textTableFormat);
            sertTable->cellAt(i+1,2).firstCursorPosition().setBlockFormat(formatCenter);
            sertTable->cellAt(i+1,2).firstCursorPosition().insertText(nom,textTableFormat);
            sertTable->cellAt(i+1,3).firstCursorPosition().setBlockFormat(formatCenter);
            sertTable->cellAt(i+1,3).firstCursorPosition().insertText(dat,textTableFormat);
        }
        cursor.movePosition(QTextCursor::End);
    }

    cursor.setBlockFormat(formatLeft);

    for (int i=0; i<odobr.size(); i++){
        cursor.insertText(odobr.at(i),textNormalFormat);
        cursor.insertBlock();
    }

    QString gto="";
    for (int i=0; i<data->sertModel->rowCount(); i++){
        gto+=data->sertModel->data(data->sertModel->index(i,5)).toString();
    }
    if (!gto.isEmpty()) {
        cursor.insertText(tr("Группы технических устройств "),textBoldFormat);
        cursor.insertText(gto,textNormalFormat);
        cursor.insertBlock();
    }

    if (is_ship){
        cursor.insertText(tr("Грузополучатель: "),textBoldFormat);
        cursor.insertText(data->poluch,textNormalFormat);
        cursor.insertBlock();
    }
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(tr("При переписке по вопросам качества просьба ссылаться на номер партии"),textNormalFormat);
    cursor.insertBlock();
    cursor.setBlockFormat(formatLeft);
    cursor.insertText(tr("Заключение: "),textBoldFormat);
    cursor.insertText(tr("соответствует "),textNormalFormat);
    for (int i=0; i<data->tuList.size(); i++){
        cursor.insertText(data->tuList.at(i),textNormalFormat);
        if (i!=data->tuList.size()-1)
            cursor.insertText(tr(", "),textNormalFormat);
    }
    cursor.insertBlock();
    cursor.setBlockFormat(formatCenter);
    QDate date;
    date=(is_ship)? data->dateVidSert : QDate::currentDate();
    cursor.insertText(tr("Дата ")+date.toString("dd.MM.yy"),textNormalFormat);
    cursor.insertBlock();

    cursor.setBlockFormat(formatLeft);
    addResource(QTextDocument::ImageResource, QUrl("qrcode"), data->qrCode);
    QTextImageFormat qrformat;
    qrformat.setName("qrcode");
    qrformat.setHeight(150);
    cursor.insertImage(qrformat);

    cursor.insertText("                                    ",textNormalFormat);
    if (prn) {
        addResource(QTextDocument::ImageResource, QUrl("sign"), data->sign);
        QTextImageFormat signformat;
        signformat.setName("sign");
        signformat.setHeight(150);
        cursor.insertImage(signformat);
    } else {
        cursor.insertText(tr("Начальник ОТК______________")+data->otk,textNormalFormat);
    }
}

void SertBuild::setPrn(bool p)
{
    prn=p;
    this->build(current_id,current_is_ship);
}

DataSert::DataSert(QObject *parent) : QObject(parent)
{
    chemModel = new QSqlQueryModel(this);
    mechModel = new QSqlQueryModel(this);
    sertModel = new QSqlQueryModel(this);
    mechCategory = new QSqlQueryModel(this);
    refreshMechCategory();
    QSqlQuery query;
    query.prepare("select adr, telboss||', '||telfax||', '||teldop||' '||site||' '||email, otk from hoz where id=1");
    if (query.exec()){
        while(query.next()){
            adres=query.value(0).toString();
            contact=query.value(1).toString();
            otk=query.value(2).toString();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
    logo.load("images/logo2.png");
    sign.load("images/nach.png");
}

void DataSert::refresh(int id, bool is_ship)
{
    QSqlQuery query;
    QString sQuery;
    sQuery= is_ship ? QString("select p.id, p.n_s, p.yea, p.dat_part, e.marka_sert, p.diam, "
                              "gt.nam, pu.nam, p.ibco, v.nam, pol.naim, s.nom_s, s.dat_vid, o.massa "
                              "from otpusk as o "
                              "inner join sertifikat as s on o.id_sert=s.id "
                              "inner join parti as p on o.id_part=p.id "
                              "inner join elrtr as e on e.id=p.id_el "
                              "inner join provol as v on v.id=e.id_gost "
                              "inner join poluch as pol on s.id_pol=pol.id "
                              "inner join gost_types as gt on e.id_gost_type=gt.id "
                              "inner join purpose as pu on e.id_purpose=pu.id "
                              "where o.id = :id") :
                  QString("select p.id, p.n_s, p.yea, p.dat_part, e.marka_sert, p.diam, "
                          "gt.nam, pu.nam, p.ibco, v.nam, NULL, NULL, NULL, j.sum "
                          "from parti as p "
                          "inner join elrtr as e on e.id=p.id_el "
                          "inner join provol as v on v.id=e.id_gost "
                          "inner join gost_types as gt on e.id_gost_type=gt.id "
                          "inner join purpose as pu on e.id_purpose=pu.id "
                          "left outer join (select id_part as id, sum(kvo) as sum from part_prod group by id_part) as j on j.id=p.id "
                          "where p.id = :id");
    query.prepare(sQuery);
    query.bindValue(":id",id);
    if (query.exec()){
        while(query.next()){
            id_parti=query.value(0).toInt();
            nomPart=query.value(1).toString();
            yearPart=query.value(2).toInt();
            datePart=query.value(3).toDate();
            marka=query.value(4).toString();
            diam=query.value(5).toDouble();
            tip1=query.value(6).toString();
            tip2=query.value(7).toString();
            znam=query.value(8).toString();
            prov=query.value(9).toString();
            poluch=query.value(10).toString();
            nomSert=query.value(11).toString();
            dateVidSert=query.value(12).toDate();
            netto=query.value(13).toDouble();
        }
        refreshTu();
        refreshChem();
        refreshMech();
        refreshSert();
        refreshQR(id,is_ship);
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}

void DataSert::refreshTu()
{
    QSqlQuery tuQuery;
    tuQuery.prepare("select nam "
                    "from zvd_get_tu((select dat_part from parti where id = :id1 ), "
                    "(select id_el from parti where id = :id2 ), "
                    "(select d.id from diam as d where d.diam = (select diam from parti where id = :id3 )) ) ");
    tuQuery.bindValue(":id1",id_parti);
    tuQuery.bindValue(":id2",id_parti);
    tuQuery.bindValue(":id3",id_parti);
    tuList.clear();
    if (tuQuery.exec()){
        while(tuQuery.next()){
            tuList.push_back(tuQuery.value(0).toString());
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),tuQuery.lastError().text(),QMessageBox::Ok);
    }
}

void DataSert::refreshChem()
{
    chemModel->setQuery("select c.sig, s.value from sert_chem as s "
                        "inner join chem_tbl as c on s.id_chem=c.id "
                        "where s.id_part="+QString::number(id_parti)+" order by c.sig");
    if (chemModel->lastError().isValid())
        QMessageBox::critical(NULL,"Error",chemModel->lastError().text(),QMessageBox::Ok);
}

void DataSert::refreshMech()
{
    mechModel->setQuery("select m.id_cat, m.nam_html, m.sig_html, m.prefix, s.value "
                        "from sert_mech as s "
                        "inner join mech_tbl as m on s.id_mech=m.id "
                        "where s.id_part= "+QString::number(id_parti)+" "
                        "union "
                        "select t.id_cat, t.nam, null, n.nam, null from sert_mechx as x "
                        "inner join mechx_tbl as t on x.id_mechx=t.id "
                        "inner join mechx_nams as n on x.id_value=n.id "
                        "where x.id_part= "+QString::number(id_parti)+" order by 2");
    if (mechModel->lastError().isValid())
        QMessageBox::critical(NULL,"Error",mechModel->lastError().text(),QMessageBox::Ok);
}

void DataSert::refreshSert()
{
    QString id_part=QString::number(id_parti);
    sertModel->setQuery("select z.id_ved, z.doc_nam, z.ved_nam, z.nom_doc, z.dat_doc, z.gr_tech_ust, z.id_doc_t, z.ved_short, z.grade_nam "
                        "from zvd_get_sert((select dat_part from parti where id = "+id_part+" ), "
                        "(select id_el from parti where id = "+id_part+" ), "
                        "(select d.id from diam as d where d.diam = (select diam from parti where id = "+id_part+" )) ) as z");
    if (sertModel->lastError().isValid())
        QMessageBox::critical(NULL,"Error",sertModel->lastError().text(),QMessageBox::Ok);
}

void DataSert::refreshQR(int id, bool is_ship)
{
    QString str;
    str+="СЕРТИФИКАТ КАЧЕСТВА №"+nomPart.toUtf8()+"-"+QString::number(yearPart).toUtf8();
    if (is_ship) str+="/"+nomSert.toUtf8();
    str+="\n";
    str+="Марка ";
    str+=marka.toUtf8();
    str+="\n";
    str+="Диаметр, мм ";
    str+=QString::number(diam).toUtf8();
    str+="\n";
    str+="Номер партии "+nomPart.toUtf8();
    str+="\n";
    str+="Дата производства "+datePart.toString("dd.MM.yy").toUtf8();
    str+="\n";
    str+="Масса нетто, кг "+QString::number(netto).toUtf8();
    str+="\n";
    if (is_ship) str+="Грузополучатель: "+poluch.toUtf8()+"\n";
    QDate date;
    date=(is_ship)? dateVidSert : QDate::currentDate();
    str+="Дата "+date.toString("dd.MM.yy").toUtf8()+"\n";
    u_int32_t id_part=id_parti;
    u_int32_t id_ship= is_ship ? id : 0;
    u_int64_t cod=0;

    memcpy((u_int8_t*)&cod,(u_int8_t*)&id_ship,4);
    memcpy((u_int8_t*)&cod+4,(u_int8_t*)&id_part,4);

    str+="Код подлинности "+QString::number(cod).toUtf8();
    QRcode *qr = QRcode_encodeString(str.toStdString().c_str(), 1, QR_ECLEVEL_L, QR_MODE_8, 0);
    bool ok= qr!=0;
    const int scale=10;
    int s=1;
    if (ok) s=(qr->width>0)? qr->width : 1;
    QImage img(s*scale,s*scale,QImage::Format_RGB32);
    QPainter painter(&img);
    if(ok){
        QColor fg("black");
        QColor bg("white");
        painter.setBrush(bg);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0,0,s*scale,s*scale);
        painter.setBrush(fg);
        for(int y=0;y<s;y++){
            const int yy=y*s;
            for(int x=0;x<s;x++){
                const int xx=yy+x;
                const unsigned char b=qr->data[xx];
                if(b & 0x01){
                    const double rx1=x*scale, ry1=y*scale;
                    QRectF r(rx1, ry1, scale, scale);
                    painter.drawRects(&r,1);
                }
            }
        }
        QRcode_free(qr);
    }
    else{
        QColor error("red");
        painter.setBrush(error);
        painter.drawRect(0,0,s*scale,s*scale);
    }
    qrCode=img;
}

void DataSert::refreshMechCategory()
{
    mechCategory->setQuery("select id, nam from mech_category");
    if (mechCategory->lastError().isValid())
        QMessageBox::critical(NULL,"Error",mechCategory->lastError().text(),QMessageBox::Ok);
}

