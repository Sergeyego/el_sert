#include "editor.h"
#include "ui_editor.h"
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QDir>
#include <QAbstractTextDocumentLayout>
#include <QUrl>
#include <QPainter>
#include <QDebug>
#include <QFileDialog>

Editor::Editor(QTextDocument *doc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);

    QString rsrcPath=":/icons";

    const QIcon printIcon = QIcon::fromTheme("document-print", QIcon(rsrcPath + "/fileprint.png"));
    ui->cmd_print->setIcon(printIcon);

    const QIcon exportPdfIcon = QIcon::fromTheme("exportpdf", QIcon(rsrcPath + "/exportpdf.png"));
    ui->cmd_pdf->setIcon(exportPdfIcon);

    toolBar = new QToolBar(this);
    toolBarFont = new QToolBar(this);

    setupTextActions();

    ui->verticalLayoutTool->addWidget(toolBar);
    ui->verticalLayoutTool->addWidget(toolBarFont);

    printer = new QPrinter();
    printer->setPageMargins(QMarginsF(30, 30, 30, 30));
    printer->setColorMode(QPrinter::Color);
    printer->setPageOrientation(QPageLayout::Portrait);
    printer->setPageSize(QPageSize(QPageSize::A4));

    colVal t;
    t.val=0;
    if (!Rels::instance()->relSertType->isInital()){
        Rels::instance()->relSertType->refreshModel();
    }
    ui->comboBoxType->setModel(Rels::instance()->relSertType->model());
    ui->comboBoxType->setEditable(false);
    ui->comboBoxType->setCurrentData(t);

    ui->textEdit->setDocument(doc);
    SertBuild *s=qobject_cast<SertBuild *>(doc);
    if (s){
        ui->comboBoxType->setCurrentIndex(s->getType());
        connect(ui->comboBoxType,SIGNAL(currentIndexChanged(int)),this,SLOT(setType()));
        connect(ui->radioButtonRus,SIGNAL(clicked(bool)),this,SLOT(setLang()));
        connect(ui->radioButtonEn,SIGNAL(clicked(bool)),this,SLOT(setLang()));
        connect(ui->radioButtonMix,SIGNAL(clicked(bool)),this,SLOT(setLang()));
        connect(s,SIGNAL(sigRefresh()),this,SLOT(chDoc()));
    }

    connect(ui->textEdit->document(), SIGNAL(undoAvailable(bool)), actionUndo, SLOT(setEnabled(bool)));
    connect(ui->textEdit->document(), SIGNAL(redoAvailable(bool)), actionRedo, SLOT(setEnabled(bool)));

    connect(ui->textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(ui->textEdit, SIGNAL(cursorPositionChanged()),this,SLOT(cursorPositionChanged()));

    connect(ui->cmd_print,SIGNAL(clicked()),this,SLOT(filePrint()));
    connect(ui->cmd_pdf,SIGNAL(clicked()),this,SLOT(exportPdf()));
    connect(ui->comboBoxType,SIGNAL(currentIndexChanged(int)),this,SLOT(setObr()));
    connect(ui->radioButtonRus,SIGNAL(clicked(bool)),this,SLOT(setObr()));
    connect(ui->radioButtonEn,SIGNAL(clicked(bool)),this,SLOT(setObr()));
    connect(ui->radioButtonMix,SIGNAL(clicked(bool)),this,SLOT(setObr()));
    connect(ui->pushButtonSertDef,SIGNAL(clicked(bool)),this,SLOT(setDefaultDoc()));
    connect(ui->pushButtonHtml,SIGNAL(clicked(bool)),this,SLOT(exportHtml()));
    connect(ui->toolButtonLoad,SIGNAL(clicked(bool)),this,SLOT(loadHtml()));
    connect(ui->pushButtonDS,SIGNAL(clicked(bool)),this,SLOT(signDS()));
}

QTextDocument *Editor::document()
{
    return ui->textEdit->document();
}

Editor::~Editor()
{
    delete ui;
    delete printer;
}

void Editor::setupTextActions()
{
    QString rsrcPath=":/icons";
    const QIcon boldIcon = QIcon::fromTheme("format-text-bold", QIcon(rsrcPath + "/textbold.png"));
    actionTextBold=toolBar->addAction(boldIcon,QString::fromUtf8("Жирный"),this,SLOT(textBold()));
    actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    actionTextBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    actionTextBold->setCheckable(true);

    const QIcon italicIcon = QIcon::fromTheme("format-text-italic", QIcon(rsrcPath + "/textitalic.png"));
    actionTextItalic = toolBar->addAction(italicIcon, QString::fromUtf8("Курсив"), this,SLOT(textItalic()));
    actionTextItalic->setPriority(QAction::LowPriority);
    actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    actionTextItalic->setCheckable(true);

    const QIcon underlineIcon = QIcon::fromTheme("format-text-underline", QIcon(rsrcPath + "/textunder.png"));
    actionTextUnderline = toolBar->addAction(underlineIcon, QString::fromUtf8("Подчеркнутый"), this,SLOT(textUnderline()));
    actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    actionTextUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    actionTextUnderline->setCheckable(true);

    toolBar->addSeparator();

    const QIcon leftIcon = QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png"));
    actionAlignLeft = new QAction(leftIcon, tr("Выровнять влево"), this);
    actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
    actionAlignLeft->setCheckable(true);
    actionAlignLeft->setPriority(QAction::LowPriority);
    const QIcon centerIcon = QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png"));
    actionAlignCenter = new QAction(centerIcon, tr("Центрировать по горизонтали"), this);
    actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
    actionAlignCenter->setCheckable(true);
    actionAlignCenter->setPriority(QAction::LowPriority);
    const QIcon rightIcon = QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png"));
    actionAlignRight = new QAction(rightIcon, tr("Выровнять вправо"), this);
    actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
    actionAlignRight->setCheckable(true);
    actionAlignRight->setPriority(QAction::LowPriority);
    const QIcon fillIcon = QIcon::fromTheme("format-justify-fill", QIcon(rsrcPath + "/textjustify.png"));
    actionAlignJustify = new QAction(fillIcon, tr("По ширине"), this);
    actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
    actionAlignJustify->setCheckable(true);
    actionAlignJustify->setPriority(QAction::LowPriority);

    QActionGroup *alignGroup = new QActionGroup(this);
    connect(alignGroup, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));

    if (QApplication::isLeftToRight()) {
        alignGroup->addAction(actionAlignLeft);
        alignGroup->addAction(actionAlignCenter);
        alignGroup->addAction(actionAlignRight);
    } else {
        alignGroup->addAction(actionAlignRight);
        alignGroup->addAction(actionAlignCenter);
        alignGroup->addAction(actionAlignLeft);
    }
    alignGroup->addAction(actionAlignJustify);

    toolBar->addActions(alignGroup->actions());

    toolBar->addSeparator();

    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    actionTextColor = toolBar->addAction(pix, tr("Цвет шрифта"), this, SLOT(textColor()));

    comboStyle = new QComboBox(toolBar);
    toolBar->addWidget(comboStyle);
    comboStyle->addItem("Стандартный текст");
    comboStyle->addItem("Список маркеров (Диск)");
    comboStyle->addItem("Список маркеров (Круг)");
    comboStyle->addItem("Список маркеров (Квадрат)");
    comboStyle->addItem("Упорядоченный список (Десятичный)");
    comboStyle->addItem("Упорядоченный список (Строчные буквы)");
    comboStyle->addItem("Упорядоченный список (Заглавные буквы)");
    comboStyle->addItem("Упорядоченный список (Римские строчные)");
    comboStyle->addItem("Упорядоченный список (Римские заглавные)");
    connect(comboStyle,SIGNAL(activated(int)),this,SLOT(textStyle(int)));

    comboFont = new QFontComboBox(toolBarFont);
    toolBarFont->addWidget(comboFont);
    connect(comboFont,SIGNAL(activated(QString)),this,SLOT(textFamily(QString)));

    comboSize = new QComboBox(toolBarFont);
    comboSize->setObjectName("comboSize");
    toolBarFont->addWidget(comboSize);
    comboSize->setEditable(true);

    const QList<int> standardSizes = QFontDatabase::standardSizes();
    foreach (int size, standardSizes)
        comboSize->addItem(QString::number(size));
    comboSize->setCurrentIndex(standardSizes.indexOf(QApplication::font().pointSize()));
    connect(comboSize,SIGNAL(activated(QString)),this,SLOT(textSize(QString)));

    toolBarFont->addSeparator();

    const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(rsrcPath + "/editundo.png"));
    actionUndo = toolBarFont->addAction(undoIcon, tr("Отменить"), ui->textEdit, SLOT(undo()));
    actionUndo->setShortcut(QKeySequence::Undo);

    const QIcon redoIcon = QIcon::fromTheme("edit-redo", QIcon(rsrcPath + "/editredo.png"));
    actionRedo = toolBarFont->addAction(redoIcon, tr("Вернуть"), ui->textEdit, SLOT(redo()));
    actionRedo->setPriority(QAction::LowPriority);
    actionRedo->setShortcut(QKeySequence::Redo);
    actionRedo->setEnabled(false);

    toolBarFont->addSeparator();

    const QIcon printIcon = QIcon::fromTheme("document-print", QIcon(rsrcPath + "/fileprint.png"));
    QAction *a = toolBarFont->addAction(printIcon, tr("Печать"), this, SLOT(filePrint()));
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Print);

    const QIcon filePrintIcon = QIcon::fromTheme("fileprint", QIcon(rsrcPath + "/fileprint.png"));
    toolBarFont->addAction(filePrintIcon, tr("Просмотр печати"), this, SLOT(filePrintPreview()));

    const QIcon exportPdfIcon = QIcon::fromTheme("exportpdf", QIcon(rsrcPath + "/exportpdf.png"));
    a = toolBarFont->addAction(exportPdfIcon, tr("Экспорт PDF..."), this, SLOT(exportPdf()));
    a->setPriority(QAction::LowPriority);
    a->setShortcut(Qt::CTRL + Qt::Key_D);
}

void Editor::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    ui->textEdit->mergeCurrentCharFormat(format);
}

QString Editor::imBackPath()
{
    QString p;
    if (ui->radioButtonRus->isChecked() && !ui->radioButtonEn->isChecked()){
        p=":images/bgr.png";
    } else if (!ui->radioButtonRus->isChecked() && ui->radioButtonEn->isChecked()){
        p=":images/bge.png";
    } else {
        p=":images/bgm.png";
    }
    return p;
}

void Editor::print(QPrinter *p)
{
    QPainter painter(p);
    drawDoc(&painter);
}

void Editor::drawDoc(QPainter *painter)
{
    QTextDocument *doc=ui->textEdit->document();
    QRect rect = painter->viewport();
    QSize size (rect.size());
    size.scale(doc->size().toSize(),Qt::KeepAspectRatioByExpanding);
    painter->setWindow(0,0,size.width(),size.height());
    if (ui->comboBoxType->currentIndex()==1){
        painter->drawImage(painter->window(),QImage(imBackPath()));
    }
    doc->drawContents(painter);
}

void Editor::exportHtml()
{
    SertBuild *doc = qobject_cast<SertBuild *>(ui->textEdit->document());
    QString exportname, fname;
    fname= doc? (doc->getName()) : QString("sertificat");
    if (doc){
        QSettings settings("szsm", QApplication::applicationName());
        QDir dir(settings.value("sertPath",QDir::homePath()).toString());
        exportname = QFileDialog::getSaveFileName(this,tr("Сохранить HTML"),dir.path()+"/"+fname+".html", "*.html");
        if (!exportname.isEmpty()) {
            QFile file(exportname);
            if (file.open(QIODevice::WriteOnly)){
                file.write(doc->toHtml().toUtf8());
                file.close();
            }
        }
    }
}

void Editor::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void Editor::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(actionTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void Editor::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(actionTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void Editor::textAlign(QAction *a)
{
    if (a == actionAlignLeft)
        ui->textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == actionAlignCenter)
        ui->textEdit->setAlignment(Qt::AlignHCenter);
    else if (a == actionAlignRight)
        ui->textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == actionAlignJustify)
        ui->textEdit->setAlignment(Qt::AlignJustify);
}

void Editor::textColor()
{
    QColor col = QColorDialog::getColor(ui->textEdit->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void Editor::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    actionTextColor->setIcon(pix);
}

void Editor::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
}

void Editor::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft)
        actionAlignLeft->setChecked(true);
    else if (a & Qt::AlignHCenter)
        actionAlignCenter->setChecked(true);
    else if (a & Qt::AlignRight)
        actionAlignRight->setChecked(true);
    else if (a & Qt::AlignJustify)
        actionAlignJustify->setChecked(true);
}

void Editor::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void Editor::cursorPositionChanged()
{
    alignmentChanged(ui->textEdit->alignment());
}

void Editor::textStyle(int styleIndex)
{
    QTextCursor cursor = ui->textEdit->textCursor();

        if (styleIndex != 0) {
            QTextListFormat::Style style = QTextListFormat::ListDisc;

            switch (styleIndex) {
                default:
                case 1:
                    style = QTextListFormat::ListDisc;
                    break;
                case 2:
                    style = QTextListFormat::ListCircle;
                    break;
                case 3:
                    style = QTextListFormat::ListSquare;
                    break;
                case 4:
                    style = QTextListFormat::ListDecimal;
                    break;
                case 5:
                    style = QTextListFormat::ListLowerAlpha;
                    break;
                case 6:
                    style = QTextListFormat::ListUpperAlpha;
                    break;
                case 7:
                    style = QTextListFormat::ListLowerRoman;
                    break;
                case 8:
                    style = QTextListFormat::ListUpperRoman;
                    break;
            }

            cursor.beginEditBlock();

            QTextBlockFormat blockFmt = cursor.blockFormat();

            QTextListFormat listFmt;

            if (cursor.currentList()) {
                listFmt = cursor.currentList()->format();
            } else {
                listFmt.setIndent(blockFmt.indent() + 1);
                blockFmt.setIndent(0);
                cursor.setBlockFormat(blockFmt);
            }

            listFmt.setStyle(style);

            cursor.createList(listFmt);

            cursor.endEditBlock();
        } else {
            QTextBlockFormat bfmt;
            bfmt.setObjectIndex(-1);
            cursor.mergeBlockFormat(bfmt);
        }
}

void Editor::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void Editor::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void Editor::setObr()
{
    if (ui->comboBoxType->currentIndex()==1){
        ui->textEdit->viewport()->setStyleSheet(QString("background-image: url(%1);").arg(imBackPath()));
    } else {
        ui->textEdit->viewport()->setStyleSheet(QString(""));
    }
}

void Editor::chDoc()
{
    SertBuild *s=qobject_cast<SertBuild *>(this->document());
    if (s){
        foreach (QCheckBox *b, boxes) {
            b->deleteLater();
        }
        boxes.clear();
        for (sertData d : s->getSData()){
            QCheckBox *box = new QCheckBox(d.ved_short+": "+d.nom_doc);
            connect(box,SIGNAL(clicked(bool)),this,SLOT(setEnDoc(bool)));
            box->setChecked(d.en);
            box->setProperty("id",d.id_doc);
            boxes.push_back(box);
            ui->verticalLayoutBox->addWidget(box);
        }
    }
}

void Editor::setEnDoc(bool en)
{
    QCheckBox *box=qobject_cast<QCheckBox *>(sender());
    if (box){
        SertBuild *s=qobject_cast<SertBuild *>(this->document());
        if (s){
            s->setDocEn(box->property("id").toInt(),en);
        }
    }
}

void Editor::setDefaultDoc()
{
    SertBuild *s=qobject_cast<SertBuild *>(this->document());
    if (s){
        s->setDefaultDoc();
    }
}

void Editor::loadHtml()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open html"),QDir::homePath(), tr("HTML Files (*.html)"));
    if (!fileName.isEmpty()){
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString html=file.readAll();
            this->document()->setHtml(html);
            file.close();
        }
    }
}

void Editor::setLang()
{
    QString lang="mix";
    if (ui->radioButtonRus->isChecked()){
        lang="ru";
    } else if (ui->radioButtonEn->isChecked()){
        lang="en";
    }
    SertBuild *s=qobject_cast<SertBuild *>(this->document());
    if (s){
        s->setLang(lang);
    }
}

void Editor::setType()
{
    int type=ui->comboBoxType->getCurrentData().val.toInt();
    SertBuild *s=qobject_cast<SertBuild *>(this->document());
    if (s){
        s->setType(type);
    }
    ui->pushButtonDS->setEnabled(type==6);
}

void Editor::signDS()
{
    DialogSignature d;
    if (d.exec()==QDialog::Accepted){
        QString filename="tmpUnsignedCert.pdf";
        exportPdf(filename);
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly)){
            QByteArray data;
            bool ok = HttpSyncManager::sendRequest("/pdf/"+d.getSN(),"POST",file.readAll(),data);
            if (ok){
                QFile file("temp.pdf");
                if (file.open(QIODevice::WriteOnly)){
                    file.write(data);
                    file.close();
                    QFileInfo fileInfo(file);
                    QDesktopServices::openUrl((QUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()))));
                }
            }
            file.close();
        }
    }
}

void Editor::filePrint()
{
    QPrintDialog printDialog(printer, this);
    if (printDialog.exec()) {
        print(printer);
    }
}

void Editor::filePrintPreview()
{
    QPrintPreviewDialog preview(printer, this);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)), this, SLOT(print(QPrinter*)));
    preview.exec();
}

void Editor::exportPdf()
{
    SertBuild *doc = qobject_cast<SertBuild *>(ui->textEdit->document());
    QString exportname, fname;
    fname= doc? (doc->getName()) : QString("sertificat");
    QSettings settings("szsm", QApplication::applicationName());
    QDir dir(settings.value("sertPath",QDir::homePath()).toString());
    exportname = QFileDialog::getSaveFileName(this,tr("Сохранить PDF"),dir.path()+"/"+fname+".pdf", "*.pdf");
    if (!exportname.isEmpty()) {
        exportPdf(exportname);
        QFile file(exportname);
        QFileInfo info(file);
        settings.setValue("sertPath",info.path());
    }
}

void Editor::exportPdf(QString filename)
{
    if (QFileInfo(filename).suffix().isEmpty()){
        filename.append(".pdf");
    }
    QPrinter p;
    p.setOutputFormat(QPrinter::PdfFormat);
    p.setOutputFileName(filename);
    p.setColorMode(QPrinter::Color);
    p.setPageMargins(QMarginsF(30, 30, 30, 30));
    p.setResolution(QPrinter::HighResolution);
    print(&p);
}

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent)
{
    actionAddRow = new QAction(tr("Добавить строки"),this);
    actionAddCol = new QAction(tr("Добавить столбцы"),this);
    actionDelRow = new QAction(tr("Удалить строки"),this);
    actionDelCol = new QAction(tr("Удалить столбцы"),this);
    actionMerge = new QAction(tr("Объединить ячейки"),this);
    actionUnMerge = new QAction(tr("Разбить ячейки"),this);
    actionAddTable = new QAction(tr("Вставить таблицу"),this);

    actionAddTable->setShortcut(Qt::CTRL + Qt::Key_F12);
    actionAddTable->setPriority(QAction::LowPriority);

    connect(actionAddRow,SIGNAL(triggered(bool)),this,SLOT(addRow()));
    connect(actionAddCol,SIGNAL(triggered(bool)),this,SLOT(addCol()));
    connect(actionDelRow,SIGNAL(triggered(bool)),this,SLOT(delRow()));
    connect(actionDelCol,SIGNAL(triggered(bool)),this,SLOT(delCol()));
    connect(actionMerge,SIGNAL(triggered(bool)),this,SLOT(mergeCells()));
    connect(actionUnMerge,SIGNAL(triggered(bool)),this,SLOT(unmergeCells()));
    connect(actionAddTable,SIGNAL(triggered(bool)),this,SLOT(addTable()));
}

void TextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *menu = createStandardContextMenu();
    QTextCursor cursor=this->textCursor();
    QTextTable *table=cursor.currentTable();
    if (table){
        QTextTableCell cell=table->cellAt(cursor);
        int firstRow, numRow, firstCol, numCol;
        cursor.selectedTableCells(&firstRow,&numRow,&firstCol,&numCol);
        QMenu *tableMenu = new QMenu(tr("Таблица"),menu);
        tableMenu->addAction(actionAddRow);
        tableMenu->addAction(actionAddCol);
        tableMenu->addSeparator();
        tableMenu->addAction(actionDelRow);
        tableMenu->addAction(actionDelCol);
        tableMenu->addSeparator();
        if (numRow>0 || numCol>0){
            tableMenu->addAction(actionMerge);
        }
        if (cell.isValid()){
            QTextCharFormat fmt = cell.format();
            const int rowSpan = fmt.tableCellRowSpan();
            const int colSpan = fmt.tableCellColumnSpan();
            if (rowSpan>1 || colSpan>1){
                tableMenu->addAction(actionUnMerge);
            }
        }
        menu->addMenu(tableMenu);
    } else {
        menu->addAction(actionAddTable);
    }
    menu->exec(e->globalPos());
    delete menu;
}

void TextEdit::paintEvent(QPaintEvent *e)
{
    QTextEdit::paintEvent(e);
}

void TextEdit::addRow()
{
    QTextCursor cursor=this->textCursor();
    QTextTable *table=cursor.currentTable();
    if (table){
        bool ok;
        int n= QInputDialog::getInt(this,tr("Введите число строк"),tr("Введите число строк"),1,0,100,1,&ok);
        if (ok) {
            table->insertRows(table->cellAt(cursor).row()+1,n);
        }
    }
}

void TextEdit::addCol()
{
    QTextCursor cursor=this->textCursor();
    QTextTable *table=cursor.currentTable();
    if (table){
        bool ok;
        int n= QInputDialog::getInt(this,tr("Введите число столбцов"),tr("Введите число столбцов"),1,0,100,1,&ok);
        if (ok) {
            table->insertColumns(table->cellAt(cursor).column()+1,n);
        }
    }
}

void TextEdit::delRow()
{
    QTextCursor cursor=this->textCursor();
    QTextTable *table=cursor.currentTable();
    if (table){
        int firstRow, numRow, firstCol, numCol;
        cursor.selectedTableCells(&firstRow,&numRow,&firstCol,&numCol);
        if (firstRow>=0 && numRow>=0){
            table->removeRows(firstRow,numRow);
        } else {
            table->removeRows(table->cellAt(cursor).row(),1);
        }
    }
}

void TextEdit::delCol()
{
    QTextCursor cursor=this->textCursor();
    QTextTable *table=cursor.currentTable();
    if (table){
        int firstRow, numRow, firstCol, numCol;
        cursor.selectedTableCells(&firstRow,&numRow,&firstCol,&numCol);
        if (firstCol>=0 && numCol>=0){
            table->removeColumns(firstCol,numCol);
        } else {
            table->removeColumns(table->cellAt(cursor).column(),1);
        }
    }
}

void TextEdit::mergeCells()
{
    QTextCursor cursor=this->textCursor();
    QTextTable *table=cursor.currentTable();
    if (table){
        table->mergeCells(cursor);
    }
}

void TextEdit::unmergeCells()
{
    QTextCursor cursor=this->textCursor();
    QTextTable *table=cursor.currentTable();
    if (table){
        table->splitCell(table->cellAt(cursor).row(), table->cellAt(cursor).column(), 1, 1);
    }
}

void TextEdit::addTable()
{
    QTextCursor cursor=this->textCursor();
    QTextTable *table=cursor.currentTable();
    if (!table){
        bool ok;
        int n= QInputDialog::getInt(this,tr("Введите число столбцов"),tr("Введите число столбцов"),2,0,100,1,&ok);
        if (ok) {
            QTextTableFormat tableFormat;
            tableFormat.setAlignment(Qt::AlignHCenter);
            QBrush brush = tableFormat.borderBrush();
            brush.setColor(QColor(Qt::black));
            tableFormat.setBorderBrush(brush);
            tableFormat.setCellPadding(3);
            tableFormat.setCellSpacing(1);
            cursor.insertTable(2,n,tableFormat);
        }
    }
}
