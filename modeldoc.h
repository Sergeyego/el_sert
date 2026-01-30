#ifndef MODELDOC_H
#define MODELDOC_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "rels.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMap>
#include <QFile>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QSettings>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


struct docInfo {
    QString id;
    QString file;
};

class ModelDoc : public DbTableModel
{
    Q_OBJECT
public:
    ModelDoc(QObject *parent=0);
    QVariant data(const QModelIndex &index, int role) const;
    bool fileExist(int ind) const;
    bool isActive(int ind) const;

public slots:
    void refresh(bool activeOnly);
    void viewDoc(int ind);
    void saveAs(int ind);
    void upload(int ind);
    void clearFile(int ind);

private:
    QNetworkAccessManager *manager;
    QMap <QString, docInfo> docMap;

private slots:
    void updateList();
    void replyListFinished();
    void replyViewFinished();
    void replySaveAsFinished();
    void replyUpdFinished();

signals:
    void sigList();
};

#endif // MODELDOC_H
