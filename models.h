#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "rels.h"

class ModelEan : public DbTableModel
{
    Q_OBJECT
public:
    ModelEan(QObject *parent=0);
    void refresh(int id_el);
public slots:
    void updRels(QModelIndex index);
};

#endif // MODELS_H
