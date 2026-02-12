#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "formpart.h"
#include "formship.h"
#include "formved.h"
#include "formdoc.h"
#include "formmark.h"
#include "formpos.h"
#include "formpartwire.h"
#include <QNetworkAccessManager>
#include "progressreportdialog.h"
#include "formsigpasport.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(bool readOnly=false, QWidget *parent = 0);
    ~MainWindow();
    bool exist(QObject *a);
    void actAction(QAction *a, void (MainWindow::*sl)());
    void addSubWindow(QWidget *w, QObject *a);
    bool setActiveSubWindow(QString t);
    QMap <QString,QAction*> actions;

private slots:
    void closeTab(int index);

    void partEl();
    void partWire();
    void ship();
    void ved();
    void doc();
    void mark();
    void pos();
    void partReq(int id_part, QString prefix);
    void updSite();
    void updSiteFinished();
    void sigPastort();

private:
    Ui::MainWindow *ui;
    void loadSettings();
    void saveSettings();
    QNetworkAccessManager *manager;
    ProgressReportDialog *prd;
};

#endif // MAINWINDOW_H
