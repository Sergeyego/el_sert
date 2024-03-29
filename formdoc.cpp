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
    modelEl->addColumn("id_el",QString::fromUtf8("Марка"),Rels::instance()->relElMark);
    modelEl->addColumn("id_grade",QString::fromUtf8("Категория"),Rels::instance()->relGrade);
    modelEl->setSuffix("inner join elrtr as e on e.id=zvd_els.id_el");
    modelEl->setSort("e.marka");
    ui->tableViewEl->setModel(modelEl);
    ui->tableViewEl->setColumnHidden(0,true);
    ui->tableViewEl->setColumnWidth(1,170);
    ui->tableViewEl->setColumnWidth(2,200);

    modelElDim = new DbTableModel("zvd_eldim",this);
    modelElDim->addColumn("id_sert","id_sert");
    modelElDim->addColumn("id_eldr",QString::fromUtf8("Марка / диам."),Rels::instance()->relElDim);
    modelElDim->addColumn("id_grade",QString::fromUtf8("Категория"),Rels::instance()->relGrade);
    modelElDim->setSuffix("inner join dry_els as d on d.ide=zvd_eldim.id_eldr");
    modelElDim->setSort("d.fnam");
    ui->tableViewElDim->setModel(modelElDim);
    ui->tableViewElDim->setColumnHidden(0,true);
    ui->tableViewElDim->setColumnWidth(1,190);
    ui->tableViewElDim->setColumnWidth(2,200);

    modelTu = new DbTableModel("zvd_sert_tu",this);
    modelTu->addColumn("id_sert","id_sert");
    modelTu->addColumn("id_tu",QString::fromUtf8("ТУ, ОСТ"),Rels::instance()->relGost);
    modelTu->setSort("gost_new.nam");

    ui->tableViewTu->setModel(modelTu);
    ui->tableViewTu->setColumnHidden(0,true);
    ui->tableViewTu->setColumnWidth(1,210);

    modelEn = new ModelEn(this);
    ui->listViewEn->setModel(modelEn);

    modelWireDiam = new DbTableModel("zvd_wire_diam_sert");
    modelWireDiam->addColumn("id","id");
    modelWireDiam->addColumn("id_sert","id_sert");
    modelWireDiam->addColumn("id_provol",QString::fromUtf8("Проволока"),Rels::instance()->relProvol);
    modelWireDiam->addColumn("id_diam",QString::fromUtf8("Диам."),Rels::instance()->relWireDiam);
    modelWireDiam->addColumn("id_grade",QString::fromUtf8("Категория"),Rels::instance()->relGrade);
    modelWireDiam->setSuffix("inner join provol as p on p.id=zvd_wire_diam_sert.id_provol "
                         "inner join diam as d on d.id=zvd_wire_diam_sert.id_diam");
    modelWireDiam->setSort("p.nam, d.diam");
    ui->tableViewWireDiam->setModel(modelWireDiam);
    ui->tableViewWireDiam->setColumnHidden(0,true);
    ui->tableViewWireDiam->setColumnHidden(1,true);
    ui->tableViewWireDiam->setColumnWidth(2,150);
    ui->tableViewWireDiam->setColumnWidth(3,60);
    ui->tableViewWireDiam->setColumnWidth(4,220);

    modelWire = new DbTableModel("zvd_wire_sert");
    modelWire->addColumn("id_sert","id_sert");
    modelWire->addColumn("id_provol",QString::fromUtf8("Проволока"),Rels::instance()->relProvol);
    modelWire->addColumn("id_grade",QString::fromUtf8("Категория"),Rels::instance()->relGrade);
    modelWire->setSuffix("inner join provol as p on p.id=zvd_wire_sert.id_provol");
    modelWire->setSort("p.nam");
    ui->tableViewWire->setModel(modelWire);
    ui->tableViewWire->setColumnHidden(0,true);
    ui->tableViewWire->setColumnWidth(1,150);
    ui->tableViewWire->setColumnWidth(2,220);

    modelDoc = new ModelDoc(this);
    modelDoc->refresh(ui->checkBoxActive->isChecked());

    ui->tableViewDoc->setModel(modelDoc);
    ui->tableViewDoc->setColumnHidden(0,true);
    ui->tableViewDoc->setColumnWidth(1,190);
    ui->tableViewDoc->setColumnWidth(10,150);
    for (int i=2; i<modelDoc->rowCount(); i++){
        if (i!=10){
            ui->tableViewDoc->setColumnHidden(i,true);
        }
    }

    mapper = new DbMapper(ui->tableViewDoc,this);
    mapper->addMapping(ui->lineEditDoc,1);
    mapper->addMapping(ui->lineEditNam,2);
    mapper->addMapping(ui->dateEditDoc,3);
    mapper->addMapping(ui->dateEditBeg,4);
    mapper->addMapping(ui->dateEditEnd,5);
    mapper->addMapping(ui->lineEditNumBl,6);
    mapper->addMapping(ui->lineEditNumNak,7);
    //mapper->addMapping(ui->textEditDoc,8);
    mapper->addMapping(ui->lineEditGtu,9);
    mapper->addMapping(ui->comboBoxVed,10);
    mapper->addMapping(ui->comboBoxVid,11);

    mapper->addEmptyLock(ui->tableViewWireDiam);
    mapper->addEmptyLock(ui->tableViewEl);
    mapper->addEmptyLock(ui->tableViewElDim);
    mapper->addEmptyLock(ui->pushButtonUpload);
    mapper->addEmptyLock(ui->tableViewWire);
    mapper->addEmptyLock(ui->tableViewTu);
    mapper->addEmptyLock(ui->listViewEn);
    mapper->addEmptyLock(ui->toolButtonCheckAll);
    mapper->addEmptyLock(ui->toolButtonUnCheckAll);
    ui->horizontalLayoutMap->insertWidget(0,mapper);

    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshData(int)));
    connect(ui->checkBoxActive,SIGNAL(clicked(bool)),modelDoc,SLOT(refresh(bool)));
    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(updElDim()));

    connect(ui->pushButtonUpload,SIGNAL(clicked(bool)),this,SLOT(upload()));
    connect(ui->pushButtonView,SIGNAL(clicked(bool)),this,SLOT(viewSert()));
    connect(ui->pushButtonSaveAs,SIGNAL(clicked(bool)),this,SLOT(saveAs()));
    connect(ui->pushButtonDel,SIGNAL(clicked(bool)),this,SLOT(delSert()));
    connect(modelDoc,SIGNAL(sigList()),this,SLOT(updState()));
    connect(ui->toolButtonCheckAll,SIGNAL(clicked(bool)),modelEn,SLOT(checkAll()));
    connect(ui->toolButtonUnCheckAll,SIGNAL(clicked(bool)),modelEn,SLOT(unCheckAll()));


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

    modelTu->setDefaultValue(0,id);
    modelTu->setFilter("zvd_sert_tu.id_sert = "+QString::number(id));
    modelTu->select();

    modelEn->refresh(id);

    updState();
}

void FormDoc::updElDim()
{
    modelDoc->refreshRelsModel();
    Rels::instance()->refreshElDim();
    Rels::instance()->relGost->refreshModel();
    Rels::instance()->relElMark->refreshModel();
    Rels::instance()->relGrade->refreshModel();
    Rels::instance()->relProvol->refreshModel();
    Rels::instance()->relWireDiam->refreshModel();
    modelDoc->select();
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

ModelEn::ModelEn(QWidget *parent) : QSqlQueryModel(parent)
{
    id_d=-1;
}

Qt::ItemFlags ModelEn::flags(const QModelIndex &index) const
{
    return index.column()==0 ? (Qt::ItemIsSelectable |Qt::ItemIsUserCheckable | Qt::ItemIsEnabled) : (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void ModelEn::refresh(int id_doc)
{
    id_d=id_doc;
    disList.clear();
    QSqlQuery query;
    query.prepare("select ev.nam, ev.id, not exists (select zsd.id_var  from zvd_sert_disable zsd where zsd.id_sert = :id_sert and zsd.id_var=ev.id) "
                  "from elrtr_vars ev order by ev.id");
    query.bindValue(":id_sert",id_doc);
    if (query.exec()){
        while (query.next()){
            if (!query.value(2).toBool()){
                disList.push_back(query.value(1).toInt());
            }
        }
        setQuery(query);
        setHeaderData(0,Qt::Horizontal,tr("Вариант"));
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
        clear();
    }
}

QVariant ModelEn::data(const QModelIndex &item, int role) const
{
    if (role==Qt::CheckStateRole && item.column()==0){
        return disList.contains(this->data(this->index(item.row(),1),Qt::EditRole).toInt()) ? Qt::Unchecked : Qt::Checked;
    }
    return QSqlQueryModel::data(item,role);
}

bool ModelEn::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role==Qt::CheckStateRole && index.column()==0){
        bool ok=false;
        int id_var=this->data(this->index(index.row(),1),Qt::EditRole).toInt();
        ok=writeSql(id_var,value.toBool());
        if (ok) {
            if (value.toBool()){
                if (disList.contains(id_var)){
                    disList.removeAt(disList.indexOf(id_var));
                    emit dataChanged(index,index);
                }
            } else {
                if (!disList.contains(id_var)){
                    disList.push_back(id_var);
                    emit dataChanged(index,index);
                }
            }
        }
        return ok;
    }
    return QSqlQueryModel::setData(index,value,role);
}

void ModelEn::checkAll()
{
    QSqlQuery query;
    query.prepare("delete from zvd_sert_disable where id_sert = :id_sert");
    query.bindValue(":id_sert",id_d);
    if (!query.exec()){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        refresh(id_d);
    }
}

void ModelEn::unCheckAll()
{
    QSqlQuery query;
    query.prepare("insert into zvd_sert_disable (id_sert, id_var) (select :id_sert, ev.id from elrtr_vars ev) ON CONFLICT DO NOTHING");
    query.bindValue(":id_sert",id_d);
    if (!query.exec()){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        refresh(id_d);
    }
}

bool ModelEn::writeSql(int id_var, bool value)
{
    bool ok=false;
    if (value){
        QSqlQuery query;
        query.prepare("delete from zvd_sert_disable where id_sert = :id_sert and id_var = :id_var");
        query.bindValue(":id_sert",id_d);
        query.bindValue(":id_var",id_var);
        ok=query.exec();
        if (!ok){
            QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
        }
    } else {
        QSqlQuery query;
        query.prepare("insert into zvd_sert_disable (id_sert, id_var) values (:id_sert, :id_var) ON CONFLICT DO NOTHING");
        query.bindValue(":id_sert",id_d);
        query.bindValue(":id_var",id_var);
        ok=query.exec();
        if (!ok){
            QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
        }
    }
    return ok;
}
