#ifndef FORMDOC_H
#define FORMDOC_H

#include <QWidget>
#include "db/dbmapper.h"
#include "db/dbviewer.h"
#include "rels.h"

namespace Ui {
class FormDoc;
}

class FormDoc : public QWidget
{
    Q_OBJECT

public:
    explicit FormDoc(QWidget *parent = 0);
    ~FormDoc();

private:
    Ui::FormDoc *ui;
    DbTableModel *modelDoc;
    DbTableModel *modelElTypes;
    DbTableModel *modelEl;
    DbTableModel *modelElDim;
    DbMapper *mapper;
    void loadsettings();
    void savesettings();

private slots:
    void refreshData(int index);
    void selectDoc(bool active);
    void updElDim();
};

#endif // FORMDOC_H
