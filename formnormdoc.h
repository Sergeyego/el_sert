#ifndef FORMNORMDOC_H
#define FORMNORMDOC_H

#include <QWidget>
#include <QSqlQueryModel>
#include "db/dbtablemodel.h"
#include "rels.h"

namespace Ui {
class FormNormDoc;
}

class FormNormDoc : public QWidget
{
    Q_OBJECT

public:
    explicit FormNormDoc(QWidget *parent = 0);
    ~FormNormDoc();

private:
    Ui::FormNormDoc *ui;
    DbTableModel *modelGostEl;
    int ideTu;
    void loadSettings();
    void saveSettings();
    QSqlQueryModel *sertModel;

private slots:
    void refreshGostEl(QModelIndex index);
    void copyTu();
    void pasteTu();
    void updElDim();
    void updateFinished();
};

#endif // FORMNORMDOC_H
