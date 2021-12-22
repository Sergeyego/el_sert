#ifndef DIALOGCOPY_H
#define DIALOGCOPY_H

#include <QDialog>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <modelstu.h>

namespace Ui {
class DialogCopy;
}

class DialogCopy : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCopy(int id_p, QWidget *parent = 0);
    ~DialogCopy();
    int idPart();

private:
    Ui::DialogCopy *ui;
    QSqlQueryModel *modelPart;
    int id_partsrc;
    ModelChemSert *modelChem;
    ModelMechSert *modelMech;

private slots:
    void setQueryPart();
    void frmPart();
    void updData(QModelIndex index);
};

#endif // DIALOGCOPY_H
