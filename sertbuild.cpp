#include "sertbuild.h"

SertBuild::SertBuild(QObject *parent) :
    QTextDocument(parent)
{
    data=new DataSert(this);
    sertType=0;
    l_rus=true;
    l_en=false;
    connect(data,SIGNAL(sigRefresh()),this,SIGNAL(sigRefresh()));
}

QString SertBuild::getNomPart()
{
    return data->head()->nomPart;
}

QString SertBuild::getYearSert()
{
    return QString::number(data->head()->dateVidSert.year());
}

QString SertBuild::getYearPart()
{
    return QString::number(data->head()->yearPart);
}

QString SertBuild::getNomSert()
{
    return data->head()->nomSert;
}

int SertBuild::getType()
{
    return sertType;
}

DataSert *SertBuild::sData()
{
    return data;
}

void SertBuild::build(int id, bool is_ship)
{
    data->refresh(id, is_ship,sertType==1);
    current_id=id;
    current_is_ship=is_ship;
    rebuild();
}

void SertBuild::rebuild()
{
    this->clear();

    QString nameEN=tr("Испытания: согласно EN 10204 - 3.1. ");
    QString nameEN_en=tr("Tests: according to EN 10204 - 3.1. ");
    if (sertType==3 || sertType==4){
        nameEN=tr("Испытания: согласно EN 10204 - 3.2. ");
        nameEN_en=tr("Tests: according to EN 10204 - 3.2. ");
    }

    QFont titleFont("Droid Serif",13);
    QFont normalFont("Droid Sans",10);
    QFont smallFont("Droid Sans",8);

    QFont cursiveFont(normalFont);
    cursiveFont.setItalic(true);

    QTextBlockFormat formatRirht;
    formatRirht.setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    QTextBlockFormat formatLeft;
    formatLeft.setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    QTextBlockFormat formatCenter;
    formatCenter.setAlignment(Qt::AlignCenter|Qt::AlignVCenter);

    QTextCharFormat textAdrFormat;
    textAdrFormat.setFont(cursiveFont);

    QTextCharFormat textTitleFormat;
    textTitleFormat.setFont(titleFont);

    QTextCharFormat textNormalFormat;
    textNormalFormat.setFont(normalFont);
    textNormalFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);

    QTextCharFormat textUnderlineFormat;
    textUnderlineFormat.setFont(normalFont);
    textUnderlineFormat.setFontUnderline(true);

    QTextCharFormat textSmallFormat;
    textSmallFormat.setFont(smallFont);
    textSmallFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);

    QTextCharFormat textBoldFormat;
    QFont boldFont(normalFont);
    boldFont.setBold(true);
    textBoldFormat.setFont(boldFont);
    textBoldFormat.setVerticalAlignment(QTextCharFormat::AlignMiddle);

    QTextCharFormat textTableFormat;
    QFont tableFont(normalFont);
    tableFont.setWeight(normalFont.weight()-1);
    textTableFormat.setFont(tableFont);

    QTextCursor cursor(this);
    cursor.movePosition(QTextCursor::Start);

    cursor.insertBlock(formatCenter,textNormalFormat);

    addResource(QTextDocument::ImageResource, QUrl("logo"), data->general()->logo);
    QTextImageFormat f;
    f.setName("logo");
    f.setHeight(70);
    cursor.insertImage(f);

    cursor.insertBlock(formatRirht,textAdrFormat);
    cursor.setCharFormat(textAdrFormat);
    insertText(cursor,data->general()->adres.rus,data->general()->adres.eng,true,false);
    cursor.insertBlock(formatRirht,textAdrFormat);
    cursor.insertText(data->general()->contact,textAdrFormat);
    cursor.insertBlock(formatLeft);

    QVector<int> id_ved;
    QHash<QString,QString> odobr;
    svData enSert;

    foreach (sertData s, *data->sert()){
        if (s.en){
            enSert.push_back(s);
            if (!id_ved.contains(s.id_ved)){
                id_ved.push_back(s.id_ved);
            }
            if (s.id_doc_t==3){
                QString str("Одобрено ");
                QString str_en("Approved by ");
                str+=s.ved_short.rus;
                str_en+=s.ved_short.eng;
                QString cat=s.grade_nam;
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
    }

    foreach (int id, id_ved){
        QImage img;
        img.loadFromData(Rels::instance()->mapVedPix.value(id));
        if (!img.isNull()) {
            addResource(QTextDocument::ImageResource, QUrl("vedimage"+QString::number(id)), img);
            QTextImageFormat f;
            f.setName("vedimage"+QString::number(id));
            f.setHeight(45);
            cursor.insertImage(f);
        }
    }

    cursor.insertBlock(formatCenter,textTitleFormat);
    cursor.setCharFormat(textTitleFormat);
    insertText(cursor,tr("СЕРТИФИКАТ КАЧЕСТВА"),tr("QUALITY CERTIFICATE"));

    cursor.insertText(tr(" №")+data->head()->nomPart+"-"+QString::number(data->head()->yearPart),textTitleFormat);
    if (current_is_ship){
        cursor.insertText("/"+data->head()->nomSert,textTitleFormat);
    }
    cursor.insertBlock(formatLeft);
    cursor.setCharFormat(textBoldFormat);
    insertText(cursor,tr("Нормативная документация"),tr("Normative documents"));
    cursor.insertText(tr(": "),textBoldFormat);
    QString tulist=data->tu();

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
    if (current_is_ship){
        insertText(cursor,tr("Объем \nотгрузки, кг"),tr("Shipment volume, kg"),true);
    } else {
        insertText(cursor,tr("Объем \nвыпуска, кг"),tr("Volume of release, kg"),true);
    }

    mainTable->insertRows(mainTable->rows(),1);
    cursor=mainTable->cellAt(1,0).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    QString ob= (data->head()->tip1.isNull() || data->head()->tip2.isNull() || data->head()->znam.isNull() || data->head()->tip1==QString("-"))?
                (data->head()->marka+"-"+tr("∅")+QString::number(data->head()->diam)) : (data->head()->tip1+"-"+data->head()->marka+"-"+tr("∅")+QString::number(data->head()->diam)+"-"+data->head()->tip2+"\n"+data->head()->znam);
    cursor.insertText(ob,textTableFormat);
    cursor=mainTable->cellAt(1,1).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(data->head()->prov,textTableFormat);
    cursor=mainTable->cellAt(1,2).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.insertText(data->head()->nomPart,textTableFormat);
    cursor=mainTable->cellAt(1,3).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textTableFormat);
    insertDate(cursor,data->head()->datePart);
    cursor=mainTable->cellAt(1,4).firstCursorPosition();
    cursor.setBlockFormat(formatCenter);
    cursor.setCharFormat(textTableFormat);
    insertDouble(cursor,data->head()->netto,1);

    cursor.movePosition(QTextCursor::End);
    cursor.setBlockFormat(formatCenter);
    cursor.insertBlock();
    cursor.insertBlock();

    int row=data->chem()->size();
    if (row) {
        QTextTable *chemTable = cursor.insertTable(3, row, tableFormat);
        cursor=chemTable->cellAt(0,0).firstCursorPosition();
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,nameEN+tr("Химический состав наплавленного металла, %"),nameEN_en+tr("Chemical composition of weld metal, %"),true);
        chemTable->mergeCells(0,0,1,row);
        int i=0;
        foreach (chemData c, *data->chem()) {
            if (!c.value.isNull()){
                cursor=chemTable->cellAt(2,i).firstCursorPosition();
                cursor.setBlockFormat(formatCenter);
                cursor.setCharFormat(textTableFormat);
                insertDouble(cursor,c.value,3);
                cursor=chemTable->cellAt(1,i).firstCursorPosition();
                cursor.setBlockFormat(formatCenter);
                cursor.insertText(c.name,textTableFormat);
                i++;
            }
        }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.setBlockFormat(formatCenter);
    cursor.insertBlock();
    cursor.insertBlock();

    QVector<int> meh;
    foreach (mechData m, *data->mech()){
        if (!meh.contains(m.id_cat)){
            meh.push_back(m.id_cat);
        }
    }

    QTextTable *mechTable0 = cursor.insertTable(data->mech()->size()+meh.size(),3,tableMechFormat);
    int pos=0;
    foreach (int id_cat, meh){
            mechTable0->cellAt(pos,0).firstCursorPosition().setBlockFormat(formatCenter);
            QString nameCat;
            QString nameCat_en;
            if (pos==0) {
                nameCat+=nameEN;
                nameCat_en+=nameEN_en;
            }
            nameCat+=data->mechCategory(id_cat).rus;
            nameCat_en+=data->mechCategory(id_cat).eng;
            cursor=mechTable0->cellAt(pos,0).firstCursorPosition();
            cursor.setBlockFormat(formatCenter);
            cursor.setCharFormat(textBoldFormat);
            insertText(cursor,nameCat,nameCat_en,true);
            mechTable0->mergeCells(pos,0,1,3);
            pos++;
            foreach (mechData m, *data->mech()){
                if (m.id_cat==id_cat){

                    cursor= mechTable0->cellAt(pos,0).firstCursorPosition();
                    cursor.setBlockFormat(formatLeft);
                    cursor.setCharFormat(textTableFormat);
                    insertText(cursor,m.nam_html.rus,m.nam_html.eng,true,true,true);

                    cursor=mechTable0->cellAt(pos,1).firstCursorPosition();
                    cursor.setBlockFormat(formatCenter);
                    cursor.setCharFormat(textTableFormat);
                    insertText(cursor,m.sig_htlm.rus,m.sig_htlm.eng,true,true,true);

                    cursor=mechTable0->cellAt(pos,2).firstCursorPosition();
                    cursor.setBlockFormat(formatRirht);
                    cursor.setCharFormat(textTableFormat);
                    insertText(cursor,m.prefix.rus,m.prefix.eng,true,true);
                    if (!m.prefix.rus.isEmpty()){
                        cursor.insertText(" ",textTableFormat);
                    }
                    insertDouble(cursor,m.value,2);

                    pos++;
                }
            }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.insertBlock();

    if (enSert.size()){
        cursor.setBlockFormat(formatCenter);
        cursor.setCharFormat(textTitleFormat);
        insertText(cursor,tr("Аттестация и сертификация"),tr("Certification"),false,true);
        cursor.insertBlock();
        QTextTable *sertTable = cursor.insertTable(enSert.size()+1,4,tableFormat);
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
        insertText(cursor,tr("Дата выдачи"),tr("Date of issue"),true);
        int i=0;
        foreach (sertData s, enSert){

            cursor=sertTable->cellAt(i+1,0).firstCursorPosition();
            cursor.setBlockFormat(formatCenter);
            cursor.setCharFormat(textTableFormat);
            insertText(cursor,s.doc_nam.rus,s.doc_nam.eng,true);

            cursor=sertTable->cellAt(i+1,1).firstCursorPosition();
            cursor.setBlockFormat(formatCenter);
            cursor.setCharFormat(textTableFormat);
            insertText(cursor,s.ved_nam.rus,s.ved_nam.eng,true);

            sertTable->cellAt(i+1,2).firstCursorPosition().setBlockFormat(formatCenter);
            sertTable->cellAt(i+1,2).firstCursorPosition().insertText(s.nom_doc,textTableFormat);

            cursor=sertTable->cellAt(i+1,3).firstCursorPosition();
            cursor.setBlockFormat(formatCenter);
            cursor.setCharFormat(textTableFormat);
            insertDate(cursor,s.date_doc);
            i++;
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

    if (current_is_ship){
        cursor.setCharFormat(textBoldFormat);
        insertText(cursor,tr("Грузополучатель"),tr("Consignee"));
        cursor.insertText(":\n",textBoldFormat);
        cursor.setCharFormat(textNormalFormat);
        if (sertType!=1){
            insertText(cursor,data->head()->poluch.rus,data->head()->poluch.eng,true);
        }
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
    date=(current_is_ship)? data->head()->dateVidSert : QDate::currentDate();
    insertText(cursor,tr("Дата выдачи сертификата"),tr("Date of issue of the certificate"),false);
    cursor.insertText(tr(": "),textBoldFormat);
    cursor.setCharFormat(textNormalFormat);
    if (sertType!=1){
        insertDate(cursor,date,false);
    } else {
        insertDate(cursor,QDate(1111,11,11),false);
    }
    cursor.insertBlock();

    cursor.setBlockFormat(formatLeft);
    addResource(QTextDocument::ImageResource, QUrl("qrcode"), *data->qrCode());

    QTextImageFormat qrformat;
    qrformat.setName("qrcode");
    qrformat.setHeight(150);
    cursor.insertImage(qrformat);

    cursor.insertText("   ",textNormalFormat);
    QString nach=data->general()->otk_title.rus;
    QString nach_en=data->general()->otk_title.eng;
    QString line=tr(" ______________ ");
    if (sertType==2) { //подпись
        QImage im(data->general()->sign);
        QPainter p(&im);
        QFont f(textNormalFormat.font());
        f.setPixelSize(44);
        p.setFont(f);
        QFontMetrics fm(f);
        QString str, suf;
        if (l_en && !l_rus){
            suf=nach_en;
            str=data->general()->otk.eng;
        } else if (l_rus && !l_en){
            suf=nach;
            str=data->general()->otk.rus;
        } else if (l_rus && l_en){
            suf=nach+" / "+nach_en;
            str=data->general()->otk.rus+" / "+data->general()->otk.eng;
        }
        str=suf+line+str;
        int pos=(im.width()/2-fm.horizontalAdvance(suf));
        if (pos<0){
            pos=0;
        }
        p.drawText(pos,150,str);
        addResource(QTextDocument::ImageResource, QUrl("sign"), im);
        QTextImageFormat signformat;
        signformat.setName("sign");
        signformat.setHeight(150);
        cursor.insertImage(signformat);
    } else if (sertType==3 || sertType==4) { //транснефть, РТ-Техприемка
        cursor.insertBlock();
        cursor.setCharFormat(textNormalFormat);
        QString space="                                              ";
        if (!(l_en && l_rus)){
            cursor.insertText(space,textNormalFormat);
        }
        insertText(cursor,nach,nach_en);
        if (!(l_en && l_rus)){
            cursor.insertText("                                              ",textNormalFormat);
        } else {
            cursor.insertText("       ",textNormalFormat);
        }
        if (sertType==3){
            insertText(cursor,tr("Представитель ООО \"Транснефть Надзор\""),tr("Representative of Transneft Nadzor LLC"));
        } else {
            insertText(cursor,tr("Представитель АО \"РТ-Техприемка\""),tr("Representative of JSC \"RT-Techpriemka\""));
        }
        cursor.insertBlock();
        cursor.insertBlock();
        if (!(l_en && l_rus)){
            cursor.insertText(space,textNormalFormat);
        }
        cursor.insertText(line,textNormalFormat);
        insertText(cursor,data->general()->otk.rus,data->general()->otk.eng);
        if (!(l_en && l_rus)){
            cursor.insertText("                        ",textNormalFormat);
        } else {
            cursor.insertText("       ",textNormalFormat);
        }
        cursor.setCharFormat(textUnderlineFormat);
        if (sertType==3){
            insertText(cursor,tr("Ведущий инженер"),tr("Lead Lower"));
        } else {
            cursor.insertText("                                ",textUnderlineFormat);
            if (l_en && l_rus){
                cursor.insertText("                          ",textUnderlineFormat);
            }
        }
        cursor.insertText("  ",textNormalFormat);
        cursor.insertText("                                    ",textUnderlineFormat);
        cursor.insertText("  ",textNormalFormat);
        cursor.insertText("                                    ",textUnderlineFormat);
        cursor.insertBlock();
        if (!(l_en && l_rus)){
            cursor.insertText("                                                                                                                               ",textNormalFormat);
        } else {
            cursor.insertText("                                                                                                    ",textNormalFormat);
        }
        cursor.setCharFormat(textSmallFormat);
        cursor.insertText("(",textSmallFormat);
        insertText(cursor,tr("должность"),tr("job title"));
        if (l_en && !l_rus){
            cursor.insertText(")                        (",textSmallFormat);
        } else if (!(l_en && l_rus)){
            cursor.insertText(")                               (",textSmallFormat);
        } else {
            cursor.insertText(")                            (",textSmallFormat);
        }
        insertText(cursor,tr("подпись"),tr("signature"));
        if (!(l_en && l_rus)){
            cursor.insertText(")                               (",textSmallFormat);
        } else {
            cursor.insertText(")               (",textSmallFormat);
        }
        insertText(cursor,tr("ФИО"),tr("full name"));
        cursor.insertText(")",textSmallFormat);
    } else {
        cursor.setCharFormat(textNormalFormat);
        if (l_en && !l_rus){
            cursor.insertText("                               ",textNormalFormat);
        } else if (l_rus && !l_en){
            cursor.insertText("                                                    ",textNormalFormat);
        }
        insertText(cursor,nach,nach_en);
        cursor.insertText(line,textNormalFormat);
        if (sertType!=1){
            insertText(cursor,data->general()->otk.rus,data->general()->otk.eng);
        } else {
            insertText(cursor,tr("[МЕСТО ДЛЯ ПЕЧАТИ, ПОДПИСЬ]"),tr("[LOCUS SIGILLI, SIGNATURE]"));
        }
    }
}

void SertBuild::setType(int t)
{
    sertType=t;
    this->build(current_id,current_is_ship);
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
        c.insertText(lrus.toString(date,"dd.MM.yyyy"));
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

void SertBuild::setLRus(bool b)
{
    if (b){
        l_rus=true;
        l_en=false;
        this->rebuild();
    }
}

void SertBuild::setLEn(bool b)
{
    if (b){
        l_rus=false;
        l_en=true;
        this->rebuild();
    }
}

void SertBuild::setLMix(bool b)
{
    if (b){
        l_rus=true;
        l_en=true;
        this->rebuild();
    }
}

void SertBuild::setDocEn(int id_doc, bool en)
{
    data->setDocEn(id_doc,en);
    this->rebuild();
}

void SertBuild::setDefaultDoc()
{
    data->setDefaultDoc();
    this->build(current_id,current_is_ship);
}

DataSert::DataSert(QObject *parent) : QObject(parent)
{
}

void DataSert::refresh(int id, bool is_ship, bool sample)
{
    QSqlQuery query;
    QString sQuery;
    sQuery= is_ship ? QString("select p.id, p.n_s, p.yea, p.dat_part, e.marka_sert, p.diam, "
                              "gt.nam, pu.nam, coalesce (p.ibco, ev.znam), coalesce(pp.nam, pe.nam), pol.naim, s.nom_s, coalesce(s.dat_vid, :dt ), o.massa, pol.naim_en "
                              "from otpusk as o "
                              "inner join sertifikat as s on o.id_sert=s.id "
                              "inner join parti as p on o.id_part=p.id "
                              "inner join elrtr as e on e.id=p.id_el "
                              "inner join provol as pe on pe.id=e.id_gost "
                              "left join provol as pp on pp.id=p.id_prfact and pp.id in (select ep.id_prov from el_provol as ep where ep.id_el = p.id_el) "
                              "inner join poluch as pol on s.id_pol=pol.id "
                              "inner join gost_types as gt on e.id_gost_type=gt.id "
                              "inner join purpose as pu on e.id_purpose=pu.id "
                              "left join el_var as ev on ev.id_el = p.id_el and ev.id_var = p.id_var "
                              "where o.id = :id") :
                  QString("select p.id, p.n_s, p.yea, p.dat_part, e.marka_sert, p.diam, "
                          "gt.nam, pu.nam, coalesce (p.ibco, ev.znam), coalesce(pp.nam, pe.nam), NULL, NULL, (:dt)::date, j.sum, NULL "
                          "from parti as p "
                          "inner join elrtr as e on e.id=p.id_el "
                          "inner join provol as pe on pe.id=e.id_gost "
                          "left join provol as pp on pp.id=p.id_prfact and pp.id in (select ep.id_prov from el_provol as ep where ep.id_el = p.id_el) "
                          "inner join gost_types as gt on e.id_gost_type=gt.id "
                          "inner join purpose as pu on e.id_purpose=pu.id "
                          "left join (select id_part as id, sum(kvo) as sum from part_prod group by id_part) as j on j.id=p.id "
                          "left join el_var as ev on ev.id_el = p.id_el and ev.id_var = p.id_var "
                          "where p.id = :id");
    query.prepare(sQuery);
    query.bindValue(":id",id);
    query.bindValue(":dt",QDate::currentDate());
    if (query.exec()){
        while(query.next()){
            hData.id_parti=query.value(0).toInt();
            hData.nomPart=sample ? "1111" : query.value(1).toString();
            hData.yearPart=query.value(2).toInt();
            hData.datePart=sample ? QDate(1111,11,11) : query.value(3).toDate();
            hData.marka=query.value(4).toString();
            hData.diam=query.value(5).toDouble();
            hData.tip1=query.value(6).toString();
            hData.tip2=query.value(7).toString();
            hData.znam=query.value(8).toString();
            hData.prov=query.value(9).toString();
            hData.poluch.rus=query.value(10).toString();
            hData.nomSert=query.value(11).toString();
            hData.dateVidSert=query.value(12).toDate();
            hData.netto=sample ? 1111 : query.value(13).toDouble();
            hData.poluch.eng=query.value(14).toString();
        }
        refreshGeneralData(hData.dateVidSert);
        refreshTu();
        refreshChem();
        refreshMech();
        refreshSert();
        refreshQR(id,is_ship, sample);
        emit sigRefresh();
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}

const generalData *DataSert::general()
{
    return &gData;
}

const headData *DataSert::head()
{
    return &hData;
}

void DataSert::refreshTu()
{
    QSqlQuery tuQuery;
    tuQuery.prepare("select nam "
                    "from zvd_get_tu_var((select dat_part from parti where id = :id1 ), "
                    "(select id_el from parti where id = :id2 ), "
                    "(select d.id from diam as d where d.diam = (select diam from parti where id = :id3 )), "
                    "(select id_var from parti where id = :id4 ) ) ");
    tuQuery.bindValue(":id1",hData.id_parti);
    tuQuery.bindValue(":id2",hData.id_parti);
    tuQuery.bindValue(":id3",hData.id_parti);
    tuQuery.bindValue(":id4",hData.id_parti);
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
    QSqlQuery query;
    query.prepare("select c.sig, s.value from sert_chem as s "
                  "inner join chem_tbl as c on s.id_chem=c.id "
                  "where s.id_part= :id_part order by c.sig");
    query.bindValue(":id_part",hData.id_parti);
    cData.clear();
    if (query.exec()){
        while (query.next()) {
            chemData c;
            c.name=query.value(0).toString();
            c.value=query.value(1);
            cData.push_back(c);
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Ok);
    }
}

void DataSert::refreshMech()
{
    QSqlQuery query;
    query.prepare("select m.id_cat, m.nam_html, m.sig_html, m.prefix, s.value, m.nam_html_en, m.sig_html_en, m.prefix_en "
                  "from sert_mech as s "
                  "inner join mech_tbl as m on s.id_mech=m.id "
                  "where s.id_part = :id_part1 "
                  "union "
                  "select t.id_cat, t.nam, null, n.nam, null, t.nam_en, null, n.nam_en from sert_mechx as x "
                  "inner join mechx_tbl as t on x.id_mechx=t.id "
                  "inner join mechx_nams as n on x.id_value=n.id "
                  "where x.id_part = :id_part2 order by 2");
    query.bindValue(":id_part1",hData.id_parti);
    query.bindValue(":id_part2",hData.id_parti);
    mData.clear();
    if (query.exec()){
        while (query.next()){
            mechData m;
            m.id_cat=query.value(0).toInt();
            m.nam_html.rus=query.value(1).toString();
            m.sig_htlm.rus=query.value(2).toString();
            m.prefix.rus=query.value(3).toString();
            m.value=query.value(4);
            m.nam_html.eng=query.value(5).toString();
            m.sig_htlm.eng=query.value(6).toString();
            m.prefix.eng=query.value(7).toString();
            mData.push_back(m);
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Ok);
    }
}

void DataSert::refreshSert()
{
    QSqlQuery query;
    query.prepare("select z.id_ved, z.doc_nam, z.ved_nam, z.nom_doc, z.dat_doc, z.gr_tech_ust, z.id_doc_t, z.ved_short, z.grade_nam, z.ved_short_en, z.doc_nam_en, z.ved_nam_en, z.id_doc, z.en "
                  "from zvd_get_sert_var((select dat_part from parti where id = :id_part1 ), "
                  "(select id_el from parti where id = :id_part2 ), "
                  "(select d.id from diam as d where d.diam = (select diam from parti where id = :id_part3 )), "
                  "(select id_var from parti where id = :id_part4 ) ) as z");
    query.bindValue(":id_part1",hData.id_parti);
    query.bindValue(":id_part2",hData.id_parti);
    query.bindValue(":id_part3",hData.id_parti);
    query.bindValue(":id_part4",hData.id_parti);
    sData.clear();
    if (query.exec()){
        while (query.next()){
            sertData s;
            s.id_ved=query.value(0).toInt();
            s.doc_nam.rus=query.value(1).toString();
            s.ved_nam.rus=query.value(2).toString();
            s.nom_doc=query.value(3).toString();
            s.date_doc=query.value(4).toDate();
            s.gr_tech_ust=query.value(5).toString();
            s.id_doc_t=query.value(6).toInt();
            s.ved_short.rus=query.value(7).toString();
            s.grade_nam=query.value(8).toString();
            s.ved_short.eng=query.value(9).toString();
            s.doc_nam.eng=query.value(10).toString();
            s.ved_nam.eng=query.value(11).toString();
            s.id_doc=query.value(12).toInt();
            s.en=mapSert.value(s.id_doc,query.value(13).toBool());
            sData.push_back(s);
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Ok);
    }
}

void DataSert::refreshQR(int id, bool is_ship, bool sample)
{
    QString str;
    str+=sample? "ОБРАЗЕЦ СЕРТИФИКАТА КАЧЕСТВА №" : "СЕРТИФИКАТ КАЧЕСТВА №";
    str+=hData.nomPart+"-"+QString::number(hData.yearPart);
    if (is_ship){
        str+="/"+hData.nomSert;
    }
    str+="\n";
    str+="Марка ";
    str+=hData.marka;
    str+="\n";
    str+="Диаметр, мм ";
    str+=QString::number(hData.diam);
    str+="\n";
    str+="Номер партии ";
    str+=hData.nomPart;
    str+="\n";
    str+="Дата производства ";
    str+=hData.datePart.toString("dd.MM.yy");
    str+="\n";
    str+="Масса нетто, кг ";
    str+=QString::number(hData.netto);
    str+="\n";
    if (is_ship && !sample){
        str+="Грузополучатель: "+hData.poluch.rus+"\n";
    }
    QDate date=QDate::currentDate();
    if (is_ship && !sample){
        date=hData.dateVidSert;
    } else if (sample){
        date=QDate(1111,11,11);
    }
    str+="Дата "+date.toString("dd.MM.yy")+"\n";
    quint32 id_part=hData.id_parti;
    quint32 id_ship= is_ship ? id : 0;
    quint64 cod=0;

    memcpy((quint8*)&cod,(quint8*)&id_ship,4);
    memcpy((quint8*)&cod+4,(quint8*)&id_part,4);
    //qDebug()<<id_ship<<id_part<<cod;

    str+="Код подлинности "+QString::number(cod);

    qr_code=qrCode(str);
}

const cvData *DataSert::chem()
{
    return &cData;
}

const mvData *DataSert::mech()
{
    return &mData;
}

const svData *DataSert::sert()
{
    return &sData;
}

const QImage *DataSert::qrCode()
{
    return &qr_code;
}

const QImage DataSert::qrCode(QString str)
{
    QrEncode qr;
    bool ok=qr.encodeData(0,0,true,-1,str.toUtf8().data());
    const int scale=10;
    int s=1;
    if (ok) s=(qr.size()>0)? qr.size() : 1;
    QImage img(s*scale,s*scale,QImage::Format_RGB32);
    QPainter painter(&img);
    if(ok){
        QColor fg("black");
        QColor bg("white");
        painter.setBrush(bg);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0,0,s*scale,s*scale);
        painter.setBrush(fg);
        for(int y=0; y<s; y++){
            for(int x=0; x<s; x++){
                if(qr.data(y,x)){
                    const double rx1=x*scale, ry1=y*scale;
                    QRectF r(rx1, ry1, scale, scale);
                    painter.drawRects(&r,1);
                }
            }
        }
    } else {
        QColor error("red");
        painter.setBrush(error);
        painter.drawRect(0,0,scale-1,scale-1);
    }
    return img;
}

QString DataSert::tu()
{
    QString str;
    QStringList::const_iterator constIterator;
    for (constIterator = tuList.constBegin(); constIterator != tuList.constEnd();++constIterator){
        if (!str.isEmpty()){
            str+=", ";
        }
        str+=(*constIterator);
    }
    return str;
}

sLang DataSert::mechCategory(int id)
{
    return mechCat.value(id);
}

void DataSert::setDocEn(int id_doc, bool en)
{
    for (int i=0; i<sData.size(); i++){
        if (sData[i].id_doc==id_doc){
            sData[i].en=en;
        }
    }
    mapSert[id_doc]=en;
}

void DataSert::setDefaultDoc()
{
    mapSert.clear();
}

void DataSert::refreshMechCategory()
{
    QSqlQuery query;
    query.prepare("select id, nam, nam_en from mech_category order by id");
    if (query.exec()){
        mechCat.clear();
        while (query.next()){
            int id=query.value(0).toInt();
            sLang nam;
            nam.rus=query.value(1).toString();
            nam.eng=query.value(2).toString();
            mechCat.insert(id,nam);
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Ok);
    }
}

void DataSert::refreshGeneralData(QDate date)
{
    refreshMechCategory();
    QSqlQuery query;
    query.prepare("select adr, telboss||', '||telfax||', '||teldop||' '||site||' '||email, otk, adr_en, otk_en, otk_title, otk_title_en, sign "
                  "from general_data where dat = (select max(mgd.dat) from general_data mgd where mgd.dat <= :dat )");
    query.bindValue(":dat",date);
    if (query.exec()){
        while(query.next()){
            gData.adres.rus=query.value(0).toString();
            gData.contact=query.value(1).toString();
            gData.otk.rus=query.value(2).toString();
            gData.adres.eng=query.value(3).toString();
            gData.otk.eng=query.value(4).toString();
            gData.otk_title.rus=query.value(5).toString();
            gData.otk_title.eng=query.value(6).toString();
            gData.sign.loadFromData(query.value(7).toByteArray());
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
    gData.logo.load("images/logo2.png");
}

