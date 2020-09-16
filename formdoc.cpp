#include "formdoc.h"
#include "ui_formdoc.h"

FormDoc::FormDoc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDoc)
{
    ui->setupUi(this);

    loadsettings();

    modelEl = new DbTableModel("zvd_els",this);
    modelEl->addColumn("id_sert","id_sert");
    modelEl->addColumn("id_el",QString::fromUtf8("Марка"),NULL,Rels::instance()->relElMark);
    modelEl->addColumn("id_grade",QString::fromUtf8("Категория"),NULL,Rels::instance()->relGrade);
    modelEl->setSuffix("inner join elrtr as e on e.id=zvd_els.id_el");
    modelEl->setSort("e.marka");
    ui->tableViewEl->setModel(modelEl);
    ui->tableViewEl->setColumnHidden(0,true);
    ui->tableViewEl->setColumnWidth(1,110);
    ui->tableViewEl->setColumnWidth(2,200);

    modelElDim = new DbTableModel("zvd_eldim",this);
    modelElDim->addColumn("id_sert","id_sert");
    modelElDim->addColumn("id_eldr",QString::fromUtf8("Марка / диам."),NULL,Rels::instance()->relElDim);
    modelElDim->addColumn("id_grade",QString::fromUtf8("Категория"),NULL,Rels::instance()->relGrade);
    modelElDim->setSuffix("inner join dry_els as d on d.ide=zvd_eldim.id_eldr");
    modelElDim->setSort("d.fnam");
    ui->tableViewElDim->setModel(modelElDim);
    ui->tableViewElDim->setColumnHidden(0,true);
    ui->tableViewElDim->setColumnWidth(1,130);
    ui->tableViewElDim->setColumnWidth(2,200);

    ui->tableVieewGrade->setModel(Rels::instance()->modelGrade);
    ui->tableVieewGrade->setColumnHidden(0,true);
    ui->tableVieewGrade->setColumnWidth(1,250);

    modelWireDiam = new DbTableModel("zvd_wire_diam_sert");
    modelWireDiam->addColumn("id","id");
    modelWireDiam->addColumn("id_sert","id_sert");
    modelWireDiam->addColumn("id_provol",QString::fromUtf8("Проволока"),NULL,Rels::instance()->relProvol);
    modelWireDiam->addColumn("id_diam",QString::fromUtf8("Диам."),NULL,Rels::instance()->relWireDiam);
    modelWireDiam->addColumn("id_grade",QString::fromUtf8("Категория"),NULL,Rels::instance()->relGrade);
    modelWireDiam->setSuffix("inner join provol as p on p.id=zvd_wire_diam_sert.id_provol "
                         "inner join diam as d on d.id=zvd_wire_diam_sert.id_diam");
    modelWireDiam->setSort("p.nam, d.diam");
    ui->tableViewWireDiam->setModel(modelWireDiam);
    ui->tableViewWireDiam->setColumnHidden(0,true);
    ui->tableViewWireDiam->setColumnHidden(1,true);
    ui->tableViewWireDiam->setColumnWidth(2,120);
    ui->tableViewWireDiam->setColumnWidth(3,60);
    ui->tableViewWireDiam->setColumnWidth(4,100);

    modelWire = new DbTableModel("zvd_wire_sert");
    modelWire->addColumn("id_sert","id_sert");
    modelWire->addColumn("id_provol",QString::fromUtf8("Проволока"),NULL,Rels::instance()->relProvol);
    modelWire->addColumn("id_grade",QString::fromUtf8("Категория"),NULL,Rels::instance()->relGrade);
    modelWire->setSuffix("inner join provol as p on p.id=zvd_wire_sert.id_provol");
    modelWire->setSort("p.nam");
    ui->tableViewWire->setModel(modelWire);
    ui->tableViewWire->setColumnHidden(0,true);
    ui->tableViewWire->setColumnWidth(1,150);
    ui->tableViewWire->setColumnWidth(2,100);

    modelDoc = new ModelDoc(this);
    modelDoc->refresh(ui->checkBoxActive->isChecked());

    ui->tableViewDoc->setModel(modelDoc);
    ui->tableViewDoc->setColumnHidden(0,true);
    ui->tableViewDoc->setColumnWidth(1,200);
    for (int i=2; i<modelDoc->rowCount(); i++){
        ui->tableViewDoc->setColumnHidden(i,true);
    }

    mapper = new DbMapper(ui->tableViewDoc,this);
    mapper->addMapping(ui->lineEditDoc,1);
    mapper->addMapping(ui->lineEditNam,2);
    mapper->addMapping(ui->dateEditDoc,3);
    mapper->addMapping(ui->dateEditBeg,4);
    mapper->addMapping(ui->dateEditEnd,5);
    mapper->addMapping(ui->lineEditNumBl,6);
    mapper->addMapping(ui->lineEditNumNak,7);
    mapper->addMapping(ui->textEditDoc,8);
    mapper->addMapping(ui->lineEditGtu,9);
    mapper->addMapping(ui->comboBoxVed,10);
    mapper->addMapping(ui->comboBoxVid,11);

    mapper->addEmptyLock(ui->tableViewWireDiam);
    mapper->addEmptyLock(ui->tableViewEl);
    mapper->addEmptyLock(ui->tableViewElDim);
    mapper->addEmptyLock(ui->pushButtonUpload);
    mapper->addEmptyLock(ui->tableViewWire);
    ui->horizontalLayoutMap->insertWidget(0,mapper);

    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshData(int)));
    connect(ui->checkBoxActive,SIGNAL(clicked(bool)),modelDoc,SLOT(refresh(bool)));
    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(updElDim()));

    connect(ui->pushButtonUpload,SIGNAL(clicked(bool)),this,SLOT(upload()));
    connect(ui->pushButtonView,SIGNAL(clicked(bool)),this,SLOT(viewSert()));
    connect(ui->pushButtonSaveAs,SIGNAL(clicked(bool)),this,SLOT(saveAs()));
    connect(ui->pushButtonDel,SIGNAL(clicked(bool)),this,SLOT(delSert()));
    connect(modelDoc,SIGNAL(sigList()),this,SLOT(updState()));


    if (ui->tableViewDoc->model()->rowCount()){
        ui->tableViewDoc->selectRow(0);
    }
}

FormDoc::~FormDoc()
{
    savesettings();
    delete ui;
}

void FormDoc::loadsettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    ui->splitter->restoreState(settings.value("doc_splitter").toByteArray());
}

void FormDoc::savesettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("doc_splitter", ui->splitter->saveState());
}

void FormDoc::setOk(bool ok)
{
    ui->pushButtonSaveAs->setEnabled(ok);
    ui->pushButtonView->setEnabled(ok);
    ui->pushButtonDel->setEnabled(ok);
    QString pixname= ok ? QString(":/images/ok.png") : QString(":/images/notok.png");
    ui->labelSert->setPixmap(QPixmap(pixname));
}

int FormDoc::currentId()
{
    return ui->tableViewDoc->model()->data(ui->tableViewDoc->model()->index(ui->tableViewDoc->currentIndex().row(),0),Qt::EditRole).toInt();
}

QString FormDoc::currentDocNumber()
{
    return ui->tableViewDoc->model()->data(ui->tableViewDoc->model()->index(ui->tableViewDoc->currentIndex().row(),1),Qt::EditRole).toString();
}

void FormDoc::refreshData(int index)
{
    int id=ui->tableViewDoc->model()->data(ui->tableViewDoc->model()->index(index,0),Qt::EditRole).toInt();

    modelEl->setDefaultValue(0,id);
    modelEl->setFilter("zvd_els.id_sert = "+QString::number(id));
    modelEl->select();

    modelElDim->setDefaultValue(0,id);
    modelElDim->setFilter("zvd_eldim.id_sert = "+QString::number(id));
    modelElDim->select();

    modelWireDiam->setDefaultValue(1,id);
    modelWireDiam->setFilter("zvd_wire_diam_sert.id_sert = "+QString::number(id));
    modelWireDiam->select();

    modelWire->setDefaultValue(0,id);
    modelWire->setFilter("zvd_wire_sert.id_sert = "+QString::number(id));
    modelWire->select();

    updState();
}

void FormDoc::updElDim()
{
    QSqlQuery query;
    query.prepare("select * from rx_els()");
    if (query.exec()){
        Rels::instance()->relElDim->refreshModel();
    } else {
        QMessageBox::critical(this,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}

void FormDoc::upload()
{
    modelDoc->ftpPut(currentId());
}

void FormDoc::viewSert()
{
    modelDoc->ftpGet(currentId(),1);
}

void FormDoc::saveAs()
{
    modelDoc->ftpGet(currentId(),2);
}

void FormDoc::updState()
{
    setOk(modelDoc->ftpExist(currentId()));
}

void FormDoc::delSert()
{
    modelDoc->ftpDel(currentId());
}
