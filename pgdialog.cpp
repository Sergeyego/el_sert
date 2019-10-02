#include "pgdialog.h"
#include "ui_pgdialog.h"

PgDialog::PgDialog(const QString title, QString key, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PgDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->edtHost->setText("192.168.1.10");
    ui->groupBox->setVisible(false);
    if (!key.isEmpty()){
        ui->lineEditKey->setText(key);
        ui->lineEditKey->hide();
        ui->labelKey->hide();
    }
    db = QSqlDatabase::addDatabase("QPSQL");
    connect(ui->cmdShowOpt,SIGNAL(clicked(bool)),ui->groupBox,SLOT(setVisible(bool)));
    connect(ui->cmdConnect,SIGNAL(clicked()),this,SLOT(dBconnect()));
}

PgDialog::~PgDialog()
{
    delete ui;
    if (db.isOpen()) db.close();
}

QString PgDialog::key() const
{
    return ui->lineEditKey->text();
}

void PgDialog::dBconnect()
{
    QString key=ui->lineEditKey->text();
    if (key==QString("srt") || key==QString("prod")){
        db.setDatabaseName("neo_rtx");
        db.setHostName(ui->edtHost->text());
        db.setPort(ui->edtPort->text().toInt());
        db.setUserName(ui->edtUser->text());
        db.setPassword(ui->edtPasswd->text());
        if (!db.open()) {
            QMessageBox::critical(NULL,"Error",db.lastError().text(),QMessageBox::Ok);
        } else this->accept();
    } else {
        QMessageBox::critical(NULL,"Error",tr("Неверный ключ!"),QMessageBox::Ok);
    }
}
