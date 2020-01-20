#include "sertbuild.h"

SertBuild::SertBuild(QObject *parent) :
    QTextDocument(parent)
{
    data=new DataSert(this);
    prn=false;
    l_rus=true;
    l_en=false;
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

    cursor.insertBlock(formatCenter,textNormalFormat);

    addResource(QTextDocument::ImageResource, QUrl("logo"), data->logo);
    QTextImageFormat f;
    f.setName("logo");
    f.setHeight(70);
    cursor.insertImage(f);

    cursor.insertBlock(formatRirht,textAdrFormat);
    cursor.setCharFormat(textAdrFormat);
    insertText(cursor,data->adres,data->adres_en,true,false);
    cursor.insertBlock(formatRirht,textAdrFormat);
    cursor.insertText(data->contact,textAdrFormat);
    cursor.insertBlock(formatLeft);

    QVector<int> id_ved;
    QHash<QString,QString> odobr;
    for (int i=0; i<data->sertModel->rowCount(); i++){
        int ved=data->sertModel->data(data->sertModel->index(i,0),Qt::EditRole).toInt();

        if (!id_ved.contains(ved)){
            id_ved.push_back(ved);
        }

        int id_doc_t=data->sertModel->data(data->sertModel->index(i,6),Qt::EditRole).toInt();
        if (id_doc_t==3){
                QString str("Одобрено ");
                QString str_en("Approved by ");
                str+=data->sertModel->data(data->sertModel->index(i,7),Qt::EditRole).toString();
                str_en+=data->sertModel->data(data->sertModel->index(i,9),Qt::EditRole).toString();
                QString cat=data->sertModel->data(data->sertModel->index(i,8),Qt::EditRole).toString();
                QString sh=str+QString(".");
                if (!cat.isEmpty()){
                    str+=QString(" по категории ")+cat;
                    str_en+=QString(" in category ")+cat;
                }
                str+=QString(".");
                str_en+=QString(".");
            if (odobr.contains(sh) && (str!=sh)){
                odobr.remove(sh);
            }
            QStringList lst=odobr.keys();
            if (lst.indexOf(QRegExp("^"+str+".*"))==-1) {
                odobr.insert(str,str_en);
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
    cursor.setCharFormat(textTitleFormat);
    insertText(cursor,tr("СЕРТИФИКАТ КАЧЕСТВА"),tr("QUALITY CERTIFICATE"));

    cursor.insertText(tr(" №")+data->nomPart+"-"+QString::number(data->yearPart),textTitleFormat);
    if (is_ship){
        cursor.insertText("/"+data->nomSert,textTitleFormat);
    }
    cursor.insertBlock(formatLeft);
    cursor.setCharFormat(textBoldFormat);
    insertText(cursor,tr("Нормативная документация"),tr("Normative documents"));
    cursor.insertText(tr(": "),textBoldFormat);
    QString tulist;

    for (int i=0; i<data->tuList.size(); i++){
        tulist.push_back(data->tuList.at(i));
        if (i!=data->tuList.size()-1){
            tulist+=", ";
        }
    }
    cursor.insertText(tulist,textNormalFormat);
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
    cursor.setBlockCharFormat(textBoldFormat);
    insertText(cursor,tr("Условное обозначение электрода"),tr("Electrode symbol"),true);

    cursor=mainTable->cellAt(0,1).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setBlockCharFormat(textBoldFormat);
    insertText(cursor,tr("Проволока \nпо ГОСТ 2246-70"),tr("Wire \naccording to ГОСТ 2246-70"),true);

    cursor=mainTable->cellAt(0,2).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setBlockCharFormat(textBoldFormat);
    insertText(cursor,tr("Номер партии"),tr("Batch number"),true);

    cursor=mainTable->cellAt(0,3).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setBlockCharFormat(textBoldFormat);
    insertText(cursor,tr("Дата \nпроизводства"),tr("Date of manufacture"),true);

    cursor=mainTable->cellAt(0,4).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setBlockCharFormat(textBoldFormat);
    if (is_ship){
        insertText(cursor,tr("Объем \nотгрузки, кг"),tr("Shipment volume, kg"),true);
    } else {
        insertText(cursor,tr("Объем \nвыпуска, кг"),tr("Volume of release, kg"),true);
    }

    mainTable->insertRows(mainTable->rows(),1);
    cursor=mainTable->cellAt(1,0).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    QString ob= (data->tip1.isNull() || data->tip2.isNull() || data->znam.isNull() || data->tip1==QString("-"))?
                (data->marka+"-"+tr("∅")+QString::number(data->diam)) : (data->tip1+"-"+data->marka+"-"+tr("∅")+QString::number(data->diam)+"-"+data->tip2+"\n"+data->znam);
    cursor.insertText(ob,textTableFormat);
    cursor=mainTable->cellAt(1,1).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(data->prov,textTableFormat);
    cursor=mainTable->cellAt(1,2).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(data->nomPart,textTableFormat);
    cursor=mainTable->cellAt(1,3).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textTableFormat);
    insertDate(cursor,data->datePart);
    cursor=mainTable->cellAt(1,4).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textTableFormat);
    insertDouble(cursor,data->netto,1);

    cursor.movePosition(QTextCursor::End);
    cursor.setBlockFormat(formatCenter);
    cursor.insertBlock();
    cursor.insertBlock();

    int row=data->chemModel->rowCount();
    if (row) {
        QTextTable *chemTable = cursor.insertTable(3, row, tableFormat);
        cursor=chemTable->cellAt(0,0).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Испытания: согласно EN 10204 - 3.1. Химический состав наплавленного металла, %"),tr("Testing: according to EN 10204 - 3.1. Chemical composition of weld metal, %"),true);
        chemTable->mergeCells(0,0,1,row);
        for (int i=0; i<row; i++)
        {
            QString head=data->chemModel->data(data->chemModel->index(i,0)).toString();
            QVariant value=data->chemModel->data(data->chemModel->index(i,1));
            if (!value.isNull()){
                cursor=chemTable->cellAt(2,i).firstCursorPosition();
                cursor.setBlockFormat(formatCenter);
                cursor.setCharFormat(textTableFormat);
                insertDouble(cursor,value,3);
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
    //qDebug()<<meh;
    QTextTable *mechTable0 = cursor.insertTable(data->mechModel->rowCount()+meh.size(),3,tableMechFormat);
    int pos=0;
    for (int n=0; n<meh.size(); n++){
            QString sig, prefix, head, head_en, sig_en, prefix_en;
            mechTable0->cellAt(pos,0).firstCursorPosition().setBlockFormat(formatCenter);
            QString nameCat;
            QString nameCat_en;
            if (n==0) {
                nameCat+=tr("Испытания: согласно EN 10204 - 3.1. ");
                nameCat_en+=tr("Tests: according to EN 10204 - 3.1. ");
            }
            nameCat+=data->mechCategory->data(data->mechCategory->index(meh.at(n),1)).toString();
            nameCat_en+=data->mechCategory->data(data->mechCategory->index(meh.at(n),2)).toString();
            cursor=mechTable0->cellAt(pos,0).firstCursorPosition();
            cursor.setBlockFormat(formatCenter);
            cursor.setCharFormat(textBoldFormat);
            insertText(cursor,nameCat,nameCat_en,true);
            mechTable0->mergeCells(pos,0,1,3);
            pos++;
            for (int i=0; i<data->mechModel->rowCount(); i++){
                if (data->mechModel->data(data->mechModel->index(i,0)).toInt()==meh.at(n)){
                    head=data->mechModel->data(data->mechModel->index(i,1)).toString();
                    head_en=data->mechModel->data(data->mechModel->index(i,5)).toString();
                    sig=data->mechModel->data(data->mechModel->index(i,2)).toString();
                    sig_en=data->mechModel->data(data->mechModel->index(i,6)).toString();
                    prefix=data->mechModel->data(data->mechModel->index(i,3)).toString();
                    prefix_en=data->mechModel->data(data->mechModel->index(i,7)).toString();
                    QVariant val=data->mechModel->data(data->mechModel->index(i,4));

                    cursor= mechTable0->cellAt(pos,0).firstCursorPosition();
                    cursor.setBlockFormat(formatLeft);
                    cursor.setCharFormat(textTableFormat);
                    insertText(cursor,head,head_en,true,true,true);

                    cursor=mechTable0->cellAt(pos,1).firstCursorPosition();
                    cursor.setBlockFormat(formatCenter);
                    cursor.setCharFormat(textTableFormat);
                    insertText(cursor,sig,sig_en,true,true,true);

                    mechTable0->cellAt(pos,2).firstCursorPosition().setBlockFormat(formatRirht);
                    cursor=mechTable0->cellAt(pos,2).firstCursorPosition();
                    cursor.setBlockFormat(formatRirht);
                    cursor.setCharFormat(textTableFormat);
                    insertText(cursor,prefix,prefix_en,true,true);
                    if (!prefix.isEmpty()){
                        cursor.insertText(" ",textTableFormat);
                    }
                    insertDouble(cursor,val,2);

                    pos++;
                }
            }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.insertBlock();

    if (data->sertModel->rowCount()){
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textTitleFormat);
        insertText(cursor,tr("Аттестация и сертификация"),tr("Certification"),false,true);
        cursor.insertBlock();
        QTextTable *sertTable = cursor.insertTable(data->sertModel->rowCount()+1,4,tableFormat);
        cursor=sertTable->cellAt(0,0).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Вид документа"),tr("Type of document"),true);
        cursor=sertTable->cellAt(0,1).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Сертифицирующий орган"),tr("Certification authority"),true);
        cursor=sertTable->cellAt(0,2).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Номер документа"),tr("Document Number"),true);
        cursor=sertTable->cellAt(0,3).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Дата"),tr("Date"),true);

        for(int i=0; i<data->sertModel->rowCount(); i++){
            QString vid=data->sertModel->data(data->sertModel->index(i,1)).toString();
            QString vid_en=data->sertModel->data(data->sertModel->index(i,10)).toString();
            vid=vid.simplified();
            QString organ=data->sertModel->data(data->sertModel->index(i,2)).toString();
            QString organ_en=data->sertModel->data(data->sertModel->index(i,11)).toString();
            organ=organ.simplified();
            QString nom=data->sertModel->data(data->sertModel->index(i,3)).toString();
            nom=nom.simplified();
            QDate dat=data->sertModel->data(data->sertModel->index(i,4)).toDate();
            cursor=sertTable->cellAt(i+1,0).firstCursorPosition();
            cursor.setBlockFormat(formatCenter);
            cursor.setCharFormat(textTableFormat);
            insertText(cursor,vid,vid_en,true);
            cursor=sertTable->cellAt(i+1,1).firstCursorPosition();
            cursor.setBlockFormat(formatCenter);
            cursor.setCharFormat(textTableFormat);
            insertText(cursor,organ,organ_en,true);

            sertTable->cellAt(i+1,2).firstCursorPosition().setBlockFormat(formatCenter);
            sertTable->cellAt(i+1,2).firstCursorPosition().insertText(nom,textTableFormat);

            cursor=sertTable->cellAt(i+1,3).firstCursorPosition();
            cursor.setBlockFormat(formatCenter);
            cursor.setCharFormat(textTableFormat);
            insertDate(cursor,dat);
        }
        cursor.movePosition(QTextCursor::End);
    }

    cursor.setBlockFormat(formatLeft);
    cursor.setCharFormat(textNormalFormat);

    QHash<QString, QString>::const_iterator it = odobr.constBegin();
    while (it != odobr.constEnd()) {
        insertText(cursor,it.key(),it.value());
        cursor.insertBlock();
        ++it;
    }

    /*QString gto="";
    for (int i=0; i<data->sertModel->rowCount(); i++){
        gto+=data->sertModel->data(data->sertModel->index(i,5)).toString();
    }
    if (!gto.isEmpty()) {
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Группы технических устройств"),tr("Groups of technical devices"));
        cursor.insertText(": ",textBoldFormat);
        cursor.insertText(gto,textNormalFormat);
        cursor.insertBlock();
    }*/

    if (is_ship){
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Грузополучатель"),tr("Consignee"));
        cursor.insertText(":\n",textBoldFormat);
        cursor.setCharFormat(textNormalFormat);
        insertText(cursor,data->poluch,data->poluch_en,true);
        cursor.insertBlock();
    }
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textNormalFormat);
    insertText(cursor,tr("При переписке по вопросам качества просьба ссылаться на номер партии"),tr("When correspondence on quality issues, please refer to the batch number"),true);
    cursor.insertBlock();
    cursor.setBlockFormat(formatLeft);
    cursor.setCharFormat(textBoldFormat);
    insertText(cursor,tr("Заключение"),tr("Conclusion"));
    cursor.insertText(tr(":\n"),textBoldFormat);
    cursor.setCharFormat(textNormalFormat);
    insertText(cursor,tr("соответствует ")+tulist,tr("corresponds to ")+tulist,true);

    cursor.insertBlock();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textBoldFormat);
    QDate date;
    date=(is_ship)? data->dateVidSert : QDate::currentDate();
    insertText(cursor,tr("Дата"),tr("Date"),false);
    cursor.insertText(tr(": "),textBoldFormat);
    cursor.setCharFormat(textNormalFormat);
    insertDate(cursor,date,false);
    cursor.insertBlock();

    cursor.setBlockFormat(formatLeft);
    addResource(QTextDocument::ImageResource, QUrl("qrcode"), data->qrCode);
    QTextImageFormat qrformat;
    qrformat.setName("qrcode");
    qrformat.setHeight(150);
    cursor.insertImage(qrformat);

    cursor.insertText("   ",textNormalFormat);
    QString nach=tr("Начальник ОТК");
    QString nach_en=tr("Head of Quality Department");
    QString line=tr("______________");
    if (prn) {
        QImage im(data->sign);
        QPainter p(&im);
        QFont f(textNormalFormat.font());
        f.setPointSize(44);
        p.setFont(f);
        int pos=100;
        QString str;
        if (l_en && !l_rus){
            pos=400;
            str=nach_en+line+data->otk_en;
        } else if (l_rus && !l_en){
            pos=630;
            str=nach+line+data->otk;
        } else if (l_rus && l_en){
            pos=50;
            str=nach+" / "+nach_en+line+data->otk+" / "+data->otk_en;
        }
        p.drawText(pos,150,str);
        addResource(QTextDocument::ImageResource, QUrl("sign"), im);
        QTextImageFormat signformat;
        signformat.setName("sign");
        signformat.setHeight(150);
        cursor.insertImage(signformat);
    } else {
        cursor.setCharFormat(textNormalFormat);
        if (l_en && !l_rus){
            cursor.insertText("                               ",textNormalFormat);
        } else if (l_rus && !l_en){
            cursor.insertText("                                                    ",textNormalFormat);
        }
        insertText(cursor,nach,nach_en);
        cursor.insertText(line,textNormalFormat);
        insertText(cursor,data->otk,data->otk_en);
    }
}

void SertBuild::insertText(QTextCursor &c, const QString &rus, const QString &eng, bool newpar, bool sep, bool html)
{
    QTextBlockFormat blockFormat=c.blockFormat();
    QTextCharFormat charFormat=c.charFormat();
    if (l_rus || eng.isEmpty() || rus==eng){
        if (html){
            insertHtml(c,rus);
        } else {
            c.insertText(rus,charFormat);
        }
    }
    if (l_en && !eng.isEmpty() && rus!=eng){
        if (l_rus){
                c.insertText(sep ? " / " : " ",charFormat);
            if (newpar){
                c.insertBlock(blockFormat);
            }
        }
        if (html){
            insertHtml(c,eng);
        } else {
            c.insertText(eng,charFormat);
        }
    }
}

void SertBuild::insertHtml(QTextCursor &c, const QString &html)
{
    QTextDocument hd;
    QTextCursor chd(&hd);
    chd.setBlockFormat(c.blockFormat());
    chd.setCharFormat(c.charFormat());
    chd.insertHtml(html);
    chd.select(QTextCursor::Document);
    chd.mergeCharFormat(c.charFormat());
    chd.mergeBlockFormat(c.blockFormat());
    c.insertFragment(chd.selection());
}

void SertBuild::insertDouble(QTextCursor &c, const QVariant &val, int dec)
{
    if (!val.isNull()){
        QLocale lrus(QLocale::Russian);
        bool ok=false;
        double v=val.toDouble(&ok);
        if (ok) {
            if (l_rus && !l_en){
                c.insertText(lrus.toString(v,'f',dec));
            } else {
                c.insertText(QString::number(v,'f',dec));
            }
        }
    }
}

void SertBuild::insertDate(QTextCursor &c, const QDate &date, bool newpar)
{
    QLocale lrus(QLocale::Russian);
    QLocale leng(QLocale::English);
    if (l_rus && !l_en){
        c.insertText(lrus.toString(date,QLocale::ShortFormat));
    }
    if (l_en){
        if (l_rus){
            c.insertText(lrus.toString(date,"dd MMM yyyy")+" / ");
            if(newpar){
                c.insertText("\n");
            }
        }
        c.insertText(leng.toString(date,"MMM dd, yyyy"));
    }
}

void SertBuild::setPrn(bool p)
{
    prn=p;
    this->build(current_id,current_is_ship);
}

void SertBuild::setLRus(bool b)
{
    if (b){
        l_rus=true;
        l_en=false;
        this->build(current_id,current_is_ship);
    }
}

void SertBuild::setLEn(bool b)
{
    if (b){
        l_rus=false;
        l_en=true;
        this->build(current_id,current_is_ship);
    }
}

void SertBuild::setLMix(bool b)
{
    if (b){
        l_rus=true;
        l_en=true;
        this->build(current_id,current_is_ship);
    }
}

DataSert::DataSert(QObject *parent) : QObject(parent)
{
    chemModel = new QSqlQueryModel(this);
    mechModel = new QSqlQueryModel(this);
    sertModel = new QSqlQueryModel(this);
    mechCategory = new QSqlQueryModel(this);
    refreshMechCategory();
    QSqlQuery query;
    query.prepare("select adr, telboss||', '||telfax||', '||teldop||' '||site||' '||email, otk, adr_en, otk_en from hoz where id=1");
    if (query.exec()){
        while(query.next()){
            adres=query.value(0).toString();
            contact=query.value(1).toString();
            otk=query.value(2).toString();
            adres_en=query.value(3).toString();
            otk_en=query.value(4).toString();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
    logo.load("images/logo2.png");
    sign.load("images/otk.png");
}

void DataSert::refresh(int id, bool is_ship)
{
    QSqlQuery query;
    QString sQuery;
    sQuery= is_ship ? QString("select p.id, p.n_s, p.yea, p.dat_part, e.marka_sert, p.diam, "
                              "gt.nam, pu.nam, p.ibco, v.nam, pol.naim, s.nom_s, s.dat_vid, o.massa, pol.naim_en "
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
                          "gt.nam, pu.nam, p.ibco, v.nam, NULL, NULL, NULL, j.sum, NULL "
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
            poluch_en=query.value(14).toString();
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
    mechModel->setQuery("select m.id_cat, m.nam_html, m.sig_html, m.prefix, s.value, m.nam_html_en, m.sig_html_en, m.prefix_en "
                        "from sert_mech as s "
                        "inner join mech_tbl as m on s.id_mech=m.id "
                        "where s.id_part= "+QString::number(id_parti)+" "
                        "union "
                        "select t.id_cat, t.nam, null, n.nam, null, t.nam_en, null, n.nam_en from sert_mechx as x "
                        "inner join mechx_tbl as t on x.id_mechx=t.id "
                        "inner join mechx_nams as n on x.id_value=n.id "
                        "where x.id_part= "+QString::number(id_parti)+" order by 2");
    if (mechModel->lastError().isValid())
        QMessageBox::critical(NULL,"Error",mechModel->lastError().text(),QMessageBox::Ok);
}

void DataSert::refreshSert()
{
    QString id_part=QString::number(id_parti);
    sertModel->setQuery("select z.id_ved, z.doc_nam, z.ved_nam, z.nom_doc, z.dat_doc, z.gr_tech_ust, z.id_doc_t, z.ved_short, z.grade_nam, z.ved_short_en, z.doc_nam_en, z.ved_nam_en "
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
    mechCategory->setQuery("select id, nam, nam_en from mech_category order by id");
    if (mechCategory->lastError().isValid())
        QMessageBox::critical(NULL,"Error",mechCategory->lastError().text(),QMessageBox::Ok);
}

