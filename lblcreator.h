#ifndef LBLCREATOR_H
#define LBLCREATOR_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include "rtf/rtfcpp.h"
#include "rels.h"
#include "glabels/glabelslbl.h"

struct dataPart{
    int id_el;
    int id_diam;
    QDate datePart;
    QString ibco;
    int id_var;
};

struct dataLbl {
    QString marka;
    QString diam;
    QString isotype;
    QString awstype;
    QString gosttype;
    QString bukv;
    QString znam;
    QString descr;
    QString vl;
    QString pr;
    int id_pix=-1;
};

struct dataAmp {
    QString diam;
    QString bottom;
    QString vert;
    QString top;
};

class LblCreator : public QObject
{
    Q_OBJECT
public:
    explicit LblCreator(QObject *parent = nullptr);
    bool createLbl(int id_part, bool shortAmp=true);
    bool createLbl(int id_el, int id_diam, QString ibco=QString(), QDate date=QDate::currentDate(), bool shortAmp=true, bool order=false, int id_var=1);
    bool createLblGlabels(int id_el, int id_diam, QString ibco=QString(), QDate date=QDate::currentDate(), int id_var=1);
    bool createLblGlabels(int id_part);
    bool createLblGlabels2(int id_el, int id_diam, QString ibco=QString(), QDate date=QDate::currentDate(), int id_var=1);
    bool createLblGlabels2(int id_part);

private:
    QString orgNam;
    QString adres;
    QString otk;
    QString kach;
    QMap<int,QString> mapDocTypes;
    void sysCommand(QString fname);
    QString getTuList(int id_el, int id_diam, QDate date, int id_var=1);
    QString getSrtStr(int id_el, int id_diam, QDate date, int id_var=1);
    dataLbl getData(int id_el, int id_diam, int id_var=1);
    QString getCh(dataLbl &data);
    QString getProc(dataLbl &data);
    QString getIzg();
    QVector<dataAmp> getAmp(int id_el, int id_diam, bool shortAmp=false, bool single=false, int id_var=1);
    dataPart getDataPart(int id_part);
signals:

public slots:
};

#endif // LBLCREATOR_H
