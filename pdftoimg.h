#ifndef PDFTOIMG_H
#define PDFTOIMG_H

#include <QObject>
#include <QImage>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QProgressDialog>
#include <QApplication>
#include <poppler/qt5/poppler-qt5.h>
#include "qftp/qftp.h"

class PdfToImg : public QObject
{
    Q_OBJECT
public:
    explicit PdfToImg(QObject *parent = nullptr);
    ~PdfToImg();

private:
    QFtp *ftpClient;
    QStringList ftpFiles;
    QString ftphost;
    QString ftpuser;
    QString ftppassword;
    QString ftppath;
    QFile *getFile;
    QFile *putFile;
    QProgressDialog *progress;
    int currentIndex;

signals:
    void finished();

public slots:
    void start();

private slots:
    void ftpConnect();
    void updateList();
    void ftpCommandFinished(int commandId, bool error);
    void ftpCommandStart(int commandId);
    void addToList(const QUrlInfo &urlInfo);
    void updateFtpInfo();
    void nextConvert();
    void ftpPut(QString fileName);
};

#endif // PDFTOIMG_H
