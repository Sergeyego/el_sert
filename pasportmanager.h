#ifndef PASPORTMANAGER_H
#define PASPORTMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDir>
#include <QDesktopServices>
#include "rels.h"

class PasportManager : public QObject
{
    Q_OBJECT
public:
    explicit PasportManager(QObject *parent = nullptr);
    void getPasport(int id_part, QString filename, int year, const QByteArray &json);

private:
    QNetworkAccessManager *manager;

private slots:
    void onResult(QNetworkReply *reply);

signals:
};

#endif // PASPORTMANAGER_H
