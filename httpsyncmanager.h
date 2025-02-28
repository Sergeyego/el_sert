#ifndef HTTPSYNCMANAGER_H
#define HTTPSYNCMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QEventLoop>

class HttpSyncManager : public QObject
{
    Q_OBJECT
public:
    explicit HttpSyncManager(QObject *parent = nullptr);
    static bool sendRequest(QString path, QString req, const QByteArray &data, QByteArray &respData);
    static bool sendGet(QString path, QByteArray &data);

};

#endif // HTTPSYNCMANAGER_H
