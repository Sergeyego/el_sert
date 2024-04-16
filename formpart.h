#ifndef FORMPART_H
#define FORMPART_H

#include <QWidget>
#include "modelship.h"
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

    ModelChemSrc *modelSrcChem;
    ModelChemSert *modelSertChem;
    ModelMechSrc *modelSrcMech;
    ModelMechSert *modelSertMech;
    DbTableModel *modelSertMechx;
    ModelRo *modelSrcGost;

    QSqlQueryModel *modelAdd;

    void loadPrim(int id_part);
    void loadAdd(int id_part);
    void loadGost(int id_part);
    int currentIdPart();
    void loadsettings();
    void savesettings();

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
    void genLblSmall();
    void genLblSmall2();
    void enPrimSave();
    void enZnamSave();
    void copyVal();
    void findPart(int id_part);
};

#endif // FORMPART_H
