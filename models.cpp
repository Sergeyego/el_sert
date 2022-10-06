#include "models.h"


ModelEan::ModelEan(QObject *parent) : DbTableModel("ean_el",parent)
{
    addColumn("id_el","id_el");
    addColumn("id_diam",tr("Диаметр"),Rels::instance()->relDiam);
    addColumn("id_pack",tr("Упаковка (ед., групп.)"),Rels::instance()->relPack);
    addColumn("ean_ed",tr("Штрих код (ед.)"),Rels::instance()->relEanEd);
    addColumn("ean_group",tr("Штрих код (гр.)"),Rels::instance()->relEanGr);
    //setSuffix("inner join diam on ean_el.id_diam=diam.id");
    setSort("diam.sdim");
}

void ModelEan::refresh(int id_el)
{
    setDefaultValue(0,id_el);
    setFilter("ean_el.id_el = "+QString::number(id_el));
    select();
}

void ModelEan::updRels(QModelIndex index)
{
    if (index.column()==3 || index.column()==4){
        QString flt;
        QSqlQuery query;
        query.prepare("select e.ean from eans as e where e.ean not in "
                      "(select ean_ed from wire_ean "
                      "union "
                      "select ean_group from wire_ean where ean_group is not null "
                      "union "
                      "select ean_ed from ean_el "
                      "union "
                      "select ean_group from ean_el where ean_group is not null) "
                      "order by e.ean ");
        if (query.exec()){
            while (query.next()){
                if (!flt.isEmpty()){
                    flt+="|";
                }
                flt+=query.value(0).toString();
            }
            Rels::instance()->relEanEd->setFilterRegExp(flt);
            Rels::instance()->relEanGr->setFilterRegExp(flt);
        } else {
            QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
        }
    }
}
