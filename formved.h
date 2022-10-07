#ifndef FORMVED_H
#define FORMVED_H

#include <QWidget>
#include <QFileDialog>
#include "db/dbtablemodel.h"
#include "rels.h"

namespace Ui {
class FormVed;
}

class FormVed : public QWidget
{
    Q_OBJECT

public:
    explicit FormVed(QWidget *parent = 0);
    ~FormVed();

private:
    Ui::FormVed *ui;
    void viewPix(const QPixmap &pix);
    DbTableModel *modelVed;
    DbTableModel *modelDoc;

private slots:
    void loadImg(QModelIndex index);
    void newImg();
    void delImg();
    void upd();
};

#endif // FORMVED_H
