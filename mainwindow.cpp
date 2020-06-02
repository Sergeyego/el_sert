#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(bool readOnly, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();
    tabWidget = new TabWidget(this);
    this->setCentralWidget(tabWidget);

    formPart = new FormPart();
    formShip = new FormShip();
    formNormDoc = new FormNormDoc();
    formVed = new FormVed();
    formDoc = new FormDoc();
    formMark = new FormMark();
    formTypes = new FormTypes();
    formPos = new FormPos();

    tabWidget->addTabAction(formPart,ui->actionPart,!readOnly);
    tabWidget->addTabAction(formShip,ui->actionSert);
    tabWidget->addTabAction(formNormDoc,ui->actionNormDoc,!readOnly);
    tabWidget->addTabAction(formVed,ui->actionVed,!readOnly);
    tabWidget->addTabAction(formDoc,ui->actionDoc,!readOnly);
    tabWidget->addTabAction(formMark,ui->actionMark,!readOnly);
    tabWidget->addTabAction(formTypes,ui->actionType,!readOnly);
    tabWidget->addTabAction(formPos,ui->actionPos,!readOnly);

    tabWidget->loadSettings();

    connect(ui->actionUpd,SIGNAL(triggered(bool)),Rels::instance(),SLOT(refresh()));
}

MainWindow::~MainWindow()
{
    saveSettings();
    tabWidget->saveSettings();

    delete ui;

    delete formPart;
    delete formShip;
    delete formNormDoc;
    delete formVed;
    delete formDoc;
    delete formMark;
    delete formTypes;
    delete formPos;
}

void MainWindow::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->restoreGeometry(settings.value("main_geometry").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("main_geometry", this->saveGeometry());
}
