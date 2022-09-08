#include "formnormdoc.h"
#include "ui_formnormdoc.h"

FormNormDoc::FormNormDoc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormNormDoc)
{
    ui->setupUi(this);

    sertModel = new QSqlQueryModel(this);
    ui->tableViewSert->setModel(sertModel);

    QString rsrcPath=":/icons";
    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(rsrcPath + "/editcopy.png"));
    ui->cmdCopy->setIcon(copyIcon);
    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(rsrcPath + "/editpaste.png"));
    ui->cmdPaste->setIcon(pasteIcon);

    loadSettings();

    ideTu=-1;

    ui->tableViewListGost->setModel(Rels::instance()->modelListGost);
    ui->tableViewListGost->setColumnHidden(0,true);
    ui->tableViewListGost->setColumnWidth(1,210);

    modelGostEl = new DbTableModel("gost_elnew",this);
    modelGostEl->addColumn("ide",tr("Марка"));
    modelGostEl->addColumn("id_var",tr("Вариант"),NULL,Rels::instance()->relVar);
    modelGostEl->addColumn("dat",tr("Дата начала действия"));
    modelGostEl->addColumn("id_gost",tr("Документ"),NULL,Rels::instance()->relGost);
    modelGostEl->addColumn("dat_end",tr("Дата окончания действия"));
    modelGostEl->setSort("gost_elnew.id_var, gost_elnew.dat, gost_elnew.id_gost");
    modelGostEl->setDefaultValue(4,QVariant());
    ui->tableViewGostEl->setModel(modelGostEl);
    ui->tableViewGostEl->setColumnHidden(0,true);
    ui->tableViewGostEl->setColumnWidth(1,200);
    ui->tableViewGostEl->setColumnWidth(2,200);
    ui->tableViewGostEl->setColumnWidth(3,230);
    ui->tableViewGostEl->setColumnWidth(4,200);

    if (Rels::instance()->relElDim->model()->columnCount()>1){
        Rels::instance()->relElDim->model()->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("Марка"));
    }

    ui->tableViewMark->verticalHeader()->setDefaultSectionSize(ui->tableViewMark->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewMark->verticalHeader()->hide();
    ui->tableViewMark->setModel(Rels::instance()->relElDim->model());
    ui->tableViewMark->setColumnHidden(0,true);
    ui->tableViewMark->setColumnWidth(1,200);

    connect(ui->tableViewMark->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(refreshGostEl(QModelIndex)));
    connect(ui->cmdCopy,SIGNAL(clicked(bool)),this,SLOT(copyTu()));
    connect(ui->cmdPaste,SIGNAL(clicked(bool)),this,SLOT(pasteTu()));
    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(updElDim()));
}

FormNormDoc::~FormNormDoc()
{
    saveSettings();
    delete ui;
}

void FormNormDoc::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->ui->splitterTu->restoreState(settings.value("tu_splitter_width").toByteArray());
}

void FormNormDoc::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("tu_splitter_width",ui->splitterTu->saveState());
}

void FormNormDoc::refreshGostEl(QModelIndex index)
{
    int ide=ui->tableViewMark->model()->data(ui->tableViewMark->model()->index(index.row(),0),Qt::EditRole).toInt();
    modelGostEl->setFilter("gost_elnew.ide = "+QString::number(ide));
    modelGostEl->setDefaultValue(0,ide);
    modelGostEl->select();

    QSqlQuery query;
    query.prepare("select doc_nam, ved_short, nom_doc, grade_nam, dat_doc, dat_end "
                  "from zvd_get_sert(:date, (select id_el from dry_els where ide=:ide1 ), (select id_diam from dry_els where ide=:ide2 ))");
    query.bindValue(":date",QDate::currentDate());
    query.bindValue(":ide1",ide);
    query.bindValue(":ide2",ide);
    if (query.exec()){
        sertModel->setQuery(query);
        ui->tableViewSert->resizeColumnsToContents();
        sertModel->setHeaderData(0,Qt::Horizontal,QString::fromUtf8("Вид документа"));
        sertModel->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("Ведомство"));
        sertModel->setHeaderData(2,Qt::Horizontal,QString::fromUtf8("Номер документа"));
        sertModel->setHeaderData(3,Qt::Horizontal,QString::fromUtf8("Категория"));
        sertModel->setHeaderData(4,Qt::Horizontal,QString::fromUtf8("Дата документа"));
        sertModel->setHeaderData(5,Qt::Horizontal,QString::fromUtf8("Дата оконч. действия"));
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
}

void FormNormDoc::copyTu()
{
    if (ui->tableViewMark->currentIndex().isValid()){
        int r=ui->tableViewMark->currentIndex().row();
        ideTu=ui->tableViewMark->model()->data(ui->tableViewMark->model()->index(r,0),Qt::EditRole).toInt();
        ui->cmdPaste->setEnabled(true);
    } else {
        ui->cmdPaste->setEnabled(false);
    }
}

void FormNormDoc::pasteTu()
{
    if (!ui->tableViewMark->currentIndex().isValid()) return;
    int r=ui->tableViewMark->currentIndex().row();
    int ide=ui->tableViewMark->model()->data(ui->tableViewMark->model()->index(r,0),Qt::EditRole).toInt();
    QSqlQuery query;
    query.prepare("delete from gost_elnew where ide = :ide");
    query.bindValue(":ide",ide);
    if (!query.exec()){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        query.clear();
        query.prepare("insert into gost_elnew (ide, dat, id_gost, dat_end, id_var) "
                      "(select :ideC, n.dat, n.id_gost, n.dat_end, n.id_var from gost_elnew as n where n.ide = :ideN)");
        query.bindValue(":ideC",ide);
        query.bindValue(":ideN",ideTu);
        if (!query.exec()){
            QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
        }
    }
    modelGostEl->select();
}

void FormNormDoc::updElDim()
{
    QSqlQuery query;
    query.prepare("select * from rx_els()");
    if (query.exec()){
        Rels::instance()->relElDim->refreshModel();
    } else {
        QMessageBox::critical(this,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}
