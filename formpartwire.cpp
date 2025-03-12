#include "formpartwire.h"
#include "ui_formpartwire.h"

FormPartWire::FormPartWire(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormPartWire)
{
    ui->setupUi(this);
    loadSettings();

    sertificatPart = new SertBuild(this);
    editorPart = new Editor(sertificatPart);
    readerPart = new Reader();

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(ui->dateEditBeg->date().addYears(1));

    if (!Rels::instance()->relProvol->isInital()){
        Rels::instance()->relProvol->refreshModel();
    }
    ui->comboBoxMarka->setModel(Rels::instance()->relProvol->model());

    modelTuSrc = new ModelRo(this);
    ui->tableViewSrcTu->setModel(modelTuSrc);

    modelTu = new DbTableModel("wire_parti_gost",this);
    modelTu->addColumn("id_parti","id_parti");
    modelTu->addColumn("id_gost",QString::fromUtf8("Наименование"),Rels::instance()->relGost);
    modelTu->setSort("gost_new.nam");

    ui->tableViewTu->setModel(modelTu);
    ui->tableViewTu->setColumnHidden(0,true);
    ui->tableViewTu->setColumnWidth(1,200);

    modelShip = new ModelRo(this);
    ui->tableViewShip->setModel(modelShip);

    modelSrcChem = new ModelChemSrcWire(this);
    ui->tableViewSrcChem->setModel(modelSrcChem);
    ui->tableViewSrcChem->setColumnHidden(0,true);
    ui->tableViewSrcChem->setColumnWidth(1,100);
    ui->tableViewSrcChem->setColumnWidth(2,90);

    modelChem = new ModelChemSertWire(this);
    ui->tableViewChem->setModel(modelChem);
    ui->tableViewChem->setColumnHidden(0,true);
    ui->tableViewChem->setColumnHidden(1,true);
    ui->tableViewChem->setColumnWidth(2,100);
    ui->tableViewChem->setColumnWidth(3,90);

    modelSrcMech = new DbTableModel("wire_mech",this);
    modelSrcMech->addColumn("id","id");
    modelSrcMech->addColumn("id_part","id_part");
    modelSrcMech->addColumn("id_mech",tr("Параметр"),Rels::instance()->relMech);
    modelSrcMech->addColumn("kvo",tr("Значен."));
    modelSrcMech->setDecimals(3,3);
    modelSrcMech->setDefaultValue(2,1);
    modelSrcMech->setSort("wire_mech.id_mech, wire_mech.kvo");
    ui->tableViewSrcMech->setModel(modelSrcMech);
    ui->tableViewSrcMech->setColumnHidden(0,true);
    ui->tableViewSrcMech->setColumnHidden(1,true);
    ui->tableViewSrcMech->setColumnWidth(2,250);
    ui->tableViewSrcMech->setColumnWidth(3,75);

    modelMech = new ModelWirePartiMech(this);
    ui->tableViewMech->setModel(modelMech);
    ui->tableViewMech->setColumnHidden(0,true);
    ui->tableViewMech->setColumnHidden(1,true);
    ui->tableViewMech->setColumnWidth(2,250);
    ui->tableViewMech->setColumnWidth(3,75);
    ui->tableViewMech->setColumnWidth(4,75);
    ui->tableViewMech->setColumnWidth(5,250);

    mapper = new QDataWidgetMapper(this);

    modelPartWire = new ModelPartWire(this);
    ui->tableViewPart->setModel(modelPartWire);

    mapper->setModel(modelPartWire);
    mapper->addMapping(ui->lineEditSrcPart,8);
    mapper->addMapping(ui->lineEditPodt,9);
    mapper->addMapping(ui->lineEditVol,10);
    mapper->addMapping(ui->lineEditLine,11);
    mapper->addMapping(ui->lineEditKvo,12);
    mapper->addMapping(ui->lineEditComment,15);

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->tableViewPart->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(currentIndexChanged(QModelIndex)));
    connect(ui->checkBoxOnly,SIGNAL(clicked(bool)),ui->comboBoxMarka,SLOT(setEnabled(bool)));
    connect(ui->comboBoxMarka,SIGNAL(currentIndexChanged(int)),this,SLOT(upd()));
    connect(ui->checkBoxOnly,SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->toolButtonCopyTu,SIGNAL(clicked(bool)),this,SLOT(copyTu()));
    connect(ui->tableViewShip,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(showShipSert(QModelIndex)));
    connect(editorPart,SIGNAL(signFinished()),modelShip,SLOT(select()));
    connect(ui->pushButtonCert,SIGNAL(clicked(bool)),this,SLOT(showPartSert()));
    connect(ui->pushButtonSavePrim,SIGNAL(clicked(bool)),this,SLOT(savePrim()));
    connect(ui->plainTextEditPrim,SIGNAL(textChanged()),this,SLOT(enPrimSave()));
    connect(ui->toolButtonCopyChem,SIGNAL(clicked(bool)),this,SLOT(copyChem()));
    connect(modelChem,SIGNAL(sigUpd()),modelPartWire,SLOT(refreshState()));
    connect(modelMech,SIGNAL(sigUpd()),modelPartWire,SLOT(refreshState()));
    connect(ui->pushButtonCode,SIGNAL(clicked(bool)),this,SLOT(showCheckForm()));
    connect(Rels::instance(),SIGNAL(partReq(int,QString)),this,SLOT(findPart(int,QString)));
    upd();
}

FormPartWire::~FormPartWire()
{
    saveSettings();
    delete ui;
}

void FormPartWire::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->ui->splitter->restoreState(settings.value("wire_parti_splitter_width").toByteArray());
}

void FormPartWire::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("wire_parti_splitter_width",ui->splitter->saveState());
}

void FormPartWire::setLock(bool b)
{
    ui->tableViewTu->setEnabled(!b);
    ui->toolButtonCopyTu->setEnabled(!b);
    ui->plainTextEditPrim->setEnabled(!b);
    ui->tableViewSrcChem->setEnabled(!b);
    ui->tableViewChem->setEnabled(!b);
    ui->toolButtonCopyChem->setEnabled(!b);
    ui->tableViewSrcMech->setEnabled(!b);
    ui->tableViewMech->setEnabled(!b);
    ui->pushButtonCert->setEnabled(!b);
}

void FormPartWire::upd()
{
    if (sender()==ui->pushButtonUpd){
        Rels::instance()->relProvol->refreshModel();
    }
    bool b=ui->checkBoxOnly->isChecked();
    if (b && sender()==ui->checkBoxOnly && ui->tableViewPart->model()->rowCount()){
        colVal d;
        d.val=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(ui->tableViewPart->currentIndex().row(),14),Qt::EditRole).toInt();
        ui->comboBoxMarka->setCurrentData(d);
    }
    int id_pr = b? ui->comboBoxMarka->getCurrentData().val.toInt() : -1;

    modelPartWire->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),id_pr);
    ui->tableViewPart->setColumnHidden(0,true);
    for (int i=8; i<ui->tableViewPart->model()->rowCount(); i++){
        ui->tableViewPart->setColumnHidden(i,true);
    }
    ui->tableViewPart->resizeToContents();
    int n=ui->tableViewPart->model()->rowCount();
    if (n){
        ui->tableViewPart->setCurrentIndex(ui->tableViewPart->model()->index(n-1,1));
    } else {
        ui->lineEditSrcPart->clear();
        ui->lineEditPodt->clear();
        ui->lineEditVol->clear();
        ui->lineEditLine->clear();
        ui->lineEditKvo->clear();
        ui->lineEditComment->clear();
        modelTuSrc->clear();
        modelShip->clear();
        ui->plainTextEditPrim->clear();
    }
    ui->pushButtonSavePrim->setEnabled(false);
    setLock(n<1);
}

void FormPartWire::currentIndexChanged(QModelIndex index)
{
    mapper->setCurrentIndex(index.row());

    int id_part=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(index.row(),0),Qt::EditRole).toInt();
    int id_m=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(index.row(),13),Qt::EditRole).toInt();
    int id_prov=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(index.row(),14),Qt::EditRole).toInt();
    int id_buht=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(index.row(),16),Qt::EditRole).toInt();

    QSqlQuery queryTu;
    queryTu.prepare("select g.id, g.nam from wire_gost as w "
                    "inner join gost_new as g on w.id_gost=g.id "
                    "where w.id_provol = :id_prov order by g.nam");
    queryTu.bindValue(":id_prov",id_prov);
    if (modelTuSrc->execQuery(queryTu)){
        modelTuSrc->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("Наименование"));
        ui->tableViewSrcTu->setColumnHidden(0,true);
        ui->tableViewSrcTu->resizeToContents();
    }

    modelTu->setFilter("wire_parti_gost.id_parti = "+QString::number(id_m));
    modelTu->setDefaultValue(0,id_m);
    modelTu->select();

    QSqlQuery queryShip;
    queryShip.prepare("select d.id, s.dat_vid, s.nom_s, p.short, d.m_netto, rp.short, d.ds_status "
                      "from wire_shipment_consist as d "
                      "inner join sertifikat as s on s.id=d.id_ship "
                      "inner join poluch as p on s.id_pol=p.id "
                      "inner join poluch as rp on d.id_pol=rp.id "
                      "where d.id_wparti = :id_part order by s.dat_vid");
    queryShip.bindValue(":id_part",id_part);
    if (modelShip->execQuery(queryShip)){
        modelShip->setHeaderData(1, Qt::Horizontal,tr("Дата"));
        modelShip->setHeaderData(2, Qt::Horizontal,tr("№"));
        modelShip->setHeaderData(3, Qt::Horizontal,tr("Получатель"));
        modelShip->setHeaderData(4, Qt::Horizontal,tr("Масса, кг"));
        modelShip->setHeaderData(5, Qt::Horizontal,tr("Реальный получатель"));
        modelShip->setHeaderData(6, Qt::Horizontal,tr("ЭЦП"));
        ui->tableViewShip->setColumnHidden(0,true);
        ui->tableViewShip->resizeToContents();
    }

    QSqlQuery queryPrim;
    queryPrim.prepare("select wp.prim_prod from wire_parti wp where wp.id = :id_part");
    queryPrim.bindValue(":id_part",id_part);
    if (queryPrim.exec()){
        queryPrim.next();
        ui->plainTextEditPrim->setPlainText(queryPrim.value(0).toString());
        ui->pushButtonSavePrim->setEnabled(false);
    } else {
        QMessageBox::critical(this,tr("Ошибка"),queryPrim.lastError().text(),QMessageBox::Cancel);
    }

    modelSrcChem->refresh(id_buht);
    modelChem->refresh(id_m);

    modelSrcMech->setDefaultValue(1,id_m);
    modelSrcMech->setFilter("wire_mech.id_part = "+QString::number(id_m));
    modelSrcMech->select();

    modelMech->refresh(id_m);
}

void FormPartWire::copyTu()
{
    int row=ui->tableViewPart->currentIndex().row();
    int id_m=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(row,13),Qt::EditRole).toInt();
    int id_prov=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(row,14),Qt::EditRole).toInt();

    QSqlQuery query;
    query.prepare("insert into wire_parti_gost (id_parti, id_gost) "
                  "(select :id_m, g.id_gost from wire_gost as g where g.id_provol = :id_prov)");
    query.bindValue(":id_m",id_m);
    query.bindValue(":id_prov",id_prov);
    if (query.exec()){
        modelTu->select();
    } else {
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
}

void FormPartWire::showShipSert(QModelIndex index)
{
    if (index.isValid()){
        int row=ui->tableViewPart->currentIndex().row();
        int id_part=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(row,0),Qt::EditRole).toInt();
        QString n_s=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(row,1),Qt::EditRole).toString();
        QDate date=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(row,2),Qt::EditRole).toDate();
        int id_ship=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(index.row(),0),Qt::EditRole).toInt();
        QString name=n_s+"_"+QString::number(date.year())+"_"+ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(index.row(),2),Qt::EditRole).toString();
        name=name.replace(QRegExp("[^\\w]"), "_");
        int sign=ui->tableViewShip->model()->data(ui->tableViewShip->model()->index(index.row(),6),Qt::EditRole).toInt();
        if (sign>0){
            readerPart->setCurrentIdShip(id_ship,name,"wire");
            readerPart->show();
        } else {
            sertificatPart->build(id_part,id_ship,name,"wire");
            editorPart->show();
        }
    }
}

void FormPartWire::showPartSert()
{
    int row=ui->tableViewPart->currentIndex().row();
    int id_part=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(row,0),Qt::EditRole).toInt();
    QString n_s=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(row,1),Qt::EditRole).toString();
    QDate date=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(row,2),Qt::EditRole).toDate();
    QString name=n_s+"_"+QString::number(date.year());

    sertificatPart->build(id_part,-1,name,"wire");
    editorPart->show();
}

void FormPartWire::savePrim()
{
    int row=ui->tableViewPart->currentIndex().row();
    int id_part=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(row,0),Qt::EditRole).toInt();
    QSqlQuery query;
    query.prepare("update wire_parti set prim_prod = :prim where id = :id_part");
    query.bindValue(":prim",ui->plainTextEditPrim->toPlainText());
    query.bindValue(":id_part",id_part);
    if (!query.exec()){
        QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Cancel);
    } else {
        ui->pushButtonSavePrim->setEnabled(false);
    }
}

void FormPartWire::enPrimSave()
{
    ui->pushButtonSavePrim->setEnabled(true);
}

void FormPartWire::copyChem()
{
    if (!modelSrcChem->rowCount()) return;
    int row=ui->tableViewPart->currentIndex().row();
    int id_m=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(row,13),Qt::EditRole).toInt();
    if (!modelChem->isEmpty()){
        QMessageBox::information(this,tr("Предупреждение"),tr("Сначала удалите все уже существующие элементы!"),QMessageBox::Ok);
        return;
    }
    int n=QMessageBox::question(this,tr("Подтвердите действия"),tr("Скопировать значения?"),QMessageBox::Yes,QMessageBox::No);
    if (n==QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("insert into wire_parti_chem (id_part, id_chem, value ) "
                      "(select :id_part1, pr.id_el, pr.kvo from prov_chem as pr "
                      "where pr.id_buht=(select p.id_buht from wire_parti_m as p where p.id= :id_part2 ))");
        query.bindValue(":id_part1",id_m);
        query.bindValue(":id_part2",id_m);
        if (query.exec()){
            modelPartWire->refreshState();
            modelChem->select();
        } else {
            QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
        }
    }
}

void FormPartWire::showCheckForm()
{
    CheckForm *f = new CheckForm();
    f->setCheckEl(false);
    f->show();
    connect(f,SIGNAL(sigClose()),f,SLOT(deleteLater()));
}

void FormPartWire::findPart(int id_part, QString prefix)
{
    if (prefix!="wire"){
        return;
    }
    bool ok=false;
    for (int i=0; i<modelPartWire->rowCount(); i++){
        if (id_part==modelPartWire->data(modelPartWire->index(i,0),Qt::EditRole).toInt()){
            ui->tableViewPart->selectRow(i);
            ok=true;
            break;
        }
    }
    if (!ok){
        QMessageBox::information(this,tr("Информация"),tr("Не найдено такой партии за указанный период"),QMessageBox::Ok);
    }
}



ModelPartWire::ModelPartWire(QWidget *parent) : ModelRo(parent)
{
    setDecimal(1);
}

void ModelPartWire::refresh(QDate beg, QDate end, int id_provol)
{
    dateBeg=beg;
    dateEnd=end;
    refreshState();
    QString flt = (id_provol>0) ? ("and wpm.id_provol = "+QString::number(id_provol)+" "):"";
    QSqlQuery query;
    query.prepare("select wp.id, wpm.n_s, wpm.dat, ws.nam, p.nam, d.diam, "
                  "wpk.short ||' ('||wp2.mas_ed ||' кг)', ev.nam, "
                  "pb.str, wp3.str, we.snam, wl.snam, wpm.kvo, wp.id_m, wpm.id_provol, wpm.comment, wpm.id_buht "
                  "from wire_parti wp "
                  "inner join wire_parti_m wpm on wpm.id = wp.id_m "
                  "inner join wire_source ws on ws.id = wpm.id_source "
                  "inner join provol p on p.id = wpm.id_provol "
                  "inner join diam d on d.id = wpm.id_diam "
                  "inner join wire_pack_kind wpk on wpk.id = wp.id_pack "
                  "inner join wire_pack wp2 on wp2.id = wp.id_pack_type "
                  "inner join elrtr_vars ev on ev.id = wp.id_var "
                  "inner join prov_buht pb on pb.id = wpm.id_buht "
                  "left join wire_podt wp3 on wp3.id = wpm.id_podt "
                  "left join wire_empl we on we.id = wpm.id_empl "
                  "inner join wire_line wl on wl.id = wpm.id_type "
                  "where wpm.dat between :d1 and :d2 "+flt+
                  "order by date_part('year',wpm.dat), wpm.n_s, wpm.dat, wpk.short");
    query.bindValue(":d1",beg);
    query.bindValue(":d2",end);
    if (execQuery(query)){
        setHeaderData(1,Qt::Horizontal,tr("Номер"));
        setHeaderData(2,Qt::Horizontal,tr("Дата"));
        setHeaderData(3,Qt::Horizontal,tr("Источник"));
        setHeaderData(4,Qt::Horizontal,tr("Марка"));
        setHeaderData(5,Qt::Horizontal,tr("Диам."));
        setHeaderData(6,Qt::Horizontal,tr("Носитель"));
        setHeaderData(7,Qt::Horizontal,tr("Вариант"));
    }
}

QVariant ModelPartWire::data(const QModelIndex &item, int role) const
{
    if((role == Qt::BackgroundRole)) {
        int area = colorState.value(data(this->index(item.row(),13),Qt::EditRole).toInt());
        if(area == 0) return QVariant(QColor(255,170,170)); else
            if(area == 1) return QVariant(QColor(Qt::yellow)); else
                if(area == 2) return QVariant(QColor(Qt::gray)); else
                    if(area == 3) return QVariant(QColor(170,255,170));
    } else return ModelRo::data(item,role);
}

void ModelPartWire::refreshState()
{
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("select wire_parti_m.id, "
                  "(select case when exists(select id from wire_parti_chem where id_part=wire_parti_m.id) "
                  "then 1 else 0 end "
                  "+ "
                  "case when exists(select id from wire_parti_mech where id_part=wire_parti_m.id) "
                  "then 2 else 0 end "
                  "as r) from wire_parti_m where wire_parti_m.dat between :d1 and :d2");
    query.bindValue(":d1",dateBeg);
    query.bindValue(":d2",dateEnd);
    if (query.exec()){
        colorState.clear();
        while (query.next()){
            colorState[query.value(0).toInt()]=query.value(1).toInt();
        }
        if (this->rowCount()){
            emit dataChanged(this->index(0,0),this->index(this->rowCount()-1,this->columnCount()-1));
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}

ModelWirePartiMech::ModelWirePartiMech(QObject *parent):DbTableModel("wire_parti_mech",parent)
{
    addColumn("id","id");
    addColumn("id_part","id_part");
    addColumn("id_mech",tr("Параметр"),Rels::instance()->relMech);
    addColumn("value",tr("Значен."));
    addColumn("value_max",tr("Значен.-2"));
    addColumn("id_cat",tr("Категория"),Rels::instance()->relMechCat);
    setSort("mech_tbl.nam");
    setDecimals(3,3);
    setDecimals(4,3);
}

void ModelWirePartiMech::refresh(int id_part)
{
    setFilter("wire_parti_mech.id_part="+QString::number(id_part));
    setDefaultValue(1,id_part);
    select();
}

bool ModelWirePartiMech::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column()==2 && role==Qt::EditRole){
        int id_cat=0;
        int id_mech=value.toInt();
        if (id_mech==1 || id_mech==2 || id_mech==3 || id_mech==21)
            id_cat=0;
        else if (id_mech==23 || id_mech==24)
            id_cat=1;
        else id_cat=2;
        DbTableModel::setData(this->index(index.row(),5),id_cat,role);
    }
    return DbTableModel::setData(index,value,role);
}
