#include "formtypes.h"
#include "ui_formtypes.h"

FormTypes::FormTypes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTypes)
{
    ui->setupUi(this);

    ui->tableViewVar->setModel(Rels::instance()->modelVar);
    ui->tableViewVar->setColumnHidden(0,true);
    ui->tableViewVar->setColumnWidth(1,200);

    ui->tableViewGost->setModel(Rels::instance()->modelGost);
    ui->tableViewGost->setColumnHidden(0,true);
    ui->tableViewGost->setColumnWidth(1,200);

    ui->tableViewBukv->setModel(Rels::instance()->modelBukv);
    ui->tableViewBukv->setColumnHidden(0,true);
    ui->tableViewBukv->setColumnWidth(1,200);

    ui->tableViewZnam->setModel(Rels::instance()->modelZnam);
    ui->tableViewZnam->setColumnHidden(0,true);
    ui->tableViewZnam->setColumnWidth(1,200);

    ui->tableViewIso->setModel(Rels::instance()->modelIso);
    ui->tableViewIso->setColumnHidden(0,true);
    ui->tableViewIso->setColumnWidth(1,200);

    ui->tableViewAws->setModel(Rels::instance()->modelAws);
    ui->tableViewAws->setColumnHidden(0,true);
    ui->tableViewAws->setColumnWidth(1,200);
}

FormTypes::~FormTypes()
{
    delete ui;
}
