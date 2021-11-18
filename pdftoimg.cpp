#include "pdftoimg.h"

PdfToImg::PdfToImg(QObject *parent) : QObject(parent)
{
    updateFtpInfo();
    ftpClient = new QFtp(this);
    connect(ftpClient,SIGNAL(commandStarted(int)),this,SLOT(ftpCommandStart(int)));
    connect(ftpClient, SIGNAL(commandFinished(int,bool)),this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftpClient, SIGNAL(listInfo(QUrlInfo)),this, SLOT(addToList(QUrlInfo)));
    currentIndex=-1;
    progress = new QProgressDialog();
    progress->setWindowTitle(QString::fromUtf8("Создание изображений сертификатов"));
    progress->setAutoClose(false);
    progress->setCancelButton(NULL);
    progress->setMinimumDuration(0);
    progress->setMinimum(0);
}

PdfToImg::~PdfToImg()
{
    delete progress;
}

void PdfToImg::start()
{
    currentIndex=-1;
    updateList();
}

void PdfToImg::ftpConnect()
{
    ftpClient->connectToHost(ftphost);
}

void PdfToImg::updateList()
{
    if (ftpClient->state()==QFtp::LoggedIn){
        ftpClient->list();
    } else {
        ftpConnect();
    }
}

void PdfToImg::ftpCommandFinished(int /*commandId*/, bool error)
{
    if (ftpClient->currentCommand() == QFtp::Get){
        getFile->close();
        /*if (!error) {
            Poppler::Document *doc = Poppler::Document::load(getFile->fileName());
            if (doc){
                Poppler::Page *pdfPage=doc->page(0);
                if (pdfPage){
                    QImage img=pdfPage->renderToImage(150.0,150.0);
                    QString putfname=getFile->fileName()+QString("-1.jpg");
                    if (img.save(putfname,"jpg",83)){
                        ftpPut(putfname);
                    }
                    delete pdfPage;
                }
                delete doc;
            } else {
                QMessageBox::critical(NULL, tr("Convert"),QString::fromUtf8("Ошибка открытия документа ")+getFile->fileName());
            }
        }*/
        getFile->remove();
        delete getFile;
    } else if (ftpClient->currentCommand() == QFtp::Put){
        putFile->close();
        putFile->remove();
        delete putFile;
        nextConvert();
    }

    if (error) {
        QMessageBox::critical(NULL, tr("FTP"),ftpClient->errorString());
    } else {
        if (ftpClient->currentCommand() == QFtp::ConnectToHost) {
            //qDebug()<<"connect ok";
            ftpClient->login(ftpuser,ftppassword);
        } else if (ftpClient->currentCommand() == QFtp::Login){
            ftpClient->cd(ftppath);
        } else if (ftpClient->currentCommand() == QFtp::List) {
            //qDebug()<<"list finished";
            progress->setMaximum(ftpFiles.size());
            nextConvert();
        } else if (ftpClient->currentCommand()==QFtp::Cd){
            ftpClient->list();
        } else if (ftpClient->currentCommand()==QFtp::Remove){
            ftpClient->list();
        }
    }
}

void PdfToImg::ftpCommandStart(int /*commandId*/)
{
    if (ftpClient->currentCommand()==QFtp::List){
        ftpFiles.clear();
        //qDebug()<<"Start list";
    }
}

void PdfToImg::addToList(const QUrlInfo &urlInfo)
{
    if (urlInfo.isFile()){
        if (urlInfo.name().right(4).toLower()==".pdf"){
            ftpFiles.append(urlInfo.name());
        }
    }
}

void PdfToImg::updateFtpInfo()
{
    QSqlQuery query;
    query.prepare("select host_ext, user_rw, pass_rw, path_sert from ftp_info where id = 1");
    if (query.exec()){
        while (query.next()){
            ftphost=query.value(0).toString();
            ftpuser=query.value(1).toString();
            ftppassword=query.value(2).toString();
            ftppath=query.value(3).toString();
        }
    } else {
        QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),query.lastError().text());
    }
}

void PdfToImg::nextConvert()
{
    currentIndex++;
    if (currentIndex>=0 && (currentIndex<ftpFiles.size())){
        progress->setLabelText(QString::fromUtf8("Обработка файла ")+ftpFiles.at(currentIndex)+QString::fromUtf8(" (%1 из %2)").arg(currentIndex+1).arg(ftpFiles.size()));
        progress->setValue(currentIndex);
        QApplication::processEvents();
        getFile = new QFile(ftpFiles.at(currentIndex));
        if (!getFile->open(QIODevice::WriteOnly)){
            delete getFile;
            emit finished();
            return;
        }
        ftpClient->get(ftpFiles.at(currentIndex),getFile);
    } else {
        emit finished();
    }
}

void PdfToImg::ftpPut(QString fileName)
{
    if (ftpClient->state()==QFtp::LoggedIn){
        putFile = new QFile(fileName);
        if (putFile->open(QIODevice::ReadOnly)){
            ftpClient->put(putFile,fileName);
        }
    }
}
