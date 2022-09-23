#ifndef RELS_H
#define RELS_H

#include <QObject>
#include "db/dbtablemodel.h"

class Rels : public QObject
{
    Q_OBJECT
public:
    static Rels *instance();
    DbTableModel *modelGrade;
    DbTableModel *modelListGost;
    DbTableModel *modelDoc;
    DbTableModel *modelVed;
    DbTableModel *modelGost;
    DbTableModel *modelIso;
    DbTableModel *modelAws;
    DbTableModel *modelZnam;
    DbTableModel *modelBukv;
    DbTableModel *modelVar;

    DbRelation *relVed;
    DbRelation *relVedPix;
    DbRelation *relVidDoc;
    DbRelation *relElMark;
    DbRelation *relElDim;
    DbRelation *relElTypes;
    DbRelation *relGrade;
    DbRelation *relGost;
    DbRelation *relChem;
    DbRelation *relMech;
    DbRelation *relMechx;
    DbRelation *relMechxVal;
    DbRelation *relProvol;
    DbRelation *relGrp;
    DbRelation *relVid;
    DbRelation *relPol;
    DbRelation *relGostType;
    DbRelation *relIso;
    DbRelation *relAws;
    DbRelation *relZnam;
    DbRelation *relBukv;
    DbRelation *relDiam;
    DbRelation *relPosPix;
    DbRelation *relDocType;
    DbRelation *relPlav;
    DbRelation *relWireDiam;
    DbRelation *relChemDev;
    DbRelation *relPack;
    DbRelation *relEan;
    DbRelation *relVar;

protected:
    explicit Rels(QObject *parent = nullptr);

private:
    static Rels *rels_instance;

signals:
    void sigRefresh();

public slots:
    void refresh();

};

#endif // RELS_H
