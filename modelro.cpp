#include "modelro.h"

ModelRo::ModelRo(QObject *parent) : QSqlQueryModel(parent)
{
    dec=2;
}

QVariant ModelRo::data(const QModelIndex &item, int role) const
{
    QVariant origData=QSqlQueryModel::data(item,Qt::EditRole);
    QVariant::Type type=origData.type();
    if (role==Qt::DisplayRole){
        if (type==QMetaType::Double){
            return (origData.isNull()) ? QString("") : QLocale().toString(origData.toDouble(),'f',dec);
        } else if (type==QMetaType::QDate){
            return (origData.isNull()) ? QString("") : origData.toDate().toString("dd.MM.yy");
        }
    } else if (role==Qt::TextAlignmentRole){
        return (type==QMetaType::Int || type==QMetaType::Double || type==QMetaType::Float || type==QMetaType::LongLong ) ?
        int(Qt::AlignRight | Qt::AlignVCenter) : int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    return QSqlQueryModel::data(item,role);
}

bool ModelRo::execQuery(QSqlQuery &query)
{
    bool ok=query.exec();
    if (ok){
        setQuery(query);
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
        clear();
    }
    return ok;
}

void ModelRo::setQuery(const QSqlQuery &query)
{
    QSqlQueryModel::setQuery(query);
    emit newQuery();
}

void ModelRo::select()
{
    QSqlQuery q(query());
    execQuery(q);
}
