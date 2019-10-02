#include "rels.h"

Rels* Rels::rels_instance=nullptr;

Rels *Rels::instance()
{
    if (rels_instance==nullptr)
        rels_instance = new Rels();
    return rels_instance;
}

Rels::Rels(QObject *parent) : QObject(parent)
{
    modelGrade = new DbTableModel("zvd_grade",this);
    modelGrade->addColumn("id","id",true,TYPE_INT);
    modelGrade->addColumn("nam",QString::fromUtf8("Наименование"),false,TYPE_STRING);
    modelGrade->setSort("zvd_grade.nam");
    modelGrade->select();

    modelListGost = new DbTableModel("gost_new",this);
    modelListGost->addColumn("id","id",true,TYPE_INT);
    modelListGost->addColumn("nam",QString::fromUtf8("Наименование"),false,TYPE_STRING);
    modelListGost->setSort("gost_new.nam");
    modelListGost->select();

    relDocType = new DbRelation(QString("select id, nam from zvd_doc_type order by nam"),0,1,this);

    modelDoc = new DbTableModel("zvd_doc",this);
    modelDoc->addColumn("id","id",true,TYPE_INT);
    modelDoc->addColumn("nam",QString::fromUtf8("Название"),false,TYPE_STRING);
    modelDoc->addColumn("fnam",QString::fromUtf8("Полное название"),false,TYPE_STRING);
    modelDoc->addColumn("id_doc_type",QString::fromUtf8("Тип"),false,TYPE_STRING,NULL,relDocType);
    modelDoc->setSort("zvd_doc.nam");
    modelDoc->select();

    modelVed = new DbTableModel("zvd_ved",this);
    modelVed->addColumn("id","id",true,TYPE_INT);
    modelVed->addColumn("nam",QString::fromUtf8("Название"),false,TYPE_STRING);
    modelVed->addColumn("short",QString::fromUtf8("Кратеое название"),false,TYPE_STRING);
    modelVed->addColumn("fnam",QString::fromUtf8("Полное название"),false,TYPE_STRING);
    modelVed->setSort("zvd_ved.nam");
    modelVed->select();

    modelGost = new DbTableModel("gost_types",this);
    modelGost->addColumn("id","id",true,TYPE_INT);
    modelGost->addColumn("nam",QString::fromUtf8("Название"),false,TYPE_STRING);
    modelGost->setSort("gost_types.nam");
    modelGost->select();

    modelIso = new DbTableModel("iso_types",this);
    modelIso->addColumn("id","id",true,TYPE_INT);
    modelIso->addColumn("nam",QString::fromUtf8("Название"),false,TYPE_STRING);
    modelIso->setSort("iso_types.nam");
    modelIso->select();

    modelAws = new DbTableModel("aws_types",this);
    modelAws->addColumn("id","id",true,TYPE_INT);
    modelAws->addColumn("nam",QString::fromUtf8("Название"),false,TYPE_STRING);
    modelAws->setSort("aws_types.nam");
    modelAws->select();

    modelZnam = new DbTableModel("denominator",this);
    modelZnam->addColumn("id","id",true,TYPE_INT);
    modelZnam->addColumn("nam",QString::fromUtf8("Название"),false,TYPE_STRING);
    modelZnam->setSort("denominator.nam");
    modelZnam->select();

    modelBukv = new DbTableModel("purpose",this);
    modelBukv->addColumn("id","id",true,TYPE_INT);
    modelBukv->addColumn("nam",QString::fromUtf8("Название"),false,TYPE_STRING);
    modelBukv->setSort("purpose.nam");
    modelBukv->select();

    relVed = new DbRelation(modelVed,0,1,this);
    relVedPix = new DbRelation(QString("select id, simb from zvd_ved"),0,1,this);
    relVidDoc = new DbRelation(modelDoc,0,1,this);
    relElMark = new DbRelation(QString("select id, marka from elrtr where id<>0 order by marka"),0,1,this);
    relElDim = new DbRelation(QString("select ide, fnam from dry_els order by fnam"),0,1,this);
    relElTypes = new DbRelation(QString("select id, nam from el_types order by sort_order, nam"),0,1,this);
    relGrade = new DbRelation(modelGrade,0,1,this);
    relGost = new DbRelation(modelListGost,0,1,this);
    relChem = new DbRelation(QString("select id, sig from chem_tbl order by sig"),0,1,this);
    relMech = new DbRelation(QString("select id, nam from mech_tbl order by nam"),0,1,this);
    relMechx = new DbRelation(QString("select id, short from mechx_tbl order by nam"),0,1,this);
    relMechxVal = new DbRelation(QString("select id, nam from mechx_nams order by nam"),0,1,this);
    relProvol = new DbRelation(QString("select id, nam from provol order by nam"),0,1,this);
    relGrp = new DbRelation(QString("select id, typ from el_grp order by typ"),0,1,this);
    relVid = new DbRelation(QString("select id, nam from el_types order by nam"),0,1,this);
    relPol = new DbRelation(QString("select id, descr from pics order by descr"),0,1,this);
    relGostType = new DbRelation(modelGost,0,1,this);
    relIso = new DbRelation(modelIso,0,1,this);
    relAws = new DbRelation(modelAws,0,1,this);
    relZnam = new DbRelation(modelZnam,0,1,this);
    relBukv = new DbRelation(modelBukv,0,1,this);
    relDiam = new DbRelation(QString("select id, sdim, is_el from diam order by sdim"),0,1,this);
    relPosPix = new DbRelation(QString("select id, data from pics"),0,1,this);
    relPlav = new DbRelation(QString("select id, nam from el_plav_nams"),0,1,this);

    relDiam->proxyModel()->setFilterKeyColumn(2);
    relDiam->proxyModel()->setFilterFixedString("1");

    connect(modelVed,SIGNAL(sigUpd()),relVedPix,SLOT(refreshModel()));
}

void Rels::refresh()
{
    relVed->refreshModel();
    relVedPix->refreshModel();
    relVidDoc->refreshModel();
    relElMark->refreshModel();
    relElDim->refreshModel();
    relElTypes->refreshModel();
    relGrade->refreshModel();
    relGost->refreshModel();
    relChem->refreshModel();
    relMech->refreshModel();
    relMechx->refreshModel();
    relMechxVal->refreshModel();
    relProvol->refreshModel();
    relGrp->refreshModel();
    relVid->refreshModel();
    relPol->refreshModel();
    relGostType->refreshModel();
    relIso->refreshModel();
    relAws->refreshModel();
    relZnam->refreshModel();
    relBukv->refreshModel();
    relDiam->refreshModel();
    relPosPix->refreshModel();
    relDocType->refreshModel();
    relPlav->refreshModel();

    emit sigRefresh();
}
