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
    readerPart = new Reader();

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(ui->dateEditBeg->date().addYears(1));

    colVal cm;
    cm.val=283;
    if (!Rels::instance()->relElMark->model()->isInital()){
        Rels::instance()->relElMark->refreshModel();
    }
    ui->comboBoxMar->setModel(Rels::instance()->relElMark->model());
    ui->comboBoxMar->setCurrentData(cm);
    ui->comboBoxMar->setEnabled(false);

    modelSrcGost = new ModelRo(this);
    ui->listViewGost->setModel(modelSrcGost);

    modelAdd = new ModelRo(this);
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

    ui->cmdCopyMechForward->setVisible(false);

    connect(ui->cmdSavePrim,SIGNAL(clicked()),this,SLOT(savePrim()));
    connect(ui->cmdUpd,SIGNAL(clicked()),this,SLOT(refresh()));
    connect(ui->tableViewPart->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(refreshPartSert(QModelIndex)));
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
    connect(Rels::instance(),SIGNAL(partReq(int,QString)),this,SLOT(findPart(int,QString)));
    connect(modelSertChem,SIGNAL(sigUpd()),modelPart,SLOT(refreshState()));
    connect(modelSertMech,SIGNAL(sigUpd()),modelPart,SLOT(refreshState()));
    connect(ui->checkBoxOk,SIGNAL(clicked(bool)),this,SLOT(saveOk()));
    connect(ui->toolButtonGenChem,SIGNAL(clicked(bool)),this,SLOT(genChem()));
    connect(ui->toolButtonGenMech,SIGNAL(clicked(bool)),this,SLOT(genMech()));
    connect(editorPart,SIGNAL(signFinished()),modelAdd,SLOT(select()));

    refresh();
}

FormPart::~FormPart()
{
    savesettings();
    delete editorPart;
    delete readerPart;
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
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
}

void FormPart::loadAdd(int id_part)
{
    QSqlQuery query;
    QString qu="select s.nom_s, s.dat_vid, p.short, o.massa, rp.short, o.ds_status, o.id "
            "from otpusk as o "
            "inner join sertifikat as s on o.id_sert=s.id "
            "inner join poluch as p on s.id_pol=p.id "
            "inner join poluch as rp on o.id_pol=rp.id "
            "where o.id_part = :id_part order by s.dat_vid, s.nom_s";
    query.prepare(qu);
    query.bindValue(":id_part",id_part);
    if (modelAdd->execQuery(query)){
        modelAdd->setHeaderData(0,Qt::Horizontal,tr("Номер"));
        modelAdd->setHeaderData(1,Qt::Horizontal,tr("Дата"));
        modelAdd->setHeaderData(2,Qt::Horizontal,tr("Получатель"));
        modelAdd->setHeaderData(3,Qt::Horizontal,tr("Масса"));
        modelAdd->setHeaderData(4,Qt::Horizontal,tr("Реальный получатель"));
        modelAdd->setHeaderData(5,Qt::Horizontal,tr("ЭЦП"));
        ui->tableViewAdd->setColumnHidden(6,true);
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

void FormPart::setLock(bool b)
{
    ui->textEditPrim->setEnabled(!b);
    ui->textEditPrimProd->setEnabled(!b);
    ui->tableViewSrcChem->setEnabled(!b);
    ui->tableViewSertChem->setEnabled(!b);
    ui->toolButtonGenChem->setEnabled(!b);
    ui->tableViewSrcMech->setEnabled(!b);
    ui->tableViewSertMech->setEnabled(!b);
    ui->tableViewSertMechx->setEnabled(!b);
    ui->toolButtonGenMech->setEnabled(!b);
    ui->cmdLbl->setEnabled(!b);
    ui->cmdLblSmall->setEnabled(!b);
    ui->cmdLblSmall2->setEnabled(!b);
    ui->cmdPartSert->setEnabled(!b);
    ui->lineEditZnam->setEnabled(!b);
    ui->cmdSaveZnam->setEnabled(!b);
    ui->checkBoxOk->setEnabled(!b);
    ui->cmdCopyZnam->setEnabled(!b);
    ui->cmdCopyMechForward->setEnabled(!b);
    ui->pushButtonCopy->setEnabled(!b);
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
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
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
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
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
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
    modelPart->refreshState();
}

void FormPart::genChem()
{
    if (!modelSertChem->isEmpty()){
        QMessageBox::information(this,tr("Предупреждение"),tr("Сначала удалите все уже существующие элементы!"),QMessageBox::Ok);
        return;
    }
    int n=QMessageBox::question(this,tr("Подтвердите действия"),tr("Сгенерировать значения на основе испытаний и похожих партий?"),QMessageBox::Yes,QMessageBox::No);
    if (n==QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("select * from gen_chem(:id)");
        query.bindValue(":id",currentIdPart());
        if (query.exec()){
            modelSertChem->select();
        } else {
            QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
        }
    }
}

void FormPart::genMech()
{
    if (!modelSertMech->isEmpty()){
        QMessageBox::information(this,tr("Предупреждение"),tr("Сначала удалите все уже существующие элементы!"),QMessageBox::Ok);
        return;
    }
    int n=QMessageBox::question(this,tr("Подтвердите действия"),tr("Сгенерировать значения на основе испытаний и похожих партий?"),QMessageBox::Yes,QMessageBox::No);
    if (n==QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("select * from gen_mech(:id)");
        query.bindValue(":id",currentIdPart());
        if (query.exec()){
            modelSertMech->select();
            modelSertMechx->select();
        } else {
            QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
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
                QMessageBox::critical(this,"Error",qu.lastError().text(),QMessageBox::Cancel);
            }
            modelSrcMech->select();
        } else {
            QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
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
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        loadPrim(id);
    }
}

void FormPart::showPartSert()
{
    QString name=modelPart->data(modelPart->index(ui->tableViewPart->currentIndex().row(),1),Qt::EditRole).toString();
    name+="_"+QString::number(modelPart->data(modelPart->index(ui->tableViewPart->currentIndex().row(),2),Qt::EditRole).toDate().year());
    name=name.replace(QRegExp("[^\\w]"), "_");
    sertificatPart->build(currentIdPart(),-1,name,"elrtr");
    editorPart->show();
}

void FormPart::showShipSert(QModelIndex index)
{
    if (index.isValid()){
        QString name=modelPart->data(modelPart->index(ui->tableViewPart->currentIndex().row(),1),Qt::EditRole).toString();
        name+="_"+QString::number(modelPart->data(modelPart->index(ui->tableViewPart->currentIndex().row(),2),Qt::EditRole).toDate().year());
        int id_ship=ui->tableViewAdd->model()->data(ui->tableViewAdd->model()->index(index.row(),6),Qt::EditRole).toInt();
        name+="_"+ui->tableViewAdd->model()->data(ui->tableViewAdd->model()->index(index.row(),0),Qt::EditRole).toString();
        name=name.replace(QRegExp("[^\\w]"), "_");
        int sign=ui->tableViewAdd->model()->data(ui->tableViewAdd->model()->index(index.row(),5),Qt::EditRole).toInt();
        if (sign>0){
            readerPart->setCurrentIdShip(id_ship,name,"elrtr");
            readerPart->show();
        } else {
            sertificatPart->build(currentIdPart(),id_ship,name,"elrtr");
            editorPart->show();
        }
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

void FormPart::findPart(int id_part, QString prefix)
{
    if (prefix!="elrtr"){
        return;
    }
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

    if (sender()==ui->checkBoxOnly && modelPart->rowCount()){
        colVal d;
        d.val=modelPart->data(modelPart->index(ui->tableViewPart->currentIndex().row(),7),Qt::EditRole).toInt();
        ui->comboBoxMar->setCurrentData(d);
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
    } else {
        modelAdd->clear();
        modelSrcGost->clear();
        ui->lineEditSrcZnam->clear();
        ui->lineEditZnam->clear();
        ui->textEditPrim->clear();
        ui->textEditPrimProd->clear();
    }

    setLock(ui->tableViewPart->model()->rowCount()<1);
    ui->cmdSavePrim->setEnabled(false);

    if (sender()==ui->cmdUpd){
        Rels::instance()->relElMark->refreshModel();
        Rels::instance()->refreshVedPix();
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

ModelPart::ModelPart(QObject *parent) :
    QSqlQueryModel(parent)
{
    stateExecutor = new Executor(this);
    connect(stateExecutor,SIGNAL(finished()),this,SLOT(refreshStateFinished()));
}

void ModelPart::refresh(QDate dbeg, QDate dend, int id_el)
{
    dateBeg = dbeg;
    dateEnd = dend;
    QString flt= (id_el==-1) ? "" : "and p.id_el= "+QString::number(id_el)+" ";
    setQuery("select p.id, p.n_s, p.dat_part, e.marka||' "+tr("ф")+" '||cast(p.diam as varchar(3)), i.nam as inam, r.nam, ev.nam, p.id_el "
                                                                       "from parti p "
                                                                       "inner join elrtr e on e.id=p.id_el "
                                                                       "inner join istoch i on i.id=p.id_ist "
                                                                       "left join rcp_nam r on r.id=p.id_rcp "
                                                                       "inner join elrtr_vars ev on ev.id = p.id_var "
                                                                       "where p.dat_part between '"+dbeg.toString("yyyy-MM-dd")+"' and '"
             +dend.toString("yyyy-MM-dd")+"' "+flt+
             "order by p.n_s, p.dat_part");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Партия"));
        setHeaderData(2, Qt::Horizontal,tr("Дата"));
        setHeaderData(3, Qt::Horizontal,tr("Марка"));
        setHeaderData(4, Qt::Horizontal,tr("Источник"));
        setHeaderData(5, Qt::Horizontal,tr("Рецептура"));
        setHeaderData(6, Qt::Horizontal,tr("Вариант"));
    }
    refreshState();
}

QVariant ModelPart::data(const QModelIndex &index, int role) const
{
    if (role == Qt::BackgroundRole) {
        int id_part=this->data(this->index(index.row(),0),Qt::EditRole).toInt();
        int area = mapStat.value(id_part);
        if (area==7 || area==6){
            return QVariant(QColor(170,255,170));
        } else if (area==2 || area==3){
            return QVariant(QColor(Qt::gray));
        } else if (area==4 || area==5){
            return QVariant(QColor(Qt::yellow));
        } else if (area==1){
            return QVariant(QColor(255,200,100));
        } else {
            return QVariant(QColor(255,170,170));
        }
    }
    if((role == Qt::DisplayRole) && index.column()==2){
        return QSqlQueryModel::data(index,role).toDate().toString("dd.MM.yy");
    }
    return QSqlQueryModel::data(index, role);
}

void ModelPart::refreshStateFinished()
{
    QVector<QVector<QVariant>> data=stateExecutor->getData();
    mapStat.clear();
    for (QVector<QVariant> row : data){
        mapStat.insert(row.at(0).toInt(),row.at(1).toInt());
    }
    if (this->rowCount()){
        emit dataChanged(this->index(0,0),this->index(this->rowCount()-1,this->columnCount()-1));
    }
}

void ModelPart::refreshState()
{
    stateExecutor->setQuery(QString("select p.id, ( "
                                    "(case when p.ok then 1 else 0 end) + "
                                    "(select case when exists(select id_chem from sert_chem where id_part=p.id) then 2 else 0 end ) + "
                                    "(case when exists(select id_mech from sert_mech where id_part=p.id) then 4 else 0 end ) "
                                    ") as r "
                                    "from parti p where p.dat_part between '%1' and '%2'").arg(dateBeg.toString("yyyy-MM-dd")).arg(dateEnd.toString("yyyy-MM-dd")));
    stateExecutor->start();
}
