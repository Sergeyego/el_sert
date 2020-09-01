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
    modelGrade->addColumn("id","id");
    modelGrade->addColumn("nam",QString::fromUtf8("Наименование"));
    modelGrade->setSort("zvd_grade.nam");
    modelGrade->select();

    modelListGost = new DbTableModel("gost_new",this);
    modelListGost->addColumn("id","id");
    modelListGost->addColumn("nam",QString::fromUtf8("Наименование"));
    modelListGost->setSort("gost_new.nam");
    modelListGost->select();

    relDocType = new DbRelation(QString("select id, nam from zvd_doc_type order by nam"),0,1,this);

    modelDoc = new DbTableModel("zvd_doc",this);
    modelDoc->addColumn("id","id");
    modelDoc->addColumn("nam",QString::fromUtf8("Название"));
    modelDoc->addColumn("fnam",QString::fromUtf8("Полное название"));
    modelDoc->addColumn("id_doc_type",QString::fromUtf8("Тип"),NULL,relDocType);
    modelDoc->addColumn("fnam_en",QString::fromUtf8("Полное название анг."));
    modelDoc->setSort("zvd_doc.nam");
    modelDoc->select();

    modelVed = new DbTableModel("zvd_ved",this);
    modelVed->addColumn("id","id");
    modelVed->addColumn("nam",QString::fromUtf8("Название"));
    modelVed->addColumn("short",QString::fromUtf8("Кратеое название"));
    modelVed->addColumn("fnam",QString::fromUtf8("Полное название"));
    modelVed->addColumn("fnam_en",QString::fromUtf8("Полное название анг."));
    modelVed->addColumn("short_en",QString::fromUtf8("Кратеое название анг."));
    modelVed->setSort("zvd_ved.nam");
    modelVed->select();

    modelGost = new DbTableModel("gost_types",this);
    modelGost->addColumn("id","id");
    modelGost->addColumn("nam",QString::fromUtf8("Название"));
    modelGost->setSort("gost_types.nam");
    modelGost->select();

    modelIso = new DbTableModel("iso_types",this);
    modelIso->addColumn("id","id");
    modelIso->addColumn("nam",QString::fromUtf8("Название"));
    modelIso->setSort("iso_types.nam");
    modelIso->select();

    modelAws = new DbTableModel("aws_types",this);
    modelAws->addColumn("id","id");
    modelAws->addColumn("nam",QString::fromUtf8("Название"));
    modelAws->setSort("aws_types.nam");
    modelAws->select();

    modelZnam = new DbTableModel("denominator",this);
    modelZnam->addColumn("id","id");
    modelZnam->addColumn("nam",QString::fromUtf8("Название"));
    modelZnam->setSort("denominator.nam");
    modelZnam->select();

    modelBukv = new DbTableModel("purpose",this);
    modelBukv->addColumn("id","id");
    modelBukv->addColumn("nam",QString::fromUtf8("Название"));
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
    relWireDiam = new DbRelation(QString("select id, sdim from diam order by sdim"),0,1,this);
    relChemDev = new DbRelation(QString("select id, short from chem_dev order by short"),0,1,this);

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
    relWireDiam->refreshModel();
    relChemDev->refreshModel();

    emit sigRefresh();
}
