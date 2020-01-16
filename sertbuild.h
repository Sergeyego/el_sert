#ifndef SERTBUILD_H
#define SERTBUILD_H

#include <QTextDocument>
#include <QtGui>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QTextCharFormat>
#include <qrencode.h>
#include <QMessageBox>
#include <QTextDocumentFragment>
#include <QTextLength>
#include "rels.h"

class DataSert : public QObject
{
    Q_OBJECT

public:
    DataSert(QObject *parent = 0);
    void refresh(int id, bool is_ship=true);
    int id_parti;
    double netto;
    QString nomSert;
    QDate dateVidSert;
    QString nomPart;
    int yearPart;
    QDate datePart;
    QString prov;
    QString poluch;
    QString poluch_en;
    QStringList tuList;
    QString adres;
    QString adres_en;
    QString contact;
    QString otk;
    QString otk_en;
    QString marka;
    double diam;
    QString tip1;
    QString tip2;
    QString znam;
    QSqlQueryModel *chemModel;
    QSqlQueryModel *mechModel;
    QSqlQueryModel *sertModel;
    QSqlQueryModel *mechCategory;
    QImage qrCode;
    QImage logo;
    QImage sign;

private:
    void refreshTu();
    void refreshChem();
    void refreshMech();
    void refreshSert();
    void refreshQR(int id, bool is_ship);

public slots:
    void refreshMechCategory();
};

class SertBuild : public QTextDocument
{
    Q_OBJECT
public:
    explicit SertBuild(QObject *parent = 0);
    QString getNomPart();
    QString getYearSert();
    QString getYearPart();
    QString getNomSert();
    bool getPrn();
public slots:
    void build(int id, bool is_ship);
private:
    QTextCursor* cursor;
    DataSert *data;
    bool prn;
    int current_id;
    bool current_is_ship;
    bool l_rus;
    bool l_en;
    void insertText(QTextCursor &c, const QString &rus, const QString &eng, bool newpar=false, bool sep=true, bool html=false);
    void insertDouble(QTextCursor &c, const QVariant &val, int dec);
    void insertDate(QTextCursor &c, const QDate &date, bool newpar=true);
    
signals:
    
public slots:
    void setPrn(bool p);
    void setLRus(bool b);
    void setLEn(bool b);
    void setLMix(bool b);
};


#endif // SERTBUILD_H
