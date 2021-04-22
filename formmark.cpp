#include "formmark.h"
#include "ui_formmark.h"

FormMark::FormMark(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMark)
{
    ui->setupUi(this);
    loadsettings();

    ui->lineEditPr->setInputMask(QString("999±99°C 9 часxx"));
    ui->dateEdit->setDate(QDate::currentDate());

    ui->comboBoxDiam->setModel(Rels::instance()->relDiam->proxyModel());
    ui->comboBoxDiam->setModelColumn(1);
    int ind=ui->comboBoxDiam->findText("3.0 ");
    if (ind>=0){
        ui->comboBoxDiam->setCurrentIndex(ind);
    }

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
    modelAmp->addColumn("id_diam",QString::fromUtf8("Диаметр, мм"),NULL,Rels::instance()->relDiam);
    modelAmp->addColumn("bot",QString::fromUtf8("Нижнее"));
    modelAmp->addColumn("vert",QString::fromUtf8("Вертикальное"));
    modelAmp->addColumn("ceil",QString::fromUtf8("Потолочное"));
    modelAmp->setSuffix("inner join diam on amp.id_diam=diam.id");
    modelAmp->setSort("diam.sdim");

    ui->tableViewAmp->setModel(modelAmp);
    ui->tableViewAmp->setColumnHidden(0,true);
    ui->tableViewAmp->setColumnHidden(1,true);
    ui->tableViewAmp->setColumnWidth(2,100);
    ui->tableViewAmp->setColumnWidth(3,100);
    ui->tableViewAmp->setColumnWidth(4,100);
    ui->tableViewAmp->setColumnWidth(5,100);

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
    modelMark->addColumn("pr",QString::fromUtf8("Режим повторной прокалки"));
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
    mapper->addMapping(ui->plainTextEditDescr,12);
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
    mapper->addEmptyLock(ui->cmdLbl);
    mapper->addEmptyLock(ui->cmdExt);

    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshCont(int)));
    connect(modelMark,SIGNAL(sigUpd()),Rels::instance()->relElMark,SLOT(refreshModel()));
    connect(ui->cmdLbl,SIGNAL(clicked(bool)),this,SLOT(gelLbl()));
    connect(ui->cmdLblSmall,SIGNAL(clicked(bool)),this,SLOT(gelLblSmall()));
    connect(ui->cmdExt,SIGNAL(clicked(bool)),this,SLOT(exportXml()));

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

void FormMark::refreshCont(int index)
{
    QModelIndex ind=ui->tableViewMark->model()->index(index,0);
    int id_el=ui->tableViewMark->model()->data(ind,Qt::EditRole).toInt();

    modelEan->refresh(id_el);

    modelAmp->setDefaultValue(1,id_el);
    modelAmp->setFilter("amp.id_el = "+QString::number(id_el));
    modelAmp->select();

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
    QModelIndex ind=ui->tableViewMark->model()->index(mapper->currentIndex(),0);
    int id_el=ui->tableViewMark->model()->data(ind,Qt::EditRole).toInt();
    QModelIndex ind_diam=ui->comboBoxDiam->model()->index(ui->comboBoxDiam->currentIndex(),0);
    int id_diam=ui->comboBoxDiam->model()->data(ind_diam,Qt::EditRole).toInt();
    LblCreator c;
    c.createLbl(id_el,id_diam,QString(),ui->dateEdit->date(),ui->checkBoxAmp->isChecked(),ui->checkBoxOrder->isChecked());
}

void FormMark::gelLblSmall()
{
    QModelIndex ind=ui->tableViewMark->model()->index(mapper->currentIndex(),0);
    int id_el=ui->tableViewMark->model()->data(ind,Qt::EditRole).toInt();
    QModelIndex ind_diam=ui->comboBoxDiam->model()->index(ui->comboBoxDiam->currentIndex(),0);
    int id_diam=ui->comboBoxDiam->model()->data(ind_diam,Qt::EditRole).toInt();
    LblCreator c;
    c.createLblGlabels(id_el,id_diam,QString(),ui->dateEdit->date());
}

void FormMark::exportXml()
{
    PdfToImg *conv = new PdfToImg;
    connect(conv,&PdfToImg::finished,[this,conv]() {
        conv->deleteLater();
        Export *e = new Export;
        connect(e,SIGNAL(finished()),e,SLOT(deleteLater()));
        e->start();
    });
    conv->start();
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
            QString temp=pr.left(3);
            QString dop=pr.mid(3,2);
            QString ch=pr.mid(5,1);
            QString ok=pr.mid(6,2);
            line->setText(QString("%1±%2°C %3 час%4").arg(temp).arg(dop).arg(ch).arg(ok));
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
            QRegExp reg("^(\\d{3})±(\\d{2})°C (\\d) час(\\w*)");
            QString s;
            if (reg.indexIn(text)!=-1){
                for (int i=1; i<=reg.captureCount(); i++){
                    s+=reg.cap(i);
                }
            }
            model->setData(index,s);
        }
    } else {
        DbDelegate::setModelData(editor,model,index);
    }
}
