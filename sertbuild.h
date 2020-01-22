#ifndef SERTBUILD_H
#define SERTBUILD_H

#include <QTextDocument>
#include <QtGui>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QTextCharFormat>
#include <QMessageBox>
#include <QTextDocumentFragment>
#include <QTextLength>
#include "rels.h"
#include "qr/qrencode.h"

struct sLang {
    QString rus;
    QString eng;
};

struct generalData {
    sLang adres;
    QString contact;
    sLang otk;
    QImage logo;
    QImage sign;
};

struct headData {
    int id_parti;
    QString nomPart;
    int yearPart;
    QDate datePart;
    QString marka;
    double diam;
    QString tip1;
    QString tip2;
    QString znam;
    QString prov;
    sLang poluch;
    QString nomSert;
    QDate dateVidSert;
    double netto;
};

struct chemData {
    QString name;
    QVariant value;
};

struct mechData {
    int id_cat;
    sLang nam_html;
    sLang sig_htlm;
    sLang prefix;
    QVariant value;
};

struct sertData {
    int id_ved;
    sLang doc_nam;
    sLang ved_nam;
    QString nom_doc;
    QDate date_doc;
    QString gr_tech_ust;
    int id_doc_t;
    sLang ved_short;
    QString grade_nam;
};

typedef QVector<chemData> cvData;
typedef QVector<mechData> mvData;
typedef QVector<sertData> svData;

class DataSert : public QObject
{
    Q_OBJECT
public:
    DataSert(QObject *parent = 0);
    void refresh(int id, bool is_ship=true);
    const generalData* general();
    const headData* head();
    const cvData* chem();
    const mvData* mech();
    const svData* sert();
    const QImage* qrCode();
    QString tu();
    sLang mechCategory(int id);

private:
    void refreshTu();
    void refreshChem();
    void refreshMech();
    void refreshSert();
    void refreshQR(int id, bool is_ship);
    generalData gData;
    headData hData;
    QStringList tuList;
    QMap<int,sLang> mechCat;
    cvData cData;
    mvData mData;
    svData sData;
    QImage qr_code;

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
    void insertHtml(QTextCursor &c, const QString &html);
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
