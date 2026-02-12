#ifndef FORMSIGPASPORT_H
#define FORMSIGPASPORT_H

#include <QWidget>
#include "db/dbtablemodel.h"
#include "rels.h"

namespace Ui {
class FormSigPasport;
}

class FormSigPasport : public QWidget
{
    Q_OBJECT

public:
    explicit FormSigPasport(QWidget *parent = nullptr);
    ~FormSigPasport();

private:
    Ui::FormSigPasport *ui;
    DbTableModel *modelSig;
};

#endif // FORMSIGPASPORT_H
