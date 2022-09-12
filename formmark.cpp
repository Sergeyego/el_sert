#include "formmark.h"
#include "ui_formmark.h"

FormMark::FormMark(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMark)
{
    ui->setupUi(this);
    loadsettings();

    ui->lineEditPr->setInputMask(QString("099±90°C 009 xxxxx"));
    ui->dateEdit->setDate(QDate::currentDate());

    ui->comboBoxDiam->setModel(Rels::instance()->relDiam->proxyModel());
    ui->comboBoxDiam->setModelColumn(1);
    int ind=ui->comboBoxDiam->findText("3.0 ");
    if (ind>=0){
        ui->comboBoxDiam->setCurrentIndex(ind);
    }

    ui->comboBoxProvVar->setModel(Rels::instance()->relProvol->model());
    ui->comboBoxProvVar->setModelColumn(Rels::instance()->relProvol->columnDisplay());

    ui->comboBoxVar->setModel(Rels::instance()->relVar->model());
    ui->comboBoxVar->setModelColumn(Rels::instance()->relVar->columnDisplay());

    modelEan = new ModelEan(this);
    ui->tableViewPack->setModel(modelEan);
    ui->tableViewPack->setColumnHidden(0,true);
    ui->tableViewPack->setColumnWidth(1,70);
    ui->tableViewPack->setColumnWidth(2,260);
    ui->tableViewPack->setColumnWidth(3,125);
    ui->tableViewPack->setColumnWidth(4,125);

    DbDelegate *delegate=qobject_cast<DbDelegate *>(ui->tableViewPack->itemDelegate());
    if (delegate){
        connect(delegate,SIGNAL(createEdt(QModelIndex)),modelEan,SLOT(updRels(QModelIndex)));
    }

    modelAmp = new DbTableModel("amp",this);
    modelAmp->addColumn("id","id");
    modelAmp->addColumn("id_el","id_el");
    modelAmp->addColumn("id_var","id_var");
    modelAmp->addColumn("id_diam",QString::fromUtf8("Диаметр, мм"),NULL,Rels::instance()->relDiam);
    modelAmp->addColumn("bot",QString::fromUtf8("Нижнее"));
    modelAmp->addColumn("vert",QString::fromUtf8("Вертикальное"));
    modelAmp->addColumn("ceil",QString::fromUtf8("Потолочное"));
    modelAmp->setSuffix("inner join diam on amp.id_diam=diam.id");
    modelAmp->setSort("diam.sdim");

    ui->tableViewAmp->setModel(modelAmp);
    ui->tableViewAmp->setColumnHidden(0,true);
    ui->tableViewAmp->setColumnHidden(1,true);
    ui->tableViewAmp->setColumnHidden(2,true);
    ui->tableViewAmp->setColumnWidth(3,100);
    ui->tableViewAmp->setColumnWidth(4,100);
    ui->tableViewAmp->setColumnWidth(5,100);
    ui->tableViewAmp->setColumnWidth(6,100);

    modelChemTu = new DbTableModel("chem_tu",this);
    modelChemTu->addColumn("id_el","id_el");
    modelChemTu->addColumn("id_chem",QString::fromUtf8("Элемент"),NULL,Rels::instance()->relChem);
    modelChemTu->addColumn("min",QString::fromUtf8("Минимум, %"),new QDoubleValidator(0,1000000000,3,this));
    modelChemTu->addColumn("max",QString::fromUtf8("Максимум, %"),new QDoubleValidator(0,1000000000,3,this));
    modelChemTu->setSort("chem_tu.id_chem");

    ui->tableViewChem->setModel(modelChemTu);
    ui->tableViewChem->setColumnHidden(0,true);
    ui->tableViewChem->setColumnWidth(1,100);
    ui->tableViewChem->setColumnWidth(2,100);
    ui->tableViewChem->setColumnWidth(3,100);

    modelMechTu = new DbTableModel("mech_tu",this);
    modelMechTu->addColumn("id_el","id_el");
    modelMechTu->addColumn("id_mech",QString::fromUtf8("Параметр"),NULL,Rels::instance()->relMech);
    modelMechTu->addColumn("min",QString::fromUtf8("Минимум"),new QDoubleValidator(-1000000000,1000000000,3,this));
    modelMechTu->addColumn("max",QString::fromUtf8("Максимум"),new QDoubleValidator(-1000000000,1000000000,3,this));
    modelMechTu->setSort("mech_tu.id_mech");

    ui->tableViewMech->setModel(modelMechTu);
    ui->tableViewMech->setColumnHidden(0,true);
    ui->tableViewMech->setColumnWidth(1,150);
    ui->tableViewMech->setColumnWidth(2,90);
    ui->tableViewMech->setColumnWidth(3,90);

    modelPlav = new DbTableModel("el_plav",this);
    modelPlav->addColumn("id_el","id_el");
    modelPlav->addColumn("id_plav",QString::fromUtf8("Параметр"),NULL,Rels::instance()->relPlav);
    modelPlav->addColumn("value",QString::fromUtf8("Значение"),new QDoubleValidator(-1000000000,1000000000,2,this));

    ui->tableViewPlav->setModel(modelPlav);
    ui->tableViewPlav->setColumnHidden(0,true);
    ui->tableViewPlav->setColumnWidth(1,250);
    ui->tableViewPlav->setColumnWidth(2,80);

    modelMark = new DbTableModel("elrtr",this);
    modelMark->addColumn("id","id");
    modelMark->addColumn("marka",QString::fromUtf8("Марка"));
    modelMark->addColumn("marka_sert",QString::fromUtf8("Название для сертификата"));
    modelMark->addColumn("id_grp",QString::fromUtf8("Группа"),NULL,Rels::instance()->relGrp);
    modelMark->addColumn("id_vid",QString::fromUtf8("Вид"),NULL,Rels::instance()->relVid);
    modelMark->addColumn("id_gost",QString::fromUtf8("Проволока"),NULL,Rels::instance()->relProvol);
    modelMark->addColumn("id_pic",QString::fromUtf8("Положение при сварке"),NULL,Rels::instance()->relPol);
    modelMark->addColumn("id_gost_type",QString::fromUtf8("Тип по ГОСТ"),NULL,Rels::instance()->relGostType);
    modelMark->addColumn("id_iso_type",QString::fromUtf8("Тип по ISO"),NULL,Rels::instance()->relIso);
    modelMark->addColumn("id_aws_type",QString::fromUtf8("Тип по AWS"),NULL,Rels::instance()->relAws);
    modelMark->addColumn("id_denominator",QString::fromUtf8("Знаменатель"),NULL,Rels::instance()->relZnam);
    modelMark->addColumn("id_purpose",QString::fromUtf8("Буквенное обозначение"),NULL,Rels::instance()->relBukv);
    modelMark->addColumn("descr",QString::fromUtf8("Описание"));
    modelMark->addColumn("vl",QString::fromUtf8("Допустимое содержание влаги"));
    modelMark->addColumn("pr2",QString::fromUtf8("Режим повторной прокалки"));
    modelMark->addColumn("id_u",QString::fromUtf8("Индекс сортировки"),new QIntValidator(this));
    modelMark->addColumn("katalog",QString::fromUtf8("Каталог"));
    modelMark->addColumn("descr_spec",QString::fromUtf8("Особые свойства"));
    modelMark->addColumn("descr_feature",QString::fromUtf8("Особенности сварки"));
    modelMark->setSort("elrtr.marka");
    modelMark->setFilter("elrtr.id<>0");
    modelMark->select();
    ui->tableViewMark->setModel(modelMark);
    ui->tableViewMark->setColumnHidden(0,true);
    ui->tableViewMark->setColumnWidth(1,140);
    for (int i=2; i<ui->tableViewMark->model()->columnCount(); i++){
        ui->tableViewMark->setColumnHidden(i,true);
    }

    mapper = new DbMapper(ui->tableViewMark,this);

    mapper->setItemDelegate(new CustomDelegate(this));

    ui->horizontalLayoutMap->insertWidget(0,mapper);
    mapper->addMapping(ui->lineEditMark,1);
    mapper->addMapping(ui->lineEditMarkSert,2);
    mapper->addMapping(ui->comboBoxGroup,3);
    mapper->addMapping(ui->comboBoxVid,4);
    mapper->addMapping(ui->comboBoxProv,5);
    mapper->addMapping(ui->comboBoxPol,6);
    mapper->addMapping(ui->comboBoxGost,7);
    mapper->addMapping(ui->comboBoxISO,8);
    mapper->addMapping(ui->comboBoxAWS,9);
    mapper->addMapping(ui->comboBoxZnam,10);
    mapper->addMapping(ui->comboBoxBukv,11);
    mapper->addMapping(ui->plainTextEditDescrCat,12);
    mapper->addMapping(ui->lineEditVl,13);
    mapper->addMapping(ui->lineEditPr,14);
    mapper->addMapping(ui->lineEditInd,15);
    mapper->addMapping(ui->checkBoxKat,16);
    mapper->addMapping(ui->plainTextEditDescrSpec,17);
    mapper->addMapping(ui->plainTextEditDescrFeature,18);

    mapper->addEmptyLock(ui->tableViewAmp);
    mapper->addEmptyLock(ui->tableViewChem);
    mapper->addEmptyLock(ui->tableViewMech);
    mapper->addEmptyLock(ui->tableViewPlav);
    mapper->addEmptyLock(ui->groupBoxVar);
    mapper->addEmptyLock(ui->cmdExt);
    mapper->addEmptyLock(ui->cmdLbl);
    mapper->addEmptyLock(ui->cmdLblSmall);
    mapper->addEmptyLock(ui->cmdLblSmall2);
    mapper->addEmptyLock(ui->tableViewPack);

    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshCont(int)));
    connect(modelMark,SIGNAL(sigUpd()),Rels::instance()->relElMark,SLOT(refreshModel()));
    connect(ui->cmdLbl,SIGNAL(clicked(bool)),this,SLOT(gelLbl()));
    connect(ui->cmdLblSmall,SIGNAL(clicked(bool)),this,SLOT(gelLblSmall()));
    connect(ui->cmdLblSmall2,SIGNAL(clicked(bool)),this,SLOT(gelLblSmall2()));
    connect(ui->cmdExt,SIGNAL(clicked(bool)),this,SLOT(exportXml()));
    connect(ui->pushButtonCreVar,SIGNAL(clicked(bool)),this,SLOT(createVar()));
    connect(ui->pushButtonDelVar,SIGNAL(clicked(bool)),this,SLOT(deleteVar()));
    connect(ui->comboBoxVar,SIGNAL(currentIndexChanged(int)),this,SLOT(loadVars()));
    connect(ui->plainTextEditDescr,SIGNAL(textChanged()),this,SLOT(varChanged()));
    connect(ui->lineEditVarZnam,SIGNAL(textChanged(QString)),this,SLOT(varChanged()));
    connect(ui->lineEditProcVar,SIGNAL(textChanged(QString)),this,SLOT(varChanged()));
    connect(ui->comboBoxProvVar,SIGNAL(currentIndexChanged(int)),this,SLOT(varChanged()));
    connect(ui->pushButtonSaveVar,SIGNAL(clicked(bool)),this,SLOT(saveVar()));

    if (ui->tableViewMark->model()->rowCount()){
        ui->tableViewMark->selectRow(0);
    }

}

FormMark::~FormMark()
{
    savesettings();
    delete ui;
}

void FormMark::loadsettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    ui->splitter->restoreState(settings.value("mark_splitter").toByteArray());
}

void FormMark::savesettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("mark_splitter", ui->splitter->saveState());
}

int FormMark::id_el()
{
    QModelIndex ind=ui->tableViewMark->model()->index(mapper->currentIndex(),0);
    return ui->tableViewMark->model()->data(ind,Qt::EditRole).toInt();
}

int FormMark::id_diam()
{
    QModelIndex ind_diam=ui->comboBoxDiam->model()->index(ui->comboBoxDiam->currentIndex(),0);
    return ui->comboBoxDiam->model()->data(ind_diam,Qt::EditRole).toInt();
}

int FormMark::id_var()
{
    return ui->comboBoxVar->model()->data(ui->comboBoxVar->model()->index(ui->comboBoxVar->currentIndex(),0),Qt::EditRole).toInt();
}

void FormMark::loadVars()
{
    int id_e=id_el();
    int id_v=id_var();
    QSqlQuery query;
    query.prepare("select znam, descr, proc, id_prov from el_var where id_el = :id_el and id_var=:id_var");
    query.bindValue(":id_el",id_e);
    query.bindValue(":id_var",id_v);
    if (query.exec()){
        if (query.size()>0){
            query.next();
            blockVar(false);
            ui->lineEditVarZnam->setText(query.value(0).toString());
            ui->plainTextEditDescr->setPlainText(query.value(1).toString());
            ui->lineEditProcVar->setText(query.value(2).toString());
            int id_prov=query.value(3).toInt();
            for (int i=0; i<ui->comboBoxProvVar->model()->rowCount();i++){
                int id_p=ui->comboBoxProvVar->model()->data(ui->comboBoxProvVar->model()->index(i,0),Qt::EditRole).toInt();
                if (id_p==id_prov){
                    ui->comboBoxProvVar->setCurrentIndex(i);
                    break;
                }
            }
        } else {
            blockVar(true);
        }
    } else {
        QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Cancel);
    }
    ui->pushButtonSaveVar->setEnabled(false);

    modelAmp->setDefaultValue(1,id_e);
    modelAmp->setDefaultValue(2,id_v);
    modelAmp->setFilter("amp.id_el = "+QString::number(id_e)+" and amp.id_var = "+QString::number(id_v));
    modelAmp->select();
}

void FormMark::refreshCont(int index)
{
    QModelIndex ind=ui->tableViewMark->model()->index(index,0);
    int id_el=ui->tableViewMark->model()->data(ind,Qt::EditRole).toInt();

    modelEan->refresh(id_el);

    modelChemTu->setDefaultValue(0,id_el);
    modelChemTu->setFilter("chem_tu.id_el = "+QString::number(id_el));
    modelChemTu->select();

    modelMechTu->setDefaultValue(0,id_el);
    modelMechTu->setFilter("mech_tu.id_el = "+QString::number(id_el));
    modelMechTu->select();

    modelPlav->setDefaultValue(0,id_el);
    modelPlav->setFilter("el_plav.id_el = "+QString::number(id_el));
    modelPlav->select();
    updImg();
    loadVars();
}

void FormMark::updImg()
{
    QModelIndex ind=ui->tableViewMark->model()->index(mapper->currentIndex(),6);
    int id_pix=ui->tableViewMark->model()->data(ind,Qt::EditRole).toInt();
    QPixmap pix;
    pix.loadFromData(Rels::instance()->relPosPix->data(QString::number(id_pix)).toByteArray());
    if (pix.isNull()){
        ui->labelPix->setPixmap(pix);
    } else {
        ui->labelPix->setPixmap(pix.scaled(ui->labelPix->size(),Qt::KeepAspectRatio));
    }
}

void FormMark::gelLbl()
{
    LblCreator c;
    c.createLbl(id_el(),id_diam(),QString(),ui->dateEdit->date(),false,ui->checkBoxOrder->isChecked(),id_var());
}

void FormMark::gelLblSmall()
{
    LblCreator c;
    c.createLblGlabels(id_el(),id_diam(),QString(),ui->dateEdit->date(),id_var());
}

void FormMark::gelLblSmall2()
{
    LblCreator c;
    c.createLblGlabels2(id_el(),id_diam(),QString(),ui->dateEdit->date(),id_var());
}

void FormMark::exportXml()
{
    createXml();
}

void FormMark::createXml()
{
    Export *e = new Export;
    connect(e,SIGNAL(finished()),e,SLOT(deleteLater()));
    e->start();
}

void FormMark::blockVar(bool b)
{
    ui->pushButtonCreVar->setEnabled(b);
    ui->pushButtonDelVar->setEnabled(!b);

    ui->lineEditVarZnam->setEnabled(!b);
    ui->plainTextEditDescr->setEnabled(!b);
    ui->lineEditProcVar->setEnabled(!b);
    ui->comboBoxProvVar->setEnabled(!b);
    if (b){
        ui->lineEditVarZnam->clear();
        ui->plainTextEditDescr->clear();
        ui->lineEditProcVar->clear();
        ui->comboBoxProvVar->setCurrentIndex(0);
    }
}

void FormMark::createVar()
{
    QString znam=ui->comboBoxZnam->currentText();
    int id_prov=ui->comboBoxProv->model()->data(ui->comboBoxProv->model()->index(ui->comboBoxProv->currentIndex(),0),Qt::EditRole).toInt();

    QSqlQuery query;
    query.prepare("insert into el_var (id_el, id_var, znam, descr, proc, id_prov) values (:id_el, :id_var, :znam, (select e.descr from elrtr as e where e.id = :id ), :proc, :id_prov)");
    query.bindValue(":id_el",id_el());
    query.bindValue(":id_var",id_var());
    query.bindValue(":znam",znam);
    query.bindValue(":id",id_el());
    query.bindValue(":proc",ui->lineEditPr->text());
    query.bindValue(":id_prov",id_prov);
    if (!query.exec()){
        QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Cancel);
    }

    if (modelAmp->isAdd() && modelAmp->rowCount()==1){
        QSqlQuery queryAmp;
        queryAmp.prepare("insert into amp (id_el, id_diam, bot, vert, ceil, id_var) "
                         "(select id_el, id_diam, bot, vert, ceil, :id_var from amp where id_el = :id_el and id_var = 1)");
        queryAmp.bindValue(":id_el",id_el());
        queryAmp.bindValue(":id_var",id_var());
        if (!queryAmp.exec()){
            QMessageBox::critical(this,tr("Ошибка"),queryAmp.lastError().text(),QMessageBox::Cancel);
        }
    }

    loadVars();
}

void FormMark::saveVar()
{
    QString znam=ui->lineEditVarZnam->text();
    QString descr=ui->plainTextEditDescr->toPlainText();

    int id_prov=ui->comboBoxProvVar->model()->data(ui->comboBoxProvVar->model()->index(ui->comboBoxProvVar->currentIndex(),0),Qt::EditRole).toInt();

    QSqlQuery query;
    query.prepare("update el_var set znam = :znam, descr = :descr, proc = :proc, id_prov = :id_prov where id_el = :id_el and id_var = :id_var");
    query.bindValue(":id_el",id_el());
    query.bindValue(":id_var",id_var());
    query.bindValue(":znam",znam);
    query.bindValue(":descr",descr);
    query.bindValue(":proc",ui->lineEditProcVar->text());
    query.bindValue(":id_prov",id_prov);
    if (query.exec()){
        loadVars();
    } else {
        QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Cancel);
    }
}

void FormMark::deleteVar()
{
    int n=QMessageBox::question(this,tr("Подтвердите удаление"),tr("Удалить вариант \"")+ui->comboBoxVar->currentText()+"\"",QMessageBox::Yes,QMessageBox::No);
    if (n==QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("delete from el_var where id_el = :id_el and id_var = :id_var");
        query.bindValue(":id_el",id_el());
        query.bindValue(":id_var",id_var());
        if (query.exec()){
            loadVars();
        } else {
            QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Cancel);
        }
    }
}

void FormMark::varChanged()
{
    ui->pushButtonSaveVar->setEnabled(true);
}

CustomDelegate::CustomDelegate(QObject *parent) : DbDelegate(parent)
{

}

void CustomDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column()==14){
        QLineEdit *line = qobject_cast<QLineEdit *>(editor);
        if (line){
            QString pr=index.model()->data(index,Qt::EditRole).toString();
            QStringList list=pr.split(":");
            if (list.size()==4){
                line->setText(QString("%1±%2°C %3 %4").arg(list.at(0)).arg(list.at(1)).arg(list.at(2)).arg(list.at(3)));
            } else {
                line->clear();
            }
        }

    } else {
        DbDelegate::setEditorData(editor,index);
    }
}

void CustomDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column()==14){
        QLineEdit *line = qobject_cast<QLineEdit *>(editor);
        if (line){
            QString text=line->text();
            QRegExp reg("^\\s*(\\d+)\\s*±\\s*(\\d+)\\s*°C\\s+(\\d+)\\s+(\\w+)");
            QString s;
            if (reg.indexIn(text)!=-1){
                for (int i=1; i<=reg.captureCount(); i++){
                    if(!s.isEmpty()){
                        s+=":";
                    }
                    s+=reg.cap(i);
                }
            }
            model->setData(index,s);
        }
    } else {
        DbDelegate::setModelData(editor,model,index);
    }
}
