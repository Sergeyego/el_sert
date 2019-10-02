#ifndef FORMPOS_H
#define FORMPOS_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include "db/dbtablemodel.h"
#include "rels.h"

namespace Ui {
class FormPos;
}

class FormPos : public QWidget
{
    Q_OBJECT

public:
    explicit FormPos(QWidget *parent = 0);
    ~FormPos();

private:
    Ui::FormPos *ui;
    DbTableModel *modelPos;
    void viewPix(const QPixmap &pix);

private slots:
    void loadImg(QModelIndex index);
    void newImg();
    void delImg();
};

#endif // FORMPOS_H
