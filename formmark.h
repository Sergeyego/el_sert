#ifndef FORMMARK_H
#define FORMMARK_H

#include <QWidget>
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"
#include "db/dbrelationeditdialog.h"
#include "rels.h"
#include "lblcreator.h"
#include "export.h"

namespace Ui {
class FormMark;
}

class FormMark : public QWidget
{
    Q_OBJECT

public:
    explicit FormMark(QWidget *parent = 0);
    ~FormMark();

private:
    Ui::FormMark *ui;
    DbMapper *mapper;
    DbTableModel *modelMark;
    DbTableModel *modelAmp;
    DbTableModel *modelChemTu;
    DbTableModel *modelMechTu;
    DbTableModel *modelPlav;
    DbTableModel *modelEan;
    DbTableModel *modelProvol;
    DbTableModel *modelGost;
    DbTableModel *modelIntClass;

    void loadsettings();
    void savesettings();
    int id_el();
    int id_diam();
    int id_var();

private slots:
    void updEanRel(QModelIndex index);
    void refreshCont(int index);
    void updImg();
    void updVarsList();
    void gelLbl();
    void gelLblSmall();
    void gelLblSmall2();
    void exportXml();
    void createXml();
    void blockVar(bool b);
    void loadVars();
    void createVar();
    void saveVar();
    void deleteVar();
    void varChanged();
    void copyTableData();
    void upd();
    void edtVar();
};

class CustomDelegate : public DbDelegate
{
    Q_OBJECT
public:
    explicit CustomDelegate(QObject *parent=0);
    void setEditorData(QWidget *editor,const QModelIndex &index) const;
    void setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index) const;
};

#endif // FORMMARK_H
