#ifndef FORMDOC_H
#define FORMDOC_H

#include <QWidget>
#include "db/dbmapper.h"
#include "db/dbviewer.h"
#include "rels.h"
#include "modeldoc.h"

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
    ModelDoc *modelDoc;
    DbTableModel *modelEl;
    DbTableModel *modelElDim;
    DbTableModel *modelWireDiam;
    DbTableModel *modelWire;
    DbTableModel *modelTu;
    DbMapper *mapper;
    void loadsettings();
    void savesettings();
    void setOk(bool ok);
    int currentId();
    QString currentDocNumber();
    QString currentFname;

private slots:
    void refreshData(int index);
    void updElDim();
    void upload();
    void viewSert();
    void saveAs();
    void updState();
    void delSert();
};

#endif // FORMDOC_H
