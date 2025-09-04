#include "pasportmanager.h"

PasportManager::PasportManager(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onResult(QNetworkReply*)));
}

void PasportManager::getPasport(int id_part, QString filename, int year, const QByteArray &json)
{
    QUrl url(Rels::instance()->appServer()+"/pasport/elrtr/"+QString::number(id_part));
    QNetworkRequest request;
    request.setRawHeader("Accept","application/json");
    request.setRawHeader("Accept-Charset", "UTF-8");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("User-Agent", "Appszsm");
    request.setUrl(url);
    QNetworkReply *reply = manager->post(request,json);
    reply->setProperty("filename",filename);
    reply->setProperty("year",year);
}

void PasportManager::onResult(QNetworkReply *reply)
{
    if (reply->error()){
        QMessageBox::critical(nullptr,tr("Ошибка"),reply->errorString()+"\n"+reply->readAll(),QMessageBox::Cancel);
    } else {
        QString filename=reply->property("filename").toString();
        QString year=reply->property("year").isNull() ? QString::number(QDate::currentDate().year()) : reply->property("year").toString();
        if (filename.isEmpty()){
            filename=tr("temp.pdf");
        }

        QDir dir(QDir::homePath()+tr("/Паспорта"));
        if (!dir.exists()) dir.mkdir(dir.path());
        dir.setPath(dir.path()+"/"+year);
        if (!dir.exists()) dir.mkdir(dir.path());
        filename=filename.replace(QRegularExpression("[^\\w]",QRegularExpression::UseUnicodePropertiesOption), "_");
        QString totalName=dir.path()+"/"+filename+".pdf";
        QFile file(totalName);
        if (file.open(QIODevice::WriteOnly)){
            file.write(reply->readAll());
            file.close();
            QFileInfo fileInfo(file);
            QDesktopServices::openUrl((QUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()))));
        }
    }
    reply->deleteLater();
}
