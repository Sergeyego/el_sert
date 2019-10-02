#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "formpart.h"
#include "tabwidget.h"
#include "formship.h"
#include "formnormdoc.h"
#include "formved.h"
#include "formdoc.h"
#include "formmark.h"
#include "formtypes.h"
#include "formpos.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(bool readOnly=false, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void loadSettings();
    void saveSettings();
    FormPart *formPart;
    FormShip *formShip;
    FormNormDoc *formNormDoc;
    FormVed *formVed;
    FormDoc *formDoc;
    FormMark *formMark;
    FormTypes *formTypes;
    FormPos *formPos;
    TabWidget *tabWidget;    
};

#endif // MAINWINDOW_H
