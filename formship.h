#ifndef FORMSHIP_H
#define FORMSHIP_H

#include <QWidget>
#include <QFileDialog>
#include <QProgressDialog>
#include "editor.h"
#include "sertbuild.h"
#include "reader.h"
#include "modelro.h"

class ModelShip : public ModelRo
{
    Q_OBJECT
public:
    ModelShip(QObject *parent);
    void refresh(QDate begDate, QDate endDate);
};

class ModelDataShip : public ModelRo
{
    Q_OBJECT
public:
    ModelDataShip(QObject *parent);
    void refresh(int id_ship);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

namespace Ui {
class FormShip;
}

class FormShip : public QWidget
{
    Q_OBJECT

public:
    explicit FormShip(QWidget *parent = 0);
    ~FormShip();

private:
    Ui::FormShip *ui;
    Editor *editor;
    Reader *reader;
    SertBuild *sertificat;
    ModelShip *modelShip;
    ModelDataShip *modelDataShip;
    void loadSettings();
    void saveSettings();
private slots:
    void refreshDataShip(QModelIndex index);
    void refreshShipSert(QModelIndex index);
    void printAll();
    void printAll(QPagedPaintDevice *printer);
    void pdfAll();
    void refresh();
    void partReq(QModelIndex index);
    void signFinished();
    void reloadDataShip();
    void signAll();
    void signPdfAll();
    void signPrintAll();
};

#endif // FORMSHIP_H
