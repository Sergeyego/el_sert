#include "formdoc.h"
#include "ui_formdoc.h"

FormDoc::FormDoc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDoc)
{
    ui->setupUi(this);

    loadsettings();

    modelElTypes = new DbTableModel("zvd_types",this);
    modelElTypes->addColumn("id_sert","id_sert",true,TYPE_INT);
    modelElTypes->addColumn("id_tip",QString::fromUtf8("Тип"),true,TYPE_STRING,NULL,Rels::instance()->relElTypes);
    modelElTypes->setSort("zvd_types.id_tip");
    ui->tableViewVid->setModel(modelElTypes);
    ui->tableViewVid->setColumnHidden(0,true);
    ui->tableViewVid->setColumnWidth(1,120);

    modelEl = new DbTableModel("zvd_els",this);
    modelEl->addColumn("id_sert","id_sert",true,TYPE_INT);
    modelEl->addColumn("id_el",QString::fromUtf8("Марка"),true,TYPE_STRING,NULL,Rels::instance()->relElMark);
    modelEl->addColumn("id_grade",QString::fromUtf8("Категория"),false,TYPE_STRING,NULL,Rels::instance()->relGrade);
    modelEl->setSuffix("inner join elrtr as e on e.id=zvd_els.id_el");
    modelEl->setSort("e.marka");
    ui->tableViewEl->setModel(modelEl);
    ui->tableViewEl->setColumnHidden(0,true);
    ui->tableViewEl->setColumnWidth(1,110);
    ui->tableViewEl->setColumnWidth(2,200);

    modelElDim = new DbTableModel("zvd_eldim",this);
    modelElDim->addColumn("id_sert","id_sert",true,TYPE_INT);
    modelElDim->addColumn("id_eldr",QString::fromUtf8("Марка / диам."),true,TYPE_STRING,NULL,Rels::instance()->relElDim);
    modelElDim->addColumn("id_grade",QString::fromUtf8("Категория"),false,TYPE_STRING,NULL,Rels::instance()->relGrade);
    modelElDim->setSuffix("inner join dry_els as d on d.ide=zvd_eldim.id_eldr");
    modelElDim->setSort("d.fnam");
    ui->tableViewElDim->setModel(modelElDim);
    ui->tableViewElDim->setColumnHidden(0,true);
    ui->tableViewElDim->setColumnWidth(1,130);
    ui->tableViewElDim->setColumnWidth(2,200);

    ui->tableVieewGrade->setModel(Rels::instance()->modelGrade);
    ui->tableVieewGrade->setColumnHidden(0,true);
    ui->tableVieewGrade->setColumnWidth(1,250);

    modelDoc = new DbTableModel("zvd_sert",this);
    modelDoc->addColumn("id","id",true,TYPE_INT);
    modelDoc->addColumn("nom_doc",QString::fromUtf8("Номер документа"),false,TYPE_STRING);
    modelDoc->addColumn("nazv",QString::fromUtf8("Название документа"),false,TYPE_STRING);
    modelDoc->addColumn("dat_doc",QString::fromUtf8("Дата документа"),false,TYPE_DATE);
    modelDoc->addColumn("dat_beg",QString::fromUtf8("Дата начала"),false,TYPE_DATE);
    modelDoc->addColumn("dat_end",QString::fromUtf8("Дата конца"),false,TYPE_DATE);
    modelDoc->addColumn("nom_bl",QString::fromUtf8("Номер бланка"),false,TYPE_STRING);
    modelDoc->addColumn("nom_sl",QString::fromUtf8("Номер наклейки"),false,TYPE_STRING);
    modelDoc->addColumn("txt",QString::fromUtf8("Текст документа"),false,TYPE_STRING);
    modelDoc->addColumn("gr_tech_ust",QString::fromUtf8("ГТУ"),false,TYPE_STRING);
    modelDoc->addColumn("id_ved",QString::fromUtf8("Ведомство"),false,TYPE_STRING,NULL,Rels::instance()->relVed);
    modelDoc->addColumn("id_doc",QString::fromUtf8("Тип документа"),false,TYPE_STRING,NULL,Rels::instance()->relVidDoc);

    modelDoc->setSort("zvd_sert.nom_doc, zvd_sert.dat_doc");
    selectDoc(ui->checkBoxActive->isChecked());

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

    mapper->addEmptyLock(ui->tableViewVid);
    mapper->addEmptyLock(ui->tableViewEl);
    mapper->addEmptyLock(ui->tableViewElDim);
    ui->horizontalLayoutMap->insertWidget(0,mapper);

    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshData(int)));
    connect(ui->checkBoxActive,SIGNAL(clicked(bool)),this,SLOT(selectDoc(bool)));
    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(updElDim()));

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
    QSettings settings("szsm", "el_sert");
    ui->splitter->restoreState(settings.value("doc_splitter").toByteArray());
}

void FormDoc::savesettings()
{
    QSettings settings("szsm", "el_sert");
    settings.setValue("doc_splitter", ui->splitter->saveState());
}

void FormDoc::refreshData(int index)
{
    int id=ui->tableViewDoc->model()->data(ui->tableViewDoc->model()->index(index,0),Qt::EditRole).toInt();

    modelElTypes->setDefaultValue(0,id);
    modelElTypes->setFilter("zvd_types.id_sert = "+QString::number(id));
    modelElTypes->select();

    modelEl->setDefaultValue(0,id);
    modelEl->setFilter("zvd_els.id_sert = "+QString::number(id));
    modelEl->select();

    modelElDim->setDefaultValue(0,id);
    modelElDim->setFilter("zvd_eldim.id_sert = "+QString::number(id));
    modelElDim->select();
}

void FormDoc::selectDoc(bool active)
{
    if (active){
        modelDoc->setFilter("COALESCE(zvd_sert.dat_end,'3000-01-01'::date) >= '"+QDate::currentDate().toString("yyyy-MM-dd")+"'");
    } else {
        modelDoc->setFilter("");
    }
    modelDoc->select();
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
