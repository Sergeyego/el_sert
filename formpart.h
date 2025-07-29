#ifndef FORMPART_H
#define FORMPART_H

#include <QWidget>
#include "editor.h"
#include "sertbuild.h"
#include "db/dbtablemodel.h"
#include <QSqlQueryModel>
#include <QPrintDialog>
#include "checkform.h"
#include "rels.h"
#include "modelstu.h"
#include "lblcreator.h"
#include "dialogcopy.h"
#include "modelro.h"
#include "reader.h"

class ModelPart : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelPart (QObject *parent = 0);
    void refresh(QDate dbeg, QDate dend, int id_el);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
private:
    QDate dateBeg;
    QDate dateEnd;
    QMap <int,int> mapStat;
    Executor *stateExecutor;
private slots:
    void refreshStateFinished();
public slots:
    void refreshState();
};

namespace Ui {
class FormPart;
}

class FormPart : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormPart(QWidget *parent = 0);
    ~FormPart();
    
private:
    Ui::FormPart *ui;
    ModelPart *modelPart;
    SertBuild *sertificatPart;
    Editor *editorPart;
    Reader *readerPart;

    ModelChemSrc *modelSrcChem;
    ModelChemSert *modelSertChem;
    ModelMechSrc *modelSrcMech;
    ModelMechSert *modelSertMech;
    DbTableModel *modelSertMechx;
    ModelRo *modelSrcGost;

    ModelRo *modelAdd;

    void loadPrim(int id_part);
    void loadAdd(int id_part);
    void loadGost(int id_part);
    int currentIdPart();
    void loadsettings();
    void savesettings();
    void setLock(bool b);

private slots:
    void refresh();
    void refreshPartSert(QModelIndex index);
    void savePrim();
    void saveZnam();
    void saveOk();
    void genChem();
    void genMech();
    void copyMechForward();
    void copyZnam();
    void showPartSert();
    void showShipSert(QModelIndex index);
    void showCheckForm();
    void genLbl();
    void enPrimSave();
    void enZnamSave();
    void copyVal();
    void findPart(int id_part, QString prefix);
};

#endif // FORMPART_H
