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
    bool createLbl(int id_el, int id_diam, QString ibco=QString(), QDate date=QDate::currentDate(), bool shortAmp=true, bool order=false);
    bool createLblGlabels(int id_el, int id_diam, QString ibco=QString(), QDate date=QDate::currentDate());
    bool createLblGlabels(int id_part);
    bool createLblGlabels2(int id_el, int id_diam, QString ibco=QString(), QDate date=QDate::currentDate());
    bool createLblGlabels2(int id_part);

private:
    QString orgNam;
    QString adres;
    QString otk;
    QString kach;
    void sysCommand(QString fname);
    QString getTuList(int id_el, int id_diam, QDate date);
    QString getSrtStr(int id_el, int id_diam, QDate date);
    dataLbl getData(int id_el, int id_diam);
    QString getCh(dataLbl &data);
    QString getProc(dataLbl &data);
    QString getIzg();
    QVector<dataAmp> getAmp(int id_el, int id_diam, bool shortAmp=false, bool single=false);
    dataPart getDataPart(int id_part);
signals:

public slots:
};

#endif // LBLCREATOR_H
