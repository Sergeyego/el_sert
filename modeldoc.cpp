#include "modeldoc.h"


ModelDoc::ModelDoc(QObject *parent) : DbTableModel("zvd_sert",parent)
{
    manager = new QNetworkAccessManager(this);

    addColumn("id","id");
    addColumn("nom_doc",QString::fromUtf8("Номер документа"));
    addColumn("nazv",QString::fromUtf8("Название документа"));
    addColumn("dat_doc",QString::fromUtf8("Дата документа"));
    addColumn("dat_beg",QString::fromUtf8("Дата начала"));
    addColumn("dat_end",QString::fromUtf8("Дата конца"));
    addColumn("nom_bl",QString::fromUtf8("Номер бланка"));
    addColumn("nom_sl",QString::fromUtf8("Номер наклейки"));
    addColumn("txt",QString::fromUtf8("Текст документа"));
    addColumn("gr_tech_ust",QString::fromUtf8("ГТУ"));
    addColumn("id_ved",QString::fromUtf8("Ведомство"),Rels::instance()->relVed);
    addColumn("id_doc",QString::fromUtf8("Тип документа"),Rels::instance()->relVidDoc);
    addColumn("en",QString::fromUtf8("Включен по умолчанию"));

    setDefaultValue(12,true);

    setSort("zvd_sert.nom_doc, zvd_sert.dat_doc");

    connect(this,SIGNAL(sigUpd()),this,SLOT(updateList()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(updateList()));
}

QVariant ModelDoc::data(const QModelIndex &index, int role) const
{
    if (role==Qt::BackgroundRole){
        if (fileExist(index.row()) && isActive(index.row())){
            return QVariant(QColor(170,255,170));
        } else {
            return QVariant(QColor(255,170,170));
        }
    }
    return DbTableModel::data(index,role);
}

bool ModelDoc::fileExist(int ind) const
{
    QString nom=this->data(this->index(ind,1),Qt::EditRole).toString();
    docInfo def;
    def.id="-1";
    def.file="";
    return docMap.contains(nom) && !docMap.value(nom,def).file.isEmpty();
}

bool ModelDoc::isActive(int ind) const
{
    QDate current=QDate::currentDate();
    QDate beg=data(index(ind,4),Qt::EditRole).toDate();
    QDate end=data(index(ind,5),Qt::EditRole).isNull()? current.addDays(1) : data(index(ind,5),Qt::EditRole).toDate();
    return (current>=beg && current<=end);
}

void ModelDoc::refresh(bool activeOnly)
{
    if (activeOnly){
        setFilter("COALESCE(zvd_sert.dat_end,'3000-01-01'::date) >= '"+QDate::currentDate().toString("yyyy-MM-dd")+"'");
    } else {
        setFilter("");
    }
    select();
}

void ModelDoc::viewDoc(int ind)
{
    if (fileExist(ind)){
        QString nom=this->data(this->index(ind,1),Qt::EditRole).toString();
        QNetworkRequest request(QUrl::fromUserInput(docMap.value(nom).file));
        request.setRawHeader("Accept-Charset", "UTF-8");
        request.setRawHeader("User-Agent", "Appszsm");
        QNetworkReply *reply;
        reply=manager->get(request);
        connect(reply,SIGNAL(finished()),this,SLOT(replyViewFinished()));
    }
}

void ModelDoc::saveAs(int ind)
{
    if (fileExist(ind)){
        QString nom=this->data(this->index(ind,1),Qt::EditRole).toString();
        QNetworkRequest request(QUrl::fromUserInput(docMap.value(nom).file));
        request.setRawHeader("Accept-Charset", "UTF-8");
        request.setRawHeader("User-Agent", "Appszsm");
        QNetworkReply *reply;
        reply=manager->get(request);
        reply->setProperty("index",ind);
        connect(reply,SIGNAL(finished()),this,SLOT(replySaveAsFinished()));
    }
}

void ModelDoc::upload(int ind)
{
    QSettings settings("szsm", QApplication::applicationName());
    QDir dir(settings.value("savePath",QDir::homePath()).toString());
    QString filename=QFileDialog::getOpenFileName(NULL, QString::fromUtf8("Открыть файл"),dir.path(),tr("pdf (*.pdf)"));
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)){
        QFileInfo info(file);
        settings.setValue("savePath",info.path());
        QString nom = this->data(this->index(ind,1),Qt::EditRole).toString();
        int id_doc = this->data(this->index(ind,0),Qt::EditRole).toInt();
        QUrl url;
        if (docMap.contains(nom)){
            docInfo info = docMap.value(nom);
            url = QUrl::fromUserInput(Rels::instance()->appServer()+QString("/site/docs/upd?id=%1&elementId=%2").arg(id_doc).arg(info.id));
        } else {
            url = QUrl::fromUserInput(Rels::instance()->appServer()+QString("/site/docs/add?id=%1").arg(id_doc));
        }
        QNetworkRequest request(url);
        request.setRawHeader("Accept-Charset", "UTF-8");
        request.setRawHeader("User-Agent", "Appszsm");
        request.setRawHeader("Content-Type","application/pdf");
        QNetworkReply *reply;
        reply=manager->post(request,file.readAll());
        reply->setProperty("index",ind);
        connect(reply,SIGNAL(finished()),this,SLOT(replyUpdFinished()));
        file.close();
    }
}

void ModelDoc::clearFile(int ind)
{
    QString nom = this->data(this->index(ind,1),Qt::EditRole).toString();
    int n=QMessageBox::question(NULL,QString::fromUtf8("Подтвердите удаление"),QString::fromUtf8("Подтветждаете удаление ")+nom+QString::fromUtf8("?"),QMessageBox::Yes| QMessageBox::No);
    if (n==QMessageBox::Yes) {
        if (docMap.contains(nom)){
            int id_doc = this->data(this->index(ind,0),Qt::EditRole).toInt();
            docInfo info = docMap.value(nom);
            QNetworkRequest request(QUrl::fromUserInput(Rels::instance()->appServer()+QString("/site/docs/upd?id=%1&elementId=%2").arg(id_doc).arg(info.id)));
            request.setRawHeader("Accept-Charset", "UTF-8");
            request.setRawHeader("User-Agent", "Appszsm");
            request.setRawHeader("Content-Type","application/pdf");
            QNetworkReply *reply;
            reply=manager->post(request,QByteArray());
            reply->setProperty("index",ind);
            connect(reply,SIGNAL(finished()),this,SLOT(replyUpdFinished()));
        }
    }
}

void ModelDoc::updateList()
{
    QNetworkRequest request(QUrl::fromUserInput(Rels::instance()->appServer()+"/site/docs/list"));
    request.setRawHeader("Accept-Charset", "UTF-8");
    request.setRawHeader("User-Agent", "Appszsm");
    QNetworkReply *reply;
    reply=manager->get(request);
    connect(reply,SIGNAL(finished()),this,SLOT(replyListFinished()));
}

void ModelDoc::replyListFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply){
        QByteArray data=reply->readAll();
        bool ok=(reply->error()==QNetworkReply::NoError);
        if (!ok){
            QMessageBox::critical(nullptr,tr("Ошибка"),reply->errorString()+"\n"+data,QMessageBox::Cancel);
        } else {
            QJsonDocument respDoc;
            respDoc=QJsonDocument::fromJson(data);
            if (respDoc.isArray()){
                docMap.clear();
                QJsonArray arr = respDoc.array();
                for (QJsonValue v : arr){
                    QString name=v.toObject().value("NAME").toString();
                    docInfo info;
                    info.id=v.toObject().value("ID").toString();
                    info.file=v.toObject().value("FILE").toString();
                    docMap.insert(name,info);
                }
            }
            emit dataChanged(this->index(0,0),this->index(rowCount()-1,columnCount()-1));
            emit sigList();
        }
        reply->deleteLater();
    }
}

void ModelDoc::replyViewFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply){
        QByteArray data=reply->readAll();
        bool ok=(reply->error()==QNetworkReply::NoError);
        if (!ok){
            QMessageBox::critical(nullptr,tr("Ошибка"),reply->errorString()+"\n",QMessageBox::Cancel);
        } else {
            QDir dir(QDir::homePath()+"/.szsm");
            if (!dir.exists()){
                dir.mkdir(dir.path());
            }
            QString tmpname=dir.path()+"/temp.pdf";
            QFile file(tmpname);
            if (file.open(QFile::WriteOnly)){
                file.write(data);
                file.close();
                QFileInfo fileInfo(tmpname);
                QDesktopServices::openUrl((QUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()))));
            }
        }
        reply->deleteLater();
    }

}

void ModelDoc::replySaveAsFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply){
        QByteArray data=reply->readAll();
        bool ok=(reply->error()==QNetworkReply::NoError);
        if (!ok){
            QMessageBox::critical(nullptr,tr("Ошибка"),reply->errorString()+"\n",QMessageBox::Cancel);
        } else {
            int index = reply->property("index").toInt();
            QString name=this->data(this->index(index,1),Qt::EditRole).toString();
            QSettings settings("szsm", QApplication::applicationName());
            QDir dir(settings.value("savePath",QDir::homePath()).toString());
            QString filename=QFileDialog::getSaveFileName(NULL, QString::fromUtf8("Сохранить файл"),dir.path()+QString("/")+name+".pdf",tr("pdf (*.pdf)"));
            if (!filename.isEmpty()){
                QFile file(filename);
                if (file.open(QFile::WriteOnly)){
                    file.write(data);
                    file.close();
                    QFileInfo info(file);
                    settings.setValue("savePath",info.path());
                }
            }
        }
        reply->deleteLater();
    }
}

void ModelDoc::replyUpdFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply){
        QByteArray data=reply->readAll();
        bool ok=(reply->error()==QNetworkReply::NoError);
        if (!ok){
            QMessageBox::critical(nullptr,tr("Ошибка"),reply->errorString()+"\n"+data,QMessageBox::Cancel);
        } else {
            int index = reply->property("index").toInt();
            QJsonDocument respDoc;
            respDoc=QJsonDocument::fromJson(data);
            if (respDoc.isObject()){
                QJsonObject o=respDoc.object().value("result").toObject();
                QString name=o.value("NAME").toString();
                docInfo info;
                info.id=o.value("ID").toString();
                info.file=o.value("FILE").toString();
                //qDebug()<<name<<info.id<<info.file;
                docMap[name]=info;
            }
            emit dataChanged(this->index(index,0),this->index(index,columnCount()-1));
            emit sigList();
        }
        reply->deleteLater();
    }
}
