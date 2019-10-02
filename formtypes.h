#ifndef FORMTYPES_H
#define FORMTYPES_H

#include <QWidget>
#include "rels.h"

namespace Ui {
class FormTypes;
}

class FormTypes : public QWidget
{
    Q_OBJECT

public:
    explicit FormTypes(QWidget *parent = 0);
    ~FormTypes();

private:
    Ui::FormTypes *ui;
};

#endif // FORMTYPES_H
