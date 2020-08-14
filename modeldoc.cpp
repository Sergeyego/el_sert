#include "modeldoc.h"


ModelDoc::ModelDoc(QObject *parent) : DbTableModel("zvd_sert",parent)
{
    ftpClient = new QFtp(this);
    updateFtpInfo();
    getState=0;
    connect(ftpClient,SIGNAL(commandStarted(int)),this,SLOT(ftpCommandStart(int)));
    connect(ftpClient, SIGNAL(commandFinished(int,bool)),this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftpClient, SIGNAL(listInfo(QUrlInfo)),this, SLOT(addToList(QUrlInfo)));

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
    addColumn("id_ved",QString::fromUtf8("Ведомство"),NULL,Rels::instance()->relVed);
    addColumn("id_doc",QString::fromUtf8("Тип документа"),NULL,Rels::instance()->relVidDoc);

    setSort("zvd_sert.nom_doc, zvd_sert.dat_doc");

    connect(this,SIGNAL(sigUpd()),this,SLOT(updateList()));
    connect(this,SIGNAL(sigRefresh()),this,SLOT(updateList()));
}

QVariant ModelDoc::data(const QModelIndex &index, int role) const
{
    if (role==Qt::BackgroundRole){
        int id = data(this->index(index.row(),0),Qt::EditRole).toInt();
        if (ftpExist(id) && ModelDoc::isActive(index.row())){
            return QVariant(QColor(170,255,170));
        } else {
            return QVariant(QColor(255,170,170));
        }
    }
    return DbTableModel::data(index,role);
}

bool ModelDoc::ftpGet(int id, int type)
{
    bool ok=(ftpClient->state()==QFtp::LoggedIn);
    if (!ok){
        ftpConnect();
    }
    int interval= ok ? 0 : delay;
    QTimer::singleShot(interval, [this, id, type]() {
        if (ftpClient->state()==QFtp::LoggedIn && docMap.contains(id)){
            getFile = new QFile(docMap.value(id));
            if (!getFile->open(QIODevice::WriteOnly)){
                delete getFile;
                return false;
            }
            getState=type;
            ftpClient->get(docMap.value(id),getFile);
        }
    } );
    return ok;
}

bool ModelDoc::ftpPut(int id)
{
    bool ok=(ftpClient->state()==QFtp::LoggedIn);
    if (!ok){
        ftpConnect();
    }
    int interval= ok ? 0 : delay;
    QTimer::singleShot(interval, [this, id]() {
        if (ftpClient->state()==QFtp::LoggedIn){
            QSettings settings("szsm", QApplication::applicationName());
            QDir dir(settings.value("savePath",QDir::homePath()).toString());
            QString filename=QFileDialog::getOpenFileName(NULL, QString::fromUtf8("Открыть файл"),dir.path(),tr("pdf (*.pdf)"));
            putFile = new QFile(filename);
            if (putFile->open(QIODevice::ReadOnly)){
                QFileInfo info(*putFile);
                settings.setValue("savePath",info.path());
                ftpClient->put(putFile,QString::number(id)+".pdf");
            }
        }
    } );
    return ok;
}

bool ModelDoc::ftpExist(int id) const
{
    return docMap.contains(id);
}

bool ModelDoc::ftpDel(int id)
{
    bool ok=(ftpClient->state()==QFtp::LoggedIn);
    if (!ok){
        ftpConnect();
    }
    int interval= ok ? 0 : delay;
    QTimer::singleShot(interval, [this, id]() {
        if (ftpClient->state()==QFtp::LoggedIn && docMap.contains(id)){
            int n=QMessageBox::question(NULL,QString::fromUtf8("Подтвердите удаление"),
                                        QString::fromUtf8("Подтветждаете удаление ")+getDocNumrer(id)+QString::fromUtf8("?"),QMessageBox::Yes| QMessageBox::No);
            if (n==QMessageBox::Yes) {
                ftpClient->remove(docMap.value(id));
            }
        }
    } );
    return ok;
}

QString ModelDoc::getDocNumrer(int id)
{
    QString docNum("-");
    QSqlQuery query;
    query.prepare("select nom_doc from zvd_sert where id = :id");
    query.bindValue(":id",id);
    if (query.exec()){
        while (query.next()){
            docNum=query.value(0).toString();
        }
    } else {
        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),query.lastError().text());
    }
    return docNum;
}

bool ModelDoc::isActive(int ind) const
{
    QDate beg=data(index(ind,4),Qt::EditRole).toDate();
    QDate end=data(index(ind,5),Qt::EditRole).toDate();
    QDate current=QDate::currentDate();
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

void ModelDoc::updateFtpInfo()
{
    QSqlQuery query;
    query.prepare("select host_int, user_rw, pass_rw, path_sert, conn_delay from ftp_info where id = 1");
    if (query.exec()){
        while (query.next()){
            ftphost=query.value(0).toString();
            ftpuser=query.value(1).toString();
            ftppassword=query.value(2).toString();
            ftppath=query.value(3).toString();
            delay=query.value(4).toInt();
        }
    } else {
        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),query.lastError().text());
    }
}

void ModelDoc::ftpConnect()
{
    ftpClient->connectToHost(ftphost);
}

void ModelDoc::updateList()
{
    //qDebug()<<"upd"<<ftpClient->state();
    if (ftpClient->state()==QFtp::LoggedIn){
        ftpClient->list();
    } else {
        ftpConnect();
    }
}

void ModelDoc::ftpCommandFinished(int /*commandId*/, bool error)
{
    if (ftpClient->currentCommand() == QFtp::Get){
        getFile->close();
        if (!error) {
            if (getState==1){
                QString tmpname="temp.pdf";
                QFile file(tmpname);
                if (file.exists()){
                    file.remove();
                }
                getFile->copy(tmpname);
                QFileInfo fileInfo(tmpname);
                QDesktopServices::openUrl((QUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()))));
            } else {
                QString id=getFile->fileName();
                id.truncate(id.length()-4);
                QString docNum=getDocNumrer(id.toInt());
                QString filename=QFileDialog::getSaveFileName(NULL, QString::fromUtf8("Сохранить файл"),QDir::homePath()+QString("/")+docNum+".pdf",tr("pdf (*.pdf)"));
                if (!filename.isEmpty()){
                    getFile->copy(filename);
                }
            }
        }
        getState=0;
        getFile->remove();
        delete getFile;
    } else if (ftpClient->currentCommand() == QFtp::Put){
        putFile->close();
        delete putFile;
        if (!error){
            updateList();
        }
    }
    if (error) {
        QMessageBox::critical(NULL, tr("FTP"),ftpClient->errorString());
    } else {
        if (ftpClient->currentCommand() == QFtp::ConnectToHost) {
            ftpClient->login(QString(ftpuser),QString(ftppassword));
        } else if (ftpClient->currentCommand() == QFtp::Login){
            ftpClient->cd(ftppath);
        } else if (ftpClient->currentCommand() == QFtp::List) {
            emit dataChanged(this->index(0,0),this->index(rowCount()-1,columnCount()-1));
            emit sigList();
            //qDebug()<<"list finished";
        } else if (ftpClient->currentCommand()==QFtp::Cd){
            ftpClient->list();
        } else if (ftpClient->currentCommand()==QFtp::Remove){
            ftpClient->list();
        }
    }
}

void ModelDoc::ftpCommandStart(int /*commandId*/)
{
    if (ftpClient->currentCommand()==QFtp::List){
        docMap.clear();
        //qDebug()<<"Start list";
    }
}

void ModelDoc::addToList(const QUrlInfo &urlInfo)
{
    if (urlInfo.isFile()){
        QString name=urlInfo.name();
        name.truncate(name.length()-4);
        bool ok;
        int id=name.toInt(&ok);
        if (ok){
            docMap.insert(id,urlInfo.name());
        }
    }
}

