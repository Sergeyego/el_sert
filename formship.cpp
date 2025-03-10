#include "formship.h"
#include "ui_formship.h"

FormShip::FormShip(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormShip)
{
    ui->setupUi(this);
    loadSettings();

    QString rsrcPath=":/icons";

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(rsrcPath + "/filesave.png"));
    ui->cmdSaveAll->setIcon(saveIcon);
    ui->pushButtonSaveDsAll->setIcon(saveIcon);

    const QIcon printIcon = QIcon::fromTheme("document-print", QIcon(rsrcPath + "/fileprint.png"));
    ui->cmdPrintAll->setIcon(printIcon);
    ui->pushButtonPrintDsAll->setIcon(printIcon);

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

    connect(editor,SIGNAL(signFinished()),this,SLOT(signFinished()));
    connect(editor,SIGNAL(signEnChanged(bool)),ui->cmdDsAll,SLOT(setEnabled(bool)));

    connect(ui->tabWidget,SIGNAL(currentChanged(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    connect(ui->cmdDsAll,SIGNAL(clicked(bool)),this,SLOT(signAll()));
    connect(ui->pushButtonSaveDsAll,SIGNAL(clicked(bool)),this,SLOT(signPdfAll()));
    connect(ui->pushButtonPrintDsAll,SIGNAL(clicked(bool)),this,SLOT(signPrintAll()));

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
    ui->tableViewShipData->setColumnWidth(1,75);
    ui->tableViewShipData->setColumnWidth(2,75);
    ui->tableViewShipData->setColumnWidth(3,225);
    ui->tableViewShipData->setColumnWidth(4,65);
    ui->tableViewShipData->setColumnWidth(5,60);
    ui->tableViewShipData->setColumnHidden(6,true);
    ui->tableViewShipData->setColumnHidden(7,true);
    if (modelDataShip->rowCount()){
        ui->tableViewShipData->selectRow(0);
    }
}

void FormShip::refreshShipSert(QModelIndex index)
{
    int id_part=modelDataShip->data(modelDataShip->index(index.row(),7),Qt::EditRole).toInt();
    int id_ship=modelDataShip->data(modelDataShip->index(index.row(),0),Qt::EditRole).toInt();
    QString prefix=modelDataShip->data(modelDataShip->index(index.row(),1),Qt::EditRole).toString();
    QString nomSert=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(ui->tableViewShip->currentIndex().row(),1),Qt::EditRole).toString();
    QString name = modelDataShip->data(modelDataShip->index(index.row(),2),Qt::EditRole).toString();
    name+="_"+nomSert;
    name=name.replace(QRegExp("[^\\w]"), "_");
    sertificat->build(id_part,id_ship,name,prefix);
    reader->setCurrentIdShip(id_ship,name);
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
    reloadDataShip();
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
    reloadDataShip();
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
        reader->setCurrentIdShip(-1,"");
    }
}

void FormShip::partReq(QModelIndex index)
{
    int id_part=modelDataShip->data(modelDataShip->index(index.row(),6)).toInt();
    Rels::instance()->partSelectReq(id_part);
}

void FormShip::signFinished()
{
    reader->setLang(sertificat->getLang());
    reloadDataShip();
    ui->tabWidget->setCurrentIndex(1);
}

void FormShip::reloadDataShip()
{
    int id_ship=modelDataShip->data(modelDataShip->index(ui->tableViewShipData->currentIndex().row(),0),Qt::EditRole).toInt();
    ui->tableViewShipData->blockSignals(true);
    modelDataShip->select();
    ui->tableViewShipData->blockSignals(false);
    for (int i=0; i<modelDataShip->rowCount(); i++){
        int id=modelDataShip->data(modelDataShip->index(i,0),Qt::EditRole).toInt();
        if (id==id_ship){
            ui->tableViewShipData->setCurrentIndex(ui->tableViewShipData->model()->index(i,1));
            break;
        }
    }
}

void FormShip::signAll()
{
    DialogSignature d;
    if (d.exec()==QDialog::Accepted){
        QString sn=d.getSN();
        QProgressDialog* pprd = new QProgressDialog(tr("Идет подписание документов..."),"", 0, modelDataShip->rowCount(), this);
        pprd->setCancelButton(0);
        pprd->setMinimumDuration(0);
        pprd->setWindowTitle(tr("Пожалуйста, подождите"));

        for (int i=0; i<modelDataShip->rowCount(); i++){
            QCoreApplication::processEvents();
            pprd->setValue(i);
            refreshShipSert(modelDataShip->index(i,0));
            editor->signDS(sn);
        }
        pprd->setLabelText("Синхронизация с облачным сервисом...");
        QByteArray st;
        HttpSyncManager::sendGet(Rels::instance()->appServer()+"/s3/sync",st);
        delete pprd;
        reloadDataShip();
    }
}

void FormShip::signPdfAll()
{
    QProgressDialog* pprd = new QProgressDialog(tr("Идет сохранение документов..."),"", 0, modelDataShip->rowCount(), this);
    pprd->setCancelButton(0);
    pprd->setMinimumDuration(0);
    pprd->setWindowTitle(tr("Пожалуйста, подождите"));
    int yearSert=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(ui->tableViewShip->currentIndex().row(),2),Qt::EditRole).toDate().year();
    QString nomSert=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(ui->tableViewShip->currentIndex().row(),1),Qt::EditRole).toString();
    QString lang=reader->getCurrentLang();
    for (int i=0; i<modelDataShip->rowCount(); i++){
        QCoreApplication::processEvents();
        pprd->setValue(i);
        int id_ship=modelDataShip->data(modelDataShip->index(i,0),Qt::EditRole).toInt();
        QByteArray data;
        bool ok = HttpSyncManager::sendGet(Rels::instance()->appServer()+"/s3/local/"+QString::number(id_ship)+"/"+lang,data);
        if (ok){
            QString name = modelDataShip->data(modelDataShip->index(i,2),Qt::EditRole).toString();
            name+="_"+nomSert;
            name=name.replace(QRegExp("[^\\w]"), "_");
            QDir dir(QDir::homePath()+"/el_sertificat");
            if (!dir.exists()) dir.mkdir(dir.path());
            dir.setPath(dir.path()+"/"+QString::number(yearSert));
            if (!dir.exists()) dir.mkdir(dir.path());
            dir.setPath(dir.path()+"/"+nomSert);
            if (!dir.exists()) dir.mkdir(dir.path());
            QFile file(dir.path()+"/"+name+"_"+lang+"_sig.pdf");
            if (file.open(QFile::WriteOnly)){
                file.write(data);
                file.close();
            }
        }
    }
    delete pprd;
}

void FormShip::signPrintAll()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()) {
        QProgressDialog* pprd = new QProgressDialog(tr("Идет формирование документа..."),"", 0, modelDataShip->rowCount(), this);
        pprd->setCancelButton(0);
        pprd->setMinimumDuration(0);
        pprd->setWindowTitle(tr("Пожалуйста, подождите"));

        QPainter painter(&printer);
        QString lang=reader->getCurrentLang();
        for (int i=0; i<modelDataShip->rowCount(); i++){
            QCoreApplication::processEvents();
            pprd->setValue(i);
            int id_ship=modelDataShip->data(modelDataShip->index(i,0),Qt::EditRole).toInt();
            QByteArray data;
            bool ok = HttpSyncManager::sendGet(Rels::instance()->appServer()+"/s3/img/"+QString::number(id_ship)+"/"+lang+"/300",data);
            if (ok && data.size()){
                QImage img = QImage::fromData(data,"png");
                painter.drawImage(painter.window(),img);
                if (i<modelDataShip->rowCount()-1){
                    printer.newPage();
                }
            }
        }
        delete pprd;
    }
}

ModelShip::ModelShip(QObject *parent) : ModelRo(parent)
{

}

void ModelShip::refresh(QDate begDate, QDate endDate)
{
    QSqlQuery query;
    query.prepare("select s.id, s.nom_s, s.dat_vid, p.short from sertifikat as s "
                  "inner join poluch as p on p.id=s.id_pol "
                  "where s.dat_vid between :d1 and :d2 order by s.nom_s, s.dat_vid");
    query.bindValue(":d1",begDate);
    query.bindValue(":d2",endDate);
    if (execQuery(query)){
        setHeaderData(1, Qt::Horizontal,tr("Номер"));
        setHeaderData(2, Qt::Horizontal,tr("Дата"));
        setHeaderData(3, Qt::Horizontal,tr("Получатель"));
    }
}

ModelDataShip::ModelDataShip(QObject *parent) : ModelRo(parent)
{
    setDecimal(1);
}

void ModelDataShip::refresh(int id_ship)
{
    QSqlQuery query;
    query.prepare("select z.id, z.prefix, z.parti, z.mark, z.massa, z.ds_status, z.r, z.id_part from ( "
                  "(select o.id, 'elrtr' as prefix, p.n_s||'-'||date_part('year',p.dat_part) as parti, "
                  "e.marka||' ф '||cast(p.diam as varchar(3))||CASE WHEN p.id_var <> 1 THEN (' /'::text || ev.nam::text) || '/'::text ELSE ''::text END AS mark, "
                  "o.massa as massa, o.ds_status as ds_status, "
                  "(select case when exists(select id_chem from sert_chem where id_part=p.id) then 1 else 0 end + "
                  "case when exists(select id_mech from sert_mech where id_part=p.id) then 2 else 0 end ) as r, p.id as id_part "
                  "from otpusk o inner join parti p on o.id_part=p.id "
                  "inner join elrtr e on e.id=p.id_el "
                  "inner join istoch i on i.id=p.id_ist "
                  "inner join elrtr_vars ev on ev.id=p.id_var "
                  "where o.id_sert = :id_ship order by p.n_s, p.dat_part) "
                  "union "
                  "(select w.id, 'wire', m.n_s||'-'||date_part('year',m.dat), "
                  "pr.nam||' ф '|| d.sdim ||' '||k.short ||' '|| CASE WHEN p.id_var <> 1 THEN (' /'::text || ev.nam::text) || '/'::text ELSE ''::text END AS mark, "
                  "w.m_netto, w.ds_status, "
                  "(select case when exists(select id from wire_parti_chem where id_part=(select p.id_m from wire_parti as p where p.id = w.id_wparti)) then 1 else 0 end + "
                  "case when exists(select id from wire_parti_mech where id_part=(select p.id_m from wire_parti as p where p.id = w.id_wparti)) then 2 else 0 end) as r, "
                  "p.id "
                  "from wire_shipment_consist w "
                  "inner join wire_parti p on p.id=w.id_wparti "
                  "inner join wire_parti_m m on p.id_m=m.id "
                  "inner join provol pr on pr.id=m.id_provol "
                  "inner join diam d on d.id=m.id_diam "
                  "inner join wire_pack_kind k on p.id_pack=k.id "
                  "inner join elrtr_vars ev on ev.id=p.id_var "
                  "where w.id_ship= :id_ship "
                  "order by pr.nam, d.sdim, k.short, m.n_s) "
                  ") as z order by z.prefix, z.mark, z.parti");
    query.bindValue(":id_ship",id_ship);
    if (execQuery(query)){
        setHeaderData(1, Qt::Horizontal,tr("Продукция"));
        setHeaderData(2, Qt::Horizontal,tr("Партия"));
        setHeaderData(3, Qt::Horizontal,tr("Марка"));
        setHeaderData(4, Qt::Horizontal,tr("Масса, кг"));
        setHeaderData(5, Qt::Horizontal,tr("ЭЦП"));
    }
}

QVariant ModelDataShip::data(const QModelIndex &index, int role) const
{
    if((role == Qt::BackgroundRole)&&(this->columnCount()>3)) {
        if (index.column()!=5){
            int area = record(index.row()).value(6).toInt();
            if(area == 0) {
                return QVariant(QColor(255,170,170));
            } else if(area == 1) {
                return QVariant(QColor(Qt::yellow));
            } else if(area == 2) {
                return QVariant(QColor(Qt::gray));
            } else if(area == 3) {
                return QVariant(QColor(170,255,170));
            }
        } else {
            int stat = record(index.row()).value(5).toInt();
            if(stat == 0) {
                return QVariant(QColor(255,170,170));
            } else if(stat == 1) {
                return QVariant(QColor(Qt::yellow));
            } else if(stat == 2) {
                return QVariant(QColor(170,255,170));
            }
        }
    }

    if (role == Qt::DisplayRole){
        if (index.column()==5){
            int stat = record(index.row()).value(5).toInt();
            if(stat == 1) {
                return QString("..");
            } else if(stat == 2) {
                return QString("OK");
            } else {
                return QString("-");
            }
        }
    }
    return ModelRo::data(index, role);
}
