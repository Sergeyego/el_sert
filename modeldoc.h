#ifndef MODELDOC_H
#define MODELDOC_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "rels.h"
#include "qftp/qftp.h"
#include <QMap>
#include <QFile>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QSettings>
#include <QApplication>

class ModelDoc : public DbTableModel
{
    Q_OBJECT
public:
    ModelDoc(QObject *parent=0);
    QVariant data(const QModelIndex &index, int role) const;
    bool ftpGet(int id, int type);
    bool ftpPut(int id);
    bool ftpExist(int id);
    bool ftpDel(int id);
    QString getDocNumrer(int id);

public slots:
    void refresh(bool activeOnly);

private:
    QFtp *ftpClient;
    QMap <int, QString> docMap;
    bool ftpGet(int id, QFile *file);
    int getState;
    QFile *getFile;
    QFile *putFile;

private slots:
    void ftpConnect();
    void updateList();
    void ftpCommandFinished(int commandId, bool error);
    void ftpCommandStart(int commandId);
    void addToList(const QUrlInfo &urlInfo);

signals:
    void sigList();
};

#endif // MODELDOC_H
