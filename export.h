#ifndef EXPORT_H
#define EXPORT_H

#include <QObject>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QFileDialog>
#include <QDebug>
#include "qftp/qftp.h"
#include <QProgressDialog>
#include <QApplication>

class Export: public QObject
{
    Q_OBJECT
public:
    Export(QObject *parent = nullptr);
    ~Export();
    void createXml();
    void start();

private:
    QDomElement getMark(int id_el,  QDomDocument *doc);
    QDomElement getWire(int id_pr,  QDomDocument *doc);
    QDomElement getSert(int id_sert,  QDomDocument *doc);
    QDomElement getSertMark(int id_sert,  QDomDocument *doc);
    QDomElement getSertWire(int id_sert,  QDomDocument *doc);
    QDomElement newElement(QString nam, QString val,  QDomDocument *doc);
    QDomElement getTu(int id_el,  QDomDocument *doc);
    QDomElement getWireTu(int id_pr,  QDomDocument *doc);
    QDomElement getAmp(int id_el,  QDomDocument *doc);
    QDomElement getPlav(int id_el,  QDomDocument *doc);
    QDomElement getChem(int id_pr,  QDomDocument *doc);
    QDomElement getWireChem(int id_pr,  QDomDocument *doc);
    QDomElement getMech(int id_el,  QDomDocument *doc);
    QDomElement getDiams(int id_el,  QDomDocument *doc);
    QDomElement getWireDiams(int id_pr,  QDomDocument *doc);
    QDomElement getSpool(int id_diam,  QDomDocument *doc);
    QString testStr(const QString &s);
    QString fromDouble(const QVariant &v, int d=1);
    QString fromDate(const QDate &d);
    QFtp *ftpClient;
    QMap <int, QString> docMap;
    QStringList ftpFiles;
    QString ftphost;
    QString ftpuser;
    QString ftppassword;
    QString ftpuserrw;
    QString ftppasswordrw;
    QString ftppath;
    QProgressDialog *progress;
    QByteArray xmldata;

private slots:
    void ftpConnect();
    void updateList();
    void ftpCommandFinished(int commandId, bool error);
    void ftpCommandStart(int commandId);
    void addToList(const QUrlInfo &urlInfo);
    void updateFtpInfo();

signals:
    void finished();
};

#endif // EXPORT_H
