#ifndef RELS_H
#define RELS_H

#include <QObject>
#include "db/dbtablemodel.h"

class Rels : public QObject
{
    Q_OBJECT
public:
    static Rels *instance();
    DbSqlRelation *relVed;
    QMap<int, QByteArray> mapVedPix;
    QMap<int, QByteArray> mapPolPix;
    DbSqlRelation *relVidDoc;
    DbSqlRelation *relElMark;
    DbSqlRelation *relElDim;
    DbSqlRelation *relGrade;
    DbSqlRelation *relGost;
    DbSqlRelation *relChem;
    DbSqlRelation *relMech;
    DbSqlRelation *relMechx;
    DbSqlRelation *relMechxVal;
    DbSqlRelation *relProvol;
    DbSqlRelation *relGrp;
    DbSqlRelation *relVid;
    DbSqlRelation *relPol;
    DbSqlRelation *relGostType;
    DbSqlRelation *relIso;
    DbSqlRelation *relAws;
    DbSqlRelation *relZnam;
    DbSqlRelation *relBukv;
    DbSqlRelation *relDiam;
    DbSqlRelation *relDocType;
    DbSqlRelation *relPlav;
    DbSqlRelation *relWireDiam;
    DbSqlRelation *relChemDev;
    DbSqlRelation *relPack;
    DbSqlRelation *relEanEd;
    DbSqlRelation *relEanGr;
    DbSqlRelation *relVar;
    DbSqlRelation *relSertType;
    DbSqlRelation *relIntClass;
    QString signServer();
    QString appServer();

protected:
    explicit Rels(QObject *parent = nullptr);

private:
    static Rels *rels_instance;

public slots:
    void refreshElDim();
    void refreshVedPix();
    void refreshPolPix();
    void partSelectReq(int id_part);

signals:
    void partReq(int id_part);
};

#endif // RELS_H
