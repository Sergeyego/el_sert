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
    relVed = new DbSqlRelation("zvd_ved","id","nam",this);
    //relVedPix = new DbRelation(QString("select id, simb from zvd_ved"),0,1,this);
    relVidDoc = new DbSqlRelation("zvd_doc","id","nam",this);
    relElMark = new DbSqlRelation("elrtr","id","marka",this);
    relElMark->setFilter("elrtr.id<>0");
    relElDim = new DbSqlRelation("dry_els","ide","fnam",this);
    relGrade = new DbSqlRelation("zvd_grade","id","nam",this);
    relGrade->setEditable(true);
    relGost = new DbSqlRelation("gost_new","id","nam",this);
    relGost->setEditable(true);
    relChem = new DbSqlRelation("chem_tbl","id","sig",this);
    relMech = new DbSqlRelation("mech_tbl","id","nam",this);
    relMechx = new DbSqlRelation("mechx_tbl","id","short",this);
    relMechxVal = new DbSqlRelation("mechx_nams","id","nam",this);
    relProvol = new DbSqlRelation("provol","id","nam",this);
    relProvol->setFilter("provol.id<>0");
    relGrp = new DbSqlRelation("el_grp","id","typ",this);
    relGrp->setEditable(true);
    relVid = new DbSqlRelation("el_types","id","nam",this);
    relPol = new DbSqlRelation("pics","id","descr",this);
    relGostType = new DbSqlRelation("gost_types","id","nam",this);
    relGostType->setEditable(true);
    relIso = new DbSqlRelation("iso_types","id","nam",this);
    relIso->setEditable(true);
    relAws = new DbSqlRelation("aws_types","id","nam",this);
    relAws->setEditable(true);
    relZnam = new DbSqlRelation("denominator","id","nam",this);
    relZnam->setEditable(true);
    relBukv = new DbSqlRelation("purpose","id","nam",this);
    relBukv->setEditable(true);
    relDiam = new DbSqlRelation("diam","id","sdim",this);
    relDiam->setFilter("diam.is_el<>0");
    relPlav = new DbSqlRelation("el_plav_nams","id","nam",this);
    relPlav->setEditable(true);
    relWireDiam = new DbSqlRelation("diam","id","sdim",this);
    relWireDiam->setFilter("diam.id<>0");
    relChemDev = new DbSqlRelation("chem_dev","id","short",this);
    relPack = new DbSqlRelation("el_pack","id","pack_ed",this);
    relEanEd = new DbSqlRelation("eans","ean","ean",this);
    relEanEd->setAlias("eansEd");
    relEanGr = new DbSqlRelation("eans","ean","ean",this);
    relEanGr->setAlias("eansGr");
    relVar = new DbSqlRelation("elrtr_vars","id","nam",this);
    relVar->setSort("elrtr_vars.id");
    relVar->setEditable(true);
    relDocType = new DbSqlRelation("zvd_doc_type","id","nam",this);
    relDocType->setEditable(true);
}

void Rels::refresh()
{
    emit sigRefresh();
}

void Rels::refreshElDim()
{
    QSqlQuery query;
    query.prepare("select * from rx_els()");
    if (query.exec()){
        relElDim->refreshModel();
    } else {
        QMessageBox::critical(nullptr,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}
