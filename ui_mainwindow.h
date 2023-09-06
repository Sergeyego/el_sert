/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionPart;
    QAction *actionSert;
    QAction *actionVed;
    QAction *actionDoc;
    QAction *actionMark;
    QAction *actionPos;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1014, 720);
        actionPart = new QAction(MainWindow);
        actionPart->setObjectName(QString::fromUtf8("actionPart"));
        actionPart->setChecked(false);
        actionSert = new QAction(MainWindow);
        actionSert->setObjectName(QString::fromUtf8("actionSert"));
        actionVed = new QAction(MainWindow);
        actionVed->setObjectName(QString::fromUtf8("actionVed"));
        actionDoc = new QAction(MainWindow);
        actionDoc->setObjectName(QString::fromUtf8("actionDoc"));
        actionMark = new QAction(MainWindow);
        actionMark->setObjectName(QString::fromUtf8("actionMark"));
        actionPos = new QAction(MainWindow);
        actionPos->setObjectName(QString::fromUtf8("actionPos"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(true);
        tabWidget->setMovable(true);

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1014, 25));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName(QString::fromUtf8("menu_4"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu_4->menuAction());
        menu->addAction(actionPart);
        menu_2->addAction(actionSert);
        menu_3->addAction(actionVed);
        menu_3->addAction(actionDoc);
        menu_4->addAction(actionMark);
        menu_4->addAction(actionPos);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\241\320\265\321\200\321\202\320\270\321\204\320\270\320\272\320\260\321\202\321\213 \320\272\320\260\321\207\320\265\321\201\321\202\320\262\320\260", nullptr));
        actionPart->setText(QCoreApplication::translate("MainWindow", "\320\237\320\260\321\200\321\202\320\270\320\270", nullptr));
#if QT_CONFIG(shortcut)
        actionPart->setShortcut(QCoreApplication::translate("MainWindow", "F1", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSert->setText(QCoreApplication::translate("MainWindow", "\320\241\320\265\321\200\321\202\320\270\321\204\320\270\320\272\320\260\321\202\321\213", nullptr));
#if QT_CONFIG(shortcut)
        actionSert->setShortcut(QCoreApplication::translate("MainWindow", "F5", nullptr));
#endif // QT_CONFIG(shortcut)
        actionVed->setText(QCoreApplication::translate("MainWindow", "\320\222\320\265\320\264\320\276\320\274\321\201\321\202\320\262\320\260 \320\270 \320\262\320\270\320\264\321\213 \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202\320\276\320\262", nullptr));
#if QT_CONFIG(shortcut)
        actionVed->setShortcut(QCoreApplication::translate("MainWindow", "F6", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDoc->setText(QCoreApplication::translate("MainWindow", "\320\241\320\262\320\270\320\264\320\265\321\202\320\265\320\273\321\214\321\201\321\202\320\262\320\260 \320\270 \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202\321\213", nullptr));
#if QT_CONFIG(shortcut)
        actionDoc->setShortcut(QCoreApplication::translate("MainWindow", "F2", nullptr));
#endif // QT_CONFIG(shortcut)
        actionMark->setText(QCoreApplication::translate("MainWindow", "\320\234\320\260\321\200\320\272\320\270 \321\215\320\273\320\265\320\272\321\202\321\200\320\276\320\264\320\276\320\262", nullptr));
#if QT_CONFIG(shortcut)
        actionMark->setShortcut(QCoreApplication::translate("MainWindow", "F4", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPos->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\273\320\276\320\266\320\265\320\275\320\270\321\217 \320\277\321\200\320\270 \321\201\320\262\320\260\321\200\320\272\320\265", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\276\320\270\320\267\320\262\320\276\320\264\321\201\321\202\320\262\320\276", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\263\321\200\321\203\320\267\320\272\320\270", nullptr));
        menu_3->setTitle(QCoreApplication::translate("MainWindow", "\320\241\320\265\321\200\321\202\320\270\321\204\320\270\320\272\320\260\321\206\320\270\321\217", nullptr));
        menu_4->setTitle(QCoreApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\276\321\207\320\275\320\270\320\272\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
