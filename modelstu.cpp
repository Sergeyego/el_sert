#include "modelstu.h"

ModelChem::ModelChem(QString tname, QObject *parent) : DbTableModel(tname,parent), tablename(tname)
{
    colIdPart=0;
    colIdChem=1;
    colVal=2;
}

QVariant ModelChem::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role==Qt::BackgroundColorRole){
        return getColor(index);
    }
    if (role==Qt::ToolTipRole){
        return getToolTip(index);
    }
    return DbTableModel::data(index,role);
}

ModelChem::~ModelChem()
{

}

void ModelChem::refresh(int id_part)
{
    map.clear();
    QSqlQuery query;
    QString qu = tuQuery;
    qu.replace(":id",QString::number(id_part));
    query.prepare(qu);
    if (query.exec()){
        while (query.next()){
            range r;
            r.min=query.value(1);
            r.max=query.value(2);
            map.insert(query.value(0).toInt(),r);
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
    setFilter(flt+" = "+QString::number(id_part));
    setDefaultValue(colIdPart,id_part);
    select();
}

QColor ModelChem::getColor(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QColor();
    }
    int id_chem=data(this->index(index.row(),colIdChem),Qt::EditRole).toInt();
    double val=data(this->index(index.row(),colVal),Qt::EditRole).toDouble();
    range r=map.value(id_chem);
    QColor color=Qt::white;
    if (!r.min.isNull() || !r.max.isNull()){
        if (!r.max.isNull()){
            if ((val>r.min.toDouble()) && (val<r.max.toDouble())){
                color=QColor(170,255,170);
            } else if ((val<r.min.toDouble()) || (val>r.max.toDouble())){
                color=QColor(255,170,170);
            } else {
                color=Qt::yellow;
            }
        } else {
            if (val<r.min.toDouble()){
                color=QColor(255,170,170);
            } else if (val>r.min.toDouble()){
                color=QColor(170,255,170);
            } else {
                color=Qt::yellow;
            }
        }
    }
    return color;
}

QString ModelChem::getToolTip(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QString();
    }
    int id_chem=data(this->index(index.row(),colIdChem),Qt::EditRole).toInt();
    QString el=data(this->index(index.row(),colIdChem),Qt::DisplayRole).toString();
    range r=map.value(id_chem);
    QString s;
    if (!r.min.isNull() || !r.max.isNull()){
        s+=el;
        if (r.max.isNull() && !r.min.isNull()){
            s+=QString(": min %1").arg(r.min.toDouble());
        } else if (!r.max.isNull() && r.min.isNull()){
            s+=QString(": max %1").arg(r.max.toDouble());
        } else {
            s+=QString(": min %1 max %2").arg(r.min.toDouble()).arg(r.max.toDouble());
        }
    }
    return s;
}

ModelChemSrc::ModelChemSrc(QObject *parent) : ModelChem("parti_chem",parent)
{
    addColumn("id","id");
    addColumn("id_part","id_part");
    addColumn("id_chem",tr("Элем."),Rels::instance()->relChem);
    addColumn("kvo",tr("Сод., %"));
    addColumn("id_dev",tr("Прибор"),Rels::instance()->relChemDev);
    addColumn("dt_cre",tr("Дата"));
    setSort(tablename+".id_chem, "+tablename+".dt_cre");
    flt=tableName+".id_part";
    setColumnFlags(5,Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    setDecimals(3,3);
    colIdPart=1;
    colIdChem=2;
    colVal=3;
    tuQuery="select c.id_chem, c.min, c.max from chem_tu as c where "
            "c.id_el = (select p.id_el from parti as p where p.id = :id ) "
            "and c.id_var = (select p.id_var from parti as p where p.id = :id )";
}

ModelChemSert::ModelChemSert(QObject *parent) : ModelChem("sert_chem",parent)
{
    addColumn("id_part","id_part");
    addColumn("id_chem",tr("Элем."),Rels::instance()->relChem);
    addColumn("value",tr("Сод., %"));
    setSort("sert_chem.id_chem");
    setDecimals(2,3);
    flt=tableName+".id_part";
    colIdPart=0;
    colIdChem=1;
    colVal=2;
    tuQuery="select c.id_chem, c.min, c.max from chem_tu as c where "
            "c.id_el = (select p.id_el from parti as p where p.id = :id ) "
            "and c.id_var = (select p.id_var from parti as p where p.id = :id )";
}

ModelMechSrc::ModelMechSrc(QObject *parent) : ModelChem("parti_mech",parent)
{
    addColumn("id_part","id_part");
    addColumn("id_mech",tr("Параметр"),Rels::instance()->relMech);
    addColumn("kvo",tr("Значение"));
    setSort("parti_mech.id_mech");
    setDecimals(2,2);
    flt=tableName+".id_part";
    colIdPart=0;
    colIdChem=1;
    colVal=2;
    tuQuery="select m.id_mech, m.min, m.max from mech_tu as m where "
            "m.id_el = (select p.id_el from parti as p where p.id = :id ) "
            "and m.id_var = (select p.id_var from parti as p where p.id = :id )";
}

ModelMechSert::ModelMechSert(QObject *parent) : ModelChem("sert_mech",parent)
{
    addColumn("id_part","id_part");
    addColumn("id_mech",tr("Параметр"),Rels::instance()->relMech);
    addColumn("value",tr("Значение"));
    setSort("sert_mech.id_mech");
    setDecimals(2,2);
    flt=tableName+".id_part";
    colIdPart=0;
    colIdChem=1;
    colVal=2;
    tuQuery="select m.id_mech, m.min, m.max from mech_tu as m where "
            "m.id_el = (select p.id_el from parti as p where p.id = :id ) "
            "and m.id_var = (select p.id_var from parti as p where p.id = :id )";
}
