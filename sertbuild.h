#ifndef SERTBUILD_H
#define SERTBUILD_H

#include <QTextDocument>
#include <QtGui>
#include <QFont>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QTextCharFormat>
#include <QMessageBox>
#include <QTextDocumentFragment>
#include <QTextLength>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "rels.h"

struct sertData {
    int id_doc;
    QString nom_doc;
    QString ved_short;
    bool en;
};

typedef QVector<sertData> svData;

class SertBuild : public QTextDocument
{
    Q_OBJECT
public:
    explicit SertBuild(QObject *parent = 0);
    QString getNomPart();
    QString getYearSert();
    QString getYearPart();
    QString getNomSert();
    int getType();
    svData getSData();

public slots:
    void build(int id_part, int id_ship);
    void rebuild();

    void setType(int t);
    void setLang(QString l);
    void setDocEn(int id_doc, bool en);
    void setDefaultDoc();
    void clearCache();

private:
    int sertType;
    int current_id_part;
    int current_id_ship;
    QString lang;
    bool sendRequest(QString path, QString req, const QByteArray &data, QByteArray &respData);
    void loadDoc(const QString &html);
    void updSData();
    QMap<QString,QByteArray> map;
    svData sdata;

signals:
    void sigRefresh();
};


#endif // SERTBUILD_H
