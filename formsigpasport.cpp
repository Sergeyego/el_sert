#include "formsigpasport.h"
#include "ui_formsigpasport.h"

FormSigPasport::FormSigPasport(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormSigPasport)
{
    ui->setupUi(this);
    modelSig = new DbTableModel("pasport_sig",this);
    modelSig->addColumn("id_role",tr("Роль"),Rels::instance()->relPasportRole);
    modelSig->addColumn("id_empl",tr("Подписант"),Rels::instance()->relEmpl);
    modelSig->addColumn("dat",tr("Дата"));
    //modelSig->addColumn("sig",tr("Подпись"));
    modelSig->setSort("pasport_sig.id_role, pasport_sig.dat");
    modelSig->select();
    ui->tableView->setModel(modelSig);
    ui->tableView->setColumnWidth(0,110);
    ui->tableView->setColumnWidth(1,400);
    ui->tableView->setColumnWidth(2,90);
    //ui->tableView->setColumnWidth(3,150);
}

FormSigPasport::~FormSigPasport()
{
    delete ui;
}
