#include "dialogsignature.h"
#include "ui_dialogsignature.h"

DialogSignature::DialogSignature(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSignature)
{
    ui->setupUi(this);
    connect(ui->comboBoxCert,SIGNAL(currentIndexChanged(int)),this,SLOT(setCurrentCert(int)));
}

DialogSignature::~DialogSignature()
{
    delete ui;
}

int DialogSignature::exec()
{
    QByteArray data;
    bool ok=HttpSyncManager::sendGet("/certificates",data);
    if (ok){
        QJsonDocument doc;
        doc=QJsonDocument::fromJson(data);
        if (doc.isArray()){
            certs=doc.array();
            if (certs.size()){
                for (QJsonValue v : certs){
                    ui->comboBoxCert->addItem(v.toObject().value("SurName").toString()+" "+v.toObject().value("GivenName").toString());
                }
                setCurrentCert(0);
            }
        }
    } else {
        return QDialog::Rejected;
    }
    return QDialog::exec();
}

QString DialogSignature::getSN()
{
    return ui->lineEditSN->text();
}

void DialogSignature::setCurrentCert(int i)
{
    QJsonObject v=certs.at(i).toObject();
    ui->lineEditSN->setText(v.value("SerialNumber").toString());
    ui->lineEditSub->setText(v.value("SubjectName").toString());
    ui->lineEditTitle->setText(v.value("Title").toString());
    ui->lineEditPer->setText(QString("С %1 По %2").arg(v.value("From").toVariant().toDate().toString("dd.MM.yyyy")).arg(v.value("To").toVariant().toDate().toString("dd.MM.yyyy")));
}
