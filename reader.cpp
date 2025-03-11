#include "reader.h"
#include "ui_reader.h"

Reader::Reader(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Reader)
{
    ui->setupUi(this);
    id_ship=-1;
    setLock(true);

    manager = new QNetworkAccessManager(this);

    connect(ui->radioButtonRus,SIGNAL(clicked(bool)),this,SLOT(reload()));
    connect(ui->radioButtonEng,SIGNAL(clicked(bool)),this,SLOT(reload()));
    connect(ui->radioButtonMix,SIGNAL(clicked(bool)),this,SLOT(reload()));

    connect(ui->pushButtonPrint,SIGNAL(clicked(bool)),this,SLOT(print()));
    connect(ui->pushButtonPDF,SIGNAL(clicked(bool)),this,SLOT(save()));
    connect(ui->spinBoxScale,SIGNAL(valueChanged(int)),this,SLOT(reload()));
}

Reader::~Reader()
{
    delete ui;
}



void Reader::setCurrentIdShip(int id, QString fname, QString prefix)
{
    id_ship=id;
    name=fname;
    spref=prefix;
    reload();
}

QString Reader::getCurrentLang()
{
    QString lang="mix";
    if (ui->radioButtonRus->isChecked()){
        lang="ru";
    } else if (ui->radioButtonEng->isChecked()){
        lang="en";
    }
    return lang;
}

void Reader::reload()
{
    setLock(true);
    double scale=ui->spinBoxScale->value()/100.0;
    int dpi=QApplication::desktop()->physicalDpiX()*scale;
    QNetworkRequest request(QUrl::fromUserInput(Rels::instance()->appServer()+"/s3/img/"+spref+"/"+QString::number(id_ship)+"/"+getCurrentLang()+"/"+QString::number(dpi)));
    request.setRawHeader("Accept-Charset", "UTF-8");
    request.setRawHeader("User-Agent", "Appszsm");
    QNetworkReply *reply;
    reply=manager->get(request);
    connect(reply,SIGNAL(finished()),this,SLOT(replyFinished()));
}

void Reader::setLang(QString lang)
{
    if (lang=="ru"){
        ui->radioButtonRus->setChecked(true);
    } else if (lang=="en"){
        ui->radioButtonEng->setChecked(true);
    } else {
        ui->radioButtonMix->setChecked(true);
    }
}

void Reader::clear()
{
    ui->label->clear();
    id_ship=-1;
}

void Reader::print()
{
    QByteArray data;
    QProgressDialog* pprd = new QProgressDialog(tr("Подготовка к печати..."),"", 0, 1, this);
    pprd->setCancelButton(0);
    pprd->setMinimumDuration(0);
    pprd->setWindowTitle(tr("Пожалуйста, подождите"));
    pprd->setValue(0);
    bool ok = HttpSyncManager::sendGet(Rels::instance()->appServer()+"/s3/img/"+spref+"/"+QString::number(id_ship)+"/"+getCurrentLang()+"/300",data);
    QCoreApplication::processEvents();
    pprd->setValue(1);
    if (ok && data.size()){
        QPrinter printer(QPrinter::HighResolution);
        delete pprd;
        QPrintDialog printDialog(&printer, this);
        if (printDialog.exec()) {
            QPainter painter(&printer);
            QImage img = QImage::fromData(data,"png");
            painter.drawImage(painter.window(),img);
        }
    }
}

void Reader::save()
{
    QByteArray data;
    bool ok = HttpSyncManager::sendGet(Rels::instance()->appServer()+"/s3/local/"+spref+"/"+QString::number(id_ship)+"/"+getCurrentLang(),data);
    if (!ok || !data.size()){
        return;
    }
    QSettings settings("szsm", QApplication::applicationName());
    QDir dir(settings.value("sertPath",QDir::homePath()).toString());
    QString exportname = QFileDialog::getSaveFileName(this,tr("Сохранить PDF"),dir.path()+"/"+name+".pdf", "*.pdf");
    if (!exportname.isEmpty()) {
        QFile file(exportname);
        QFileInfo info(file);
        settings.setValue("sertPath",info.path());
        if (file.open(QFile::WriteOnly)){
            file.write(data);
            file.close();
        }
    }
}

void Reader::replyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply){
        QByteArray data=reply->readAll();
        bool ok=(reply->error()==QNetworkReply::NoError);
        if (!ok){
            QMessageBox::critical(nullptr,tr("Ошибка"),reply->errorString()+"\n"+data,QMessageBox::Cancel);
        } else {
            if (data.size()){
                QPixmap pixmap;
                pixmap.loadFromData(data,"png");
                ui->label->setPixmap(pixmap);
                setLock(false);
            } else {
                ui->label->clear();
            }}
        reply->deleteLater();
    }
}

void Reader::setLock(bool b)
{
    ui->pushButtonPDF->setDisabled(b);
    ui->pushButtonPrint->setDisabled(b);
}
