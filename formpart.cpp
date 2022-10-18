#include "formpart.h"
#include "ui_formpart.h"

FormPart::FormPart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPart)
{
    ui->setupUi(this);

    loadsettings();

    sertificatPart = new SertBuild(this);
    editorPart = new Editor(sertificatPart);

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(ui->dateEditBeg->date().addYears(1));

    colVal cm;
    cm.val=283;
    ui->comboBoxMar->setModel(Rels::instance()->relElMark->model());
    ui->comboBoxMar->setCurrentData(cm);
    ui->comboBoxMar->setEnabled(false);

    modelSrcGost = new ModelRo(this);
    ui->listViewGost->setModel(modelSrcGost);

    modelAdd = new QSqlQueryModel(this);
    ui->tableViewAdd->verticalHeader()->setDefaultSectionSize(ui->tableViewAdd->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewAdd->verticalHeader()->hide();
    ui->tableViewAdd->setModel(modelAdd);

    modelSrcChem = new ModelChemSrc(this);
    ui->tableViewSrcChem->setModel(modelSrcChem);
    ui->tableViewSrcChem->setColumnHidden(0,true);
    ui->tableViewSrcChem->setColumnHidden(1,true);
    ui->tableViewSrcChem->setColumnWidth(2,80);
    ui->tableViewSrcChem->setColumnWidth(3,70);
    ui->tableViewSrcChem->setColumnWidth(4,70);
    ui->tableViewSrcChem->setColumnWidth(5,110);

    modelSertChem = new ModelChemSert(this);
    ui->tableViewSertChem->setModel(modelSertChem);
    ui->tableViewSertChem->setColumnHidden(0,true);
    ui->tableViewSertChem->setColumnWidth(1,80);
    ui->tableViewSertChem->setColumnWidth(2,70);

    modelSrcMech = new ModelMechSrc(this);
    ui->tableViewSrcMech->setModel(modelSrcMech);
    ui->tableViewSrcMech->setColumnHidden(0,true);
    ui->tableViewSrcMech->setColumnWidth(1,180);
    ui->tableViewSrcMech->setColumnWidth(2,80);

    modelSertMech = new ModelMechSert(this);
    ui->tableViewSertMech->setModel(modelSertMech);
    ui->tableViewSertMech->setColumnHidden(0,true);
    ui->tableViewSertMech->setColumnWidth(1,180);
    ui->tableViewSertMech->setColumnWidth(2,80);

    modelSertMechx = new DbTableModel("sert_mechx",this);
    modelSertMechx->addColumn("id_part","id_part");
    modelSertMechx->addColumn("id_mechx",tr("Параметр"),Rels::instance()->relMechx);
    modelSertMechx->addColumn("id_value",tr("Значение"),Rels::instance()->relMechxVal);
    ui->tableViewSertMechx->setModel(modelSertMechx);
    ui->tableViewSertMechx->setColumnHidden(0,true);
    ui->tableViewSertMechx->setColumnWidth(1,150);
    ui->tableViewSertMechx->setColumnWidth(2,110);

    ui->tableViewPart->verticalHeader()->setDefaultSectionSize(ui->tableViewPart->verticalHeader()->fontMetrics().height()*1.5);
    modelPart = new ModelPart(this);
    ui->tableViewPart->setModel(modelPart);
    ui->tableViewPart->verticalHeader()->hide();

    connect(ui->cmdSavePrim,SIGNAL(clicked()),this,SLOT(savePrim()));
    connect(ui->cmdUpd,SIGNAL(clicked()),this,SLOT(refresh()));
    connect(ui->tableViewPart->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(refreshPartSert(QModelIndex)));
    connect(ui->cmdCopyChem,SIGNAL(clicked()),this,SLOT(copyChem()));
    connect(ui->cmdCopyMech,SIGNAL(clicked()),this,SLOT(copyMech()));
    connect(ui->cmdCopyMechForward,SIGNAL(clicked()),this,SLOT(copyMechForward()));
    connect(ui->cmdPartSert,SIGNAL(clicked()),this,SLOT(showPartSert()));
    connect(ui->cmdCheck,SIGNAL(clicked()),this,SLOT(showCheckForm()));
    connect(ui->checkBoxOnly,SIGNAL(clicked(bool)),ui->comboBoxMar,SLOT(setEnabled(bool)));
    connect(ui->checkBoxOnly,SIGNAL(clicked()),this,SLOT(refresh()));
    connect(ui->comboBoxMar,SIGNAL(currentIndexChanged(int)),this,SLOT(refresh()));
    connect(ui->cmdSaveZnam,SIGNAL(clicked()),this,SLOT(saveZnam()));
    connect(ui->cmdCopyZnam,SIGNAL(clicked()),this,SLOT(copyZnam()));
    connect(ui->tableViewAdd,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(showShipSert(QModelIndex)));
    connect(ui->cmdLbl,SIGNAL(clicked(bool)),this,SLOT(genLbl()));
    connect(ui->cmdLblSmall,SIGNAL(clicked(bool)),this,SLOT(genLblSmall()));
    connect(ui->cmdLblSmall2,SIGNAL(clicked(bool)),this,SLOT(genLblSmall2()));
    connect(ui->textEditPrim,SIGNAL(textChanged()),this,SLOT(enPrimSave()));
    connect(ui->textEditPrimProd,SIGNAL(textChanged()),this,SLOT(enPrimSave()));
    connect(ui->lineEditZnam,SIGNAL(textChanged(QString)),this,SLOT(enZnamSave()));
    connect(ui->pushButtonCopy,SIGNAL(clicked(bool)),this,SLOT(copyVal()));
    connect(Rels::instance(),SIGNAL(partReq(int)),this,SLOT(findPart(int)));
    connect(modelSertChem,SIGNAL(sigUpd()),modelPart,SLOT(refreshState()));
    connect(modelSertMech,SIGNAL(sigUpd()),modelPart,SLOT(refreshState()));
    connect(ui->checkBoxOk,SIGNAL(clicked(bool)),this,SLOT(saveOk()));

    refresh();
}

FormPart::~FormPart()
{
    savesettings();
    delete editorPart;
    delete ui;
}

void FormPart::loadPrim(int id_part)
{
    ui->textEditPrim->clear();
    ui->lineEditZnam->clear();
    ui->lineEditSrcZnam->clear();
    QSqlQuery query;
    query.prepare("select p.prim, p.ibco, ev.znam, p.prim_prod, p.ok from parti as p "
                  "inner join elrtr as e on p.id_el=e.id "
                  "left join el_var ev on ev.id_el = p.id_el and ev.id_var = p.id_var "
                  "where p.id = :id");
    query.bindValue(":id",id_part);
    if (query.exec()){
        while (query.next()){
            ui->textEditPrim->setPlainText(query.value(0).toString());
            ui->lineEditZnam->setText(query.value(1).toString());
            ui->lineEditSrcZnam->setText(query.value(2).toString());
            ui->textEditPrimProd->setPlainText(query.value(3).toString());
            ui->checkBoxOk->setChecked(query.value(4).toBool());
        }
        ui->cmdSavePrim->setEnabled(false);
        ui->cmdSaveZnam->setEnabled(false);
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
}

void FormPart::loadAdd(int id_part)
{
    QSqlQuery query;
    QString qu="select s.nom_s, s.dat_vid, p.short, o.massa, o.id from otpusk as o "
            "inner join sertifikat as s on o.id_sert=s.id "
            "inner join poluch as p on s.id_pol=p.id "
            "where o.id_part= "+QString::number(id_part)+" order by s.dat_vid, s.nom_s";
    query.prepare(qu);
    bool ok = query.exec();
    if (ok){
        modelAdd->setQuery(query);
        modelAdd->setHeaderData(0,Qt::Horizontal,tr("Номер"));
        modelAdd->setHeaderData(1,Qt::Horizontal,tr("Дата"));
        modelAdd->setHeaderData(2,Qt::Horizontal,tr("Получатель"));
        modelAdd->setHeaderData(3,Qt::Horizontal,tr("Масса"));
        ui->tableViewAdd->setColumnHidden(4,true);

    } else {
        modelAdd->clear();
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
    ui->tableViewAdd->resizeColumnsToContents();
}

void FormPart::loadGost(int id_part)
{
    QSqlQuery tuQuery;
    tuQuery.prepare("select nam "
                    "from zvd_get_tu_var((select dat_part from parti where id = :id1 ), "
                    "(select id_el from parti where id = :id2 ), "
                    "(select d.id from diam as d where d.diam = (select diam from parti where id = :id3 )), "
                    "(select id_var from parti where id = :id4) ) ");
    tuQuery.bindValue(":id1",id_part);
    tuQuery.bindValue(":id2",id_part);
    tuQuery.bindValue(":id3",id_part);
    tuQuery.bindValue(":id4",id_part);
    if (modelSrcGost->execQuery(tuQuery)){
        modelSrcGost->setHeaderData(0,Qt::Horizontal,tr("ГОСТ/ТУ"));
    }
}

int FormPart::currentIdPart()
{
    return modelPart->data(modelPart->index(ui->tableViewPart->currentIndex().row(),0),Qt::EditRole).toInt();
}

void FormPart::loadsettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->ui->splitterVert->restoreState(settings.value("part_splitter_width").toByteArray());
}

void FormPart::savesettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("part_splitter_width",ui->splitterVert->saveState());
}

void FormPart::savePrim()
{
    int id=currentIdPart();
    QSqlQuery query;
    query.prepare("update parti set prim=:prim, prim_prod=:primprod where id=:id");
    query.bindValue(":prim",ui->textEditPrim->toPlainText());
    query.bindValue(":primprod",ui->textEditPrimProd->toPlainText());
    query.bindValue(":id",id);
    if (!query.exec()){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        ui->cmdSavePrim->setEnabled(false);
    }
}

void FormPart::saveZnam()
{
    int id=currentIdPart();
    QSqlQuery query;
    query.prepare("update parti set ibco=:ibco where id=:id");
    query.bindValue(":ibco",ui->lineEditZnam->text().isEmpty() ? NULL : ui->lineEditZnam->text());
    query.bindValue(":id",id);
    if (!query.exec()){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        ui->cmdSaveZnam->setEnabled(false);
    }
}

void FormPart::saveOk()
{
    int id=currentIdPart();
    QSqlQuery query;
    query.prepare("update parti set ok=:ok where id=:id");
    query.bindValue(":ok",ui->checkBoxOk->isChecked());
    query.bindValue(":id",id);
    if (!query.exec()){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
    modelPart->refreshState();
}

void FormPart::copyChem()
{
    if (modelSrcChem->rowCount()==1 && modelSrcChem->isAdd()) return;
    int r=modelSertChem->isAdd()? modelSertChem->rowCount()-1 : modelSertChem->rowCount();
    if (r){
        QMessageBox::information(this,tr("Предупреждение"),tr("Сначала удалите все уже существующие элементы!"),QMessageBox::Ok);
        return;
    }
    int n=QMessageBox::question(this,tr("Подтвердите действия"),tr("Скопировать средние значения?"),QMessageBox::Yes,QMessageBox::No);
    if (n==QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("insert into sert_chem (id_part, id_chem, value)"
                      "(select id_part, id_chem, avg(kvo) from parti_chem where id_part=:id group by id_part, id_chem)");
        query.bindValue(":id",currentIdPart());
        if (query.exec()){
            modelSertChem->select();
        } else {
            QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
        }
    }
}

void FormPart::copyMech()
{
    if (modelSrcMech->rowCount()==1 && modelSrcMech->isAdd()) return;
    int r=modelSertMech->isAdd()? modelSertMech->rowCount()-1 : modelSertMech->rowCount();
    if (r){
        QMessageBox::information(this,tr("Предупреждение"),tr("Сначала удалите все уже существующие элементы!"),QMessageBox::Ok);
        return;
    }
    int n=QMessageBox::question(this,tr("Подтвердите действия"),tr("Скопировать значения?"),QMessageBox::Yes,QMessageBox::No);
    if (n==QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("insert into sert_mech (id_part, id_mech, value)"
                      "(select id_part, id_mech, kvo from parti_mech where id_part = :id )");
        query.bindValue(":id",currentIdPart());
        if (query.exec()){
            modelSertMech->select();
        } else {
            QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
        }
    }
}


void FormPart::copyMechForward()
{
    if (modelSertMech->rowCount()==1 && modelSertMech->isAdd()) return;
    int r=modelSrcMech->isAdd()? modelSrcMech->rowCount()-1 : modelSrcMech->rowCount();
    if (r){
        QMessageBox::information(this,tr("Предупреждение"),tr("Сначала удалите все уже существующие элементы!"),QMessageBox::Ok);
        return;
    }
    int n=QMessageBox::question(this,tr("Подтвердите действия"),tr("Скопировать значения?"),QMessageBox::Yes,QMessageBox::No);
    if (n==QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("insert into parti_mech (id_part, id_mech, kvo) "
                      "(select id_part, id_mech, value from sert_mech where id_part = :id )");
        query.bindValue(":id",currentIdPart());
        if (query.exec()){
            QSqlQuery qu;
            qu.prepare("insert into parti_mech (id_part, id_mech, kvo) values (:id, 5, 0)");
            qu.bindValue(":id",currentIdPart());
            if (!qu.exec()){
                QMessageBox::critical(NULL,"Error",qu.lastError().text(),QMessageBox::Cancel);
            }
            modelSrcMech->select();
        } else {
            QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
        }
    }
}

void FormPart::copyZnam()
{
    int id=currentIdPart();
    QSqlQuery query;
    query.prepare("update parti as p set ibco = :ibco where p.id=:id");
    query.bindValue(":ibco",ui->lineEditSrcZnam->text());
    query.bindValue(":id",id);
    if (!query.exec()){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        loadPrim(id);
    }
}

void FormPart::showPartSert()
{
    sertificatPart->build(currentIdPart(),false);
    editorPart->show();
}

void FormPart::showShipSert(QModelIndex index)
{
    if (index.isValid()){
        int id_ship=ui->tableViewAdd->model()->data(ui->tableViewAdd->model()->index(index.row(),4),Qt::EditRole).toInt();
        sertificatPart->build(id_ship, true);
        editorPart->show();
    }
}

void FormPart::showCheckForm()
{
    CheckForm *f = new CheckForm();
    f->show();
    connect(f,SIGNAL(sigClose()),f,SLOT(deleteLater()));
}

void FormPart::genLbl()
{
    int id=currentIdPart();
    LblCreator c;
    c.createLbl(id,false);
}

void FormPart::genLblSmall()
{
    int id=currentIdPart();
    LblCreator c;
    c.createLblGlabels(id);
}

void FormPart::genLblSmall2()
{
    int id=currentIdPart();
    LblCreator c;
    c.createLblGlabels2(id);
}

void FormPart::enPrimSave()
{
    ui->cmdSavePrim->setEnabled(true);
}

void FormPart::enZnamSave()
{
    ui->cmdSaveZnam->setEnabled(true);
}

void FormPart::copyVal()
{
    DialogCopy d(currentIdPart());
    if (d.exec()==QDialog::Accepted){
        int id_p=d.idPart();
        if (id_p!=-1){
            QSqlQuery queryChem;
            queryChem.prepare("insert into sert_chem (id_part, id_chem, value) "
                              "(select :id_part, sc.id_chem, sc.value "
                              "from sert_chem sc "
                              "where sc.id_part = :id_partsrc)");
            queryChem.bindValue(":id_partsrc",id_p);
            queryChem.bindValue(":id_part",currentIdPart());
            if (queryChem.exec()){
                modelSertChem->select();
            } else {
                QMessageBox::critical(this,tr("Ошибка"),queryChem.lastError().text(),QMessageBox::Ok);
            }
            QSqlQuery queryMech;
            queryMech.prepare("insert into sert_mech (id_part, id_mech, value) "
                              "(select :id_part, sm.id_mech, sm.value "
                              "from sert_mech sm "
                              "where sm.id_part = :id_partsrc)");
            queryMech.bindValue(":id_partsrc",id_p);
            queryMech.bindValue(":id_part",currentIdPart());
            if (queryMech.exec()){
                modelSertMech->select();
            } else {
                QMessageBox::critical(this,tr("Ошибка"),queryMech.lastError().text(),QMessageBox::Ok);
            }
            QSqlQuery queryMechx;
            queryMechx.prepare("insert into sert_mechx (id_part, id_mechx, id_value) "
                              "(select :id_part, smx.id_mechx, smx.id_value "
                              "from sert_mechx smx "
                              "where smx.id_part = :id_partsrc)");
            queryMechx.bindValue(":id_partsrc",id_p);
            queryMechx.bindValue(":id_part",currentIdPart());
            if (queryMechx.exec()){
                modelSertMechx->select();
            } else {
                QMessageBox::critical(this,tr("Ошибка"),queryMechx.lastError().text(),QMessageBox::Ok);
            }
        }
    }
}

void FormPart::findPart(int id_part)
{
    bool ok=false;
    for (int i=0; i<modelPart->rowCount(); i++){
        if (id_part==modelPart->data(modelPart->index(i,0),Qt::EditRole).toInt()){
            ui->tableViewPart->selectRow(i);
            ok=true;
            break;
        }
    }
    if (!ok){
        QMessageBox::information(this,tr("Информация"),tr("Не найдено такой партии за указанный период"),QMessageBox::Ok);
    }
}

void FormPart::refresh()
{
    if (sender()==ui->comboBoxMar && ui->comboBoxMar->currentIndex()<0){
        return;
    }
    int id_el=-1;
    if (ui->checkBoxOnly->isChecked() && ui->comboBoxMar->currentIndex()>=0){
        id_el=ui->comboBoxMar->getCurrentData().val.toInt();
    }
    modelPart->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),id_el);
    ui->tableViewPart->setColumnHidden(0,true);
    ui->tableViewPart->setColumnHidden(7,true);
    ui->tableViewPart->setColumnWidth(1,55);
    ui->tableViewPart->setColumnWidth(2,70);
    ui->tableViewPart->setColumnWidth(3,150);
    ui->tableViewPart->setColumnWidth(4,90);
    ui->tableViewPart->setColumnWidth(5,125);
    ui->tableViewPart->setColumnWidth(6,125);
    if (ui->tableViewPart->model()->rowCount()){
        ui->tableViewPart->selectRow(ui->tableViewPart->model()->rowCount()-1);
        ui->tableViewPart->scrollToBottom();
    }
    if (sender()==ui->cmdUpd){
        Rels::instance()->relElMark->refreshModel();
        Rels::instance()->refreshVedPix();
        sertificatPart->refreshGenData();
        modelSertChem->refreshRelsModel();
        modelSrcMech->refreshRelsModel();
        modelSertMechx->refreshRelsModel();
    }
}

void FormPart::refreshPartSert(QModelIndex index)
{
    int id=modelPart->data(modelPart->index(index.row(),0),Qt::EditRole).toInt();

    modelSrcMech->refresh(id);
    modelSertMech->refresh(id);

    modelSertMechx->setFilter("sert_mechx.id_part = "+QString::number(id));
    modelSertMechx->setDefaultValue(0,id);
    modelSertMechx->select();

    modelSrcChem->refresh(id);
    modelSertChem->refresh(id);

    loadPrim(id);
    loadAdd(id);
    loadGost(id);

}
