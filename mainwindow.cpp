#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(bool readOnly, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!readOnly){
        actAction(ui->actionPartEl,&MainWindow::partEl);
        actAction(ui->actionPartWire,&MainWindow::partWire);
        actAction(ui->actionVed,&MainWindow::ved);
        actAction(ui->actionDoc,&MainWindow::doc);
        actAction(ui->actionMark,&MainWindow::mark);
        actAction(ui->actionPos,&MainWindow::pos);
    }
    actAction(ui->actionSert,&MainWindow::ship);

    loadSettings();

    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    connect(Rels::instance(),SIGNAL(partReq(int,QString)),this,SLOT(partReq(int,QString)));
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete Rels::instance();
    delete ui;

}

bool MainWindow::exist(QObject *a)
{
    bool b=false;
    QAction *action = qobject_cast<QAction *>(a);
    if (action){
        b=setActiveSubWindow(action->text());
    }
    return b;
}

void MainWindow::actAction(QAction *a, void (MainWindow::*sl)())
{
    connect(a, &QAction::triggered, this, sl);
    actions.insert(a->text(),a);
}

void MainWindow::addSubWindow(QWidget *w, QObject *a)
{
    w->setAttribute(Qt::WA_DeleteOnClose);
    QAction *action = qobject_cast<QAction *>(a);
    if (action){
        w->setWindowTitle(action->text());
    }
    ui->tabWidget->addTab(w,w->windowTitle());
    ui->tabWidget->setCurrentWidget(w);
}

bool MainWindow::setActiveSubWindow(QString t)
{
    bool b=false;
    for (int i=0; i<ui->tabWidget->count(); i++){
        if (ui->tabWidget->tabText(i)==t){
            ui->tabWidget->setCurrentIndex(i);
            b=true;
            break;
        }
    }
    return b;
}

void MainWindow::closeTab(int index)
{
    ui->tabWidget->widget(index)->close();
}

void MainWindow::partEl()
{
    if (!exist(sender())){
        addSubWindow(new FormPart(),sender());
    }
}

void MainWindow::partWire()
{
    if (!exist(sender())){
        addSubWindow(new FormPartWire(),sender());
    }
}

void MainWindow::ship()
{
    if (!exist(sender())){
        addSubWindow(new FormShip(),sender());
    }
}

void MainWindow::ved()
{
    if (!exist(sender())){
        addSubWindow(new FormVed(),sender());
    }
}

void MainWindow::doc()
{
    if (!exist(sender())){
        addSubWindow(new FormDoc(),sender());
    }
}

void MainWindow::mark()
{
    if (!exist(sender())){
        addSubWindow(new FormMark(),sender());
    }
}

void MainWindow::pos()
{
    if (!exist(sender())){
        addSubWindow(new FormPos(),sender());
    }
}

void MainWindow::partReq(int /*id_part*/, QString prefix)
{
    if (prefix=="elrtr"){
        ui->actionPartEl->trigger();
    } else {
        ui->actionPartWire->trigger();
    }
}

void MainWindow::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->restoreGeometry(settings.value("main_geometry").toByteArray());
    this->restoreState(settings.value("main_state").toByteArray());
    QString opentab=settings.value("main_opentab").toString();
    QString current=settings.value("main_currenttab").toString();

    if (opentab.isEmpty()){
        ui->actionPartEl->trigger();
    } else {
        QStringList l=opentab.split("|");
        foreach (QString a, l) {
            if (actions.contains(a)){
                actions.value(a)->trigger();
            }
        }
    }
    setActiveSubWindow(current);
}

void MainWindow::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("main_state", this->saveState());
    settings.setValue("main_geometry", this->saveGeometry());
    QString opentab, currenttab;
    for (int i=0; i<ui->tabWidget->count(); i++){
        if (!opentab.isEmpty()){
            opentab+="|";
        }
        opentab+=ui->tabWidget->tabText(i);
    }
    currenttab=ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    settings.setValue("main_opentab", opentab);
    settings.setValue("main_currenttab",currenttab);
}
