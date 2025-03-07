#ifndef FORMSHIP_H
#define FORMSHIP_H

#include <QWidget>
#include <QFileDialog>
#include <QProgressDialog>
#include "editor.h"
#include "sertbuild.h"
#include "modelship.h"
#include "reader.h"

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
