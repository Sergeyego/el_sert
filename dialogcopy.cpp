#include "dialogcopy.h"
#include "ui_dialogcopy.h"

DialogCopy::DialogCopy(int id_p, QWidget *parent) :
    QDialog(parent), id_partsrc(id_p),
    ui(new Ui::DialogCopy)
{
    ui->setupUi(this);

    modelPart = new QSqlQueryModel(this);
    ui->tableViewPart->setModel(modelPart);

    modelChem = new ModelChemSert(this);
    modelMech = new ModelMechSert(this);

    ui->tableViewChem->setModel(modelChem);
    ui->tableViewChem->setColumnHidden(0,true);
    ui->tableViewChem->setColumnWidth(1,80);
    ui->tableViewChem->setColumnWidth(2,80);

    ui->tableViewMech->setModel(modelMech);
    ui->tableViewMech->setColumnHidden(0,true);
    ui->tableViewMech->setColumnWidth(1,200);
    ui->tableViewMech->setColumnWidth(2,80);

    setQueryPart();

    connect(ui->tableViewPart,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
}

DialogCopy::~DialogCopy()
{
    delete ui;
}

int DialogCopy::idPart()
{
    int id=-1;
    if (modelPart->rowCount()){
        id = modelPart->data(modelPart->index(ui->tableViewPart->currentIndex().row(),0),Qt::EditRole).toInt();
    }
    return id;
}

void DialogCopy::setQueryPart()
{
    QSqlQuery query;
    query.prepare("select pp.id_part, p.n_s, p.dat_part, e.marka||' ф '||p.diam "
                  "from parti_perepack pp "
                  "inner join parti p on p.id = pp.id_part "
                  "inner join elrtr e on e.id = p.id_el "
                  "where pp.id_new_part = :id");
    query.bindValue(":id",id_partsrc);
    if (query.exec()){
        modelPart->setQuery(query);
        modelPart->setHeaderData(1,Qt::Horizontal,tr("Номер"));
        modelPart->setHeaderData(2,Qt::Horizontal,tr("Дата"));
        modelPart->setHeaderData(3,Qt::Horizontal,tr("Марка"));
        frmPart();
    } else {
        QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }
}

void DialogCopy::frmPart()
{
    connect(ui->tableViewPart->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(updData(QModelIndex)));
    if (modelPart->rowCount()){
        ui->tableViewPart->selectRow(0);
    }
    ui->tableViewPart->setColumnHidden(0,true);
    ui->tableViewPart->resizeColumnsToContents();
}

void DialogCopy::updData(QModelIndex index)
{
    int id = modelPart->data(modelPart->index(index.row(),0),Qt::EditRole).toInt();
    modelChem->refresh(id);
    modelMech->refresh(id);
}
