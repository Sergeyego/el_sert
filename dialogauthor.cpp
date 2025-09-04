#include "dialogauthor.h"
#include "ui_dialogauthor.h"

DialogAuthor::DialogAuthor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAuthor)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    insRab("Начальник лаборатории%",ui->lineEditAuthTitle,ui->lineEditAuth);
    insRab("Начальник ОТК%",ui->lineEditCheckTitle,ui->lineEditCheck);
    insRab("Директор по качеству%",ui->lineEditAppTitle,ui->lineEditApp);
    insRab("Техник по стандартизации%",nullptr,ui->lineEditNorm);
}

DialogAuthor::~DialogAuthor()
{
    delete ui;
}

QByteArray DialogAuthor::getJson()
{
    QJsonObject object;
    object.insert("authTitle",ui->lineEditAuthTitle->text());
    object.insert("auth",ui->lineEditAuth->text());
    object.insert("checkTitle",ui->lineEditCheckTitle->text());
    object.insert("check",ui->lineEditCheck->text());
    object.insert("appTitle",ui->lineEditAppTitle->text());
    object.insert("app",ui->lineEditApp->text());
    object.insert("norm",ui->lineEditNorm->text());
    object.insert("date",ui->dateEdit->date().toString("yyyy/MM/dd"));
    QJsonDocument doc(object);
    return doc.toJson(QJsonDocument::Indented);
}

void DialogAuthor::insRab(QString pattern, QLineEdit *title, QLineEdit *name)
{
    QSqlQuery query;
    query.prepare("select tn.nam, kj.nam from tab_number tn "
                  "inner join kamin_job kj on kj.id = tn.id_job "
                  "where kj.nam ilike :pattern");
    query.bindValue(":pattern",pattern);
    if (query.exec()){
        if (query.next()){
            if (title){
                title->setText(query.value(1).toString());
            }
            if (name){
                name->setText(query.value(0).toString());
            }
        }
    } else {
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
}
