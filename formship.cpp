#include "formship.h"
#include "ui_formship.h"

FormShip::FormShip(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormShip)
{
    ui->setupUi(this);
    loadSettings();

    QString rsrcPath=":/icons";
    const QIcon exportPdfIcon = QIcon::fromTheme("exportpdf", QIcon(rsrcPath + "/exportpdf.png"));
    ui->cmdMultipagePdf->setIcon(exportPdfIcon);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(rsrcPath + "/filesave.png"));
    ui->cmdSaveAll->setIcon(saveIcon);

    const QIcon printIcon = QIcon::fromTheme("document-print", QIcon(rsrcPath + "/fileprint.png"));
    ui->cmdPrintAll->setIcon(printIcon);

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(ui->dateEditBeg->date().addYears(1));

    sertificat = new SertBuild(this);
    editor = new Editor(sertificat,this);
    ui->tab_1->layout()->addWidget(editor);

    reader = new Reader(this);
    ui->tab_2->layout()->addWidget(reader);

    ui->tableViewShip->verticalHeader()->setDefaultSectionSize(ui->tableViewShip->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewShip->verticalHeader()->hide();
    ui->tableViewShipData->verticalHeader()->setDefaultSectionSize(ui->tableViewShipData->verticalHeader()->fontMetrics().height()*1.5);
    modelShip = new ModelShip(this);
    ui->tableViewShip->setModel(modelShip);
    modelDataShip = new ModelDataShip(this);
    ui->tableViewShipData->setModel(modelDataShip);

    connect(ui->tableViewShip->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(refreshDataShip(QModelIndex)));
    connect(ui->tableViewShipData->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(refreshShipSert(QModelIndex)));
    connect(ui->tableViewShipData,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(partReq(QModelIndex)));

    connect(ui->cmdPrintAll,SIGNAL(clicked()),this,SLOT(printAll()));
    connect(ui->cmdSaveAll,SIGNAL(clicked()),this,SLOT(pdfAll()));
    connect(ui->cmdUpd,SIGNAL(clicked()),this,SLOT(refresh()));
    connect(ui->cmdMultipagePdf,SIGNAL(clicked(bool)),this,SLOT(multipagePdf()));

    connect(editor,SIGNAL(signFinished()),this,SLOT(signFinished()));

    refresh();
}

FormShip::~FormShip()
{
    saveSettings();
    delete ui;
}

void FormShip::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->ui->splitterVert->restoreState(settings.value("main_splitter_width").toByteArray());
}

void FormShip::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("main_splitter_width",ui->splitterVert->saveState());
}

void FormShip::refreshDataShip(QModelIndex index)
{
    int id_ship=modelShip->data(modelShip->index(index.row(),0),Qt::EditRole).toInt();
    modelDataShip->refresh(id_ship);
    ui->tableViewShipData->setColumnHidden(0,true);
    ui->tableViewShipData->setColumnHidden(5,true);
    ui->tableViewShipData->setColumnHidden(6,true);
    ui->tableViewShipData->setColumnWidth(1,75);
    ui->tableViewShipData->setColumnWidth(2,225);
    ui->tableViewShipData->setColumnWidth(3,65);
    ui->tableViewShipData->setColumnWidth(4,60);
    if (modelDataShip->rowCount()){
        ui->tableViewShipData->selectRow(0);
    }
}

void FormShip::refreshShipSert(QModelIndex index)
{
    int id_part=modelDataShip->data(modelDataShip->index(index.row(),6),Qt::EditRole).toInt();
    int id_ship=modelDataShip->data(modelDataShip->index(index.row(),0),Qt::EditRole).toInt();
    QString nomSert=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(ui->tableViewShip->currentIndex().row(),1),Qt::EditRole).toString();
    QString name = modelDataShip->data(modelDataShip->index(index.row(),1),Qt::EditRole).toString();
    name+="_"+nomSert;
    name=name.replace(QRegExp("[^\\w]"), "_");
    sertificat->build(id_part,id_ship,name);
    reader->setCurrentIdShip(id_ship);
}

void FormShip::printAll()
{
    QPrintDialog printDialog(editor->printer, this);
    if (printDialog.exec()) {
        printAll(editor->printer);
    }
}

void FormShip::printAll(QPagedPaintDevice *printer)
{
    QProgressDialog* pprd = new QProgressDialog(tr("Идет формирование документа..."),"", 0, modelDataShip->rowCount(), this);
    pprd->setCancelButton(0);
    pprd->setMinimumDuration(0);
    pprd->setWindowTitle(tr("Пожалуйста, подождите"));

    QPainter painter(printer);
    for (int i=0; i<modelDataShip->rowCount(); i++){
        QCoreApplication::processEvents();
        pprd->setValue(i);
        refreshShipSert(modelDataShip->index(i,0));
        editor->drawDoc(&painter);
        if (i!=modelDataShip->rowCount()-1){
            printer->newPage();
        }
    }
    delete pprd;
}

void FormShip::pdfAll()
{
    QProgressDialog* pprd = new QProgressDialog(tr("Идет формирование документов..."),"", 0, modelDataShip->rowCount(), this);
    pprd->setCancelButton(0);
    pprd->setMinimumDuration(0);
    pprd->setWindowTitle(tr("Пожалуйста, подождите"));
    int yearSert=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(ui->tableViewShip->currentIndex().row(),2),Qt::EditRole).toDate().year();
    QString nomSert=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(ui->tableViewShip->currentIndex().row(),1),Qt::EditRole).toString();
    for (int i=0; i<modelDataShip->rowCount(); i++){
        QCoreApplication::processEvents();
        pprd->setValue(i);
        refreshShipSert(modelDataShip->index(i,0));
        QDir dir(QDir::homePath()+"/el_sertificat");
        if (!dir.exists()) dir.mkdir(dir.path());
        dir.setPath(dir.path()+"/"+QString::number(yearSert));
        if (!dir.exists()) dir.mkdir(dir.path());
        dir.setPath(dir.path()+"/"+nomSert);
        if (!dir.exists()) dir.mkdir(dir.path());
        QFile file(dir.path()+"/"+sertificat->getName()+".pdf");
        editor->exportPdf(file.fileName());
    }
    delete pprd;
}

void FormShip::multipagePdf()
{
    QString f=sertificat->getName()+".pdf";
    QString fname = QFileDialog::getSaveFileName(this,tr("Сохранить PDF"),QDir::homePath()+"/"+f, "*.pdf");
    if (!fname.isEmpty()){
        QPdfWriter writer(fname);
        writer.setPageOrientation(QPageLayout::Portrait);
        writer.setPageSize(QPageSize(QPageSize::A4));
        writer.setPageMargins(QMarginsF(30, 30, 30, 30));
        printAll(&writer);
    }
}

void FormShip::refresh()
{
    if (sender()==ui->cmdUpd){
        sertificat->clearCache();
        Rels::instance()->relSertType->refreshModel();
    }
    modelShip->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date());
    ui->tableViewShip->setColumnHidden(0,true);
    ui->tableViewShip->setColumnWidth(1,55);
    ui->tableViewShip->setColumnWidth(2,70);
    ui->tableViewShip->setColumnWidth(3,200);
    if (ui->tableViewShip->model()->rowCount()){
        ui->tableViewShip->selectRow(ui->tableViewShip->model()->rowCount()-1);
    } else {
        modelDataShip->refresh(-1);
        editor->document()->clear();
        reader->setCurrentIdShip(-1);
    }
}

void FormShip::partReq(QModelIndex index)
{
    int id_part=modelDataShip->data(modelDataShip->index(index.row(),5)).toInt();
    Rels::instance()->partSelectReq(id_part);
}

void FormShip::signFinished()
{
    reloadDataShip();
    ui->tabWidget->setCurrentIndex(1);
}

void FormShip::reloadDataShip()
{
    int id_ship=modelDataShip->data(modelDataShip->index(ui->tableViewShipData->currentIndex().row(),0),Qt::EditRole).toInt();
    ui->tableViewShipData->blockSignals(true);
    modelDataShip->refresh();
    ui->tableViewShipData->blockSignals(false);
    for (int i=0; i<modelDataShip->rowCount(); i++){
        int id=modelDataShip->data(modelDataShip->index(i,0),Qt::EditRole).toInt();
        if (id==id_ship){
            ui->tableViewShipData->setCurrentIndex(ui->tableViewShipData->model()->index(i,1));
            break;
        }
    }
}
