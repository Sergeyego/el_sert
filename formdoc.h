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

class ModelEn : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelEn(QWidget *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void refresh(int id_doc);
    QVariant data(const QModelIndex &item, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

public slots:
    void checkAll();
    void unCheckAll();

private:
    int id_d;
    QList<int> disList;
    bool writeSql(int id_var, bool value);
};

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
    ModelEn *modelEn;

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
