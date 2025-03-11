#ifndef FORMPARTWIRE_H
#define FORMPARTWIRE_H

#include <QWidget>
#include <QDataWidgetMapper>
#include "rels.h"
#include "modelro.h"

class ModelPartWire : public ModelRo
{    Q_OBJECT

public:
    explicit ModelPartWire(QWidget *parent = nullptr);
    void refresh(QDate beg, QDate end, int id_provol);
    QVariant data(const QModelIndex &item, int role) const;

public slots:
    void refreshState();

private:
    QDate dateBeg;
    QDate dateEnd;
    QMap <int,int> colorState;
};

namespace Ui {
class FormPartWire;
}

class FormPartWire : public QWidget
{
    Q_OBJECT

public:
    explicit FormPartWire(QWidget *parent = nullptr);
    ~FormPartWire();

private:
    Ui::FormPartWire *ui;
    ModelPartWire *modelPartWire;
    QDataWidgetMapper *mapper;
    void loadSettings();
    void saveSettings();

private slots:
    void upd();
    void currentIndexChanged(QModelIndex index);
};

#endif // FORMPARTWIRE_H
