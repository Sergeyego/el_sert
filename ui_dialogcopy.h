/********************************************************************************
** Form generated from reading UI file 'dialogcopy.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCOPY_H
#define UI_DIALOGCOPY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include "db/dbviewer.h"

QT_BEGIN_NAMESPACE

class Ui_DialogCopy
{
public:
    QVBoxLayout *verticalLayout;
    QSplitter *splitter_2;
    QTableView *tableViewPart;
    QSplitter *splitter;
    DbViewer *tableViewChem;
    DbViewer *tableViewMech;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogCopy)
    {
        if (DialogCopy->objectName().isEmpty())
            DialogCopy->setObjectName(QString::fromUtf8("DialogCopy"));
        DialogCopy->resize(800, 520);
        verticalLayout = new QVBoxLayout(DialogCopy);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        splitter_2 = new QSplitter(DialogCopy);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        tableViewPart = new QTableView(splitter_2);
        tableViewPart->setObjectName(QString::fromUtf8("tableViewPart"));
        tableViewPart->setSelectionMode(QAbstractItemView::SingleSelection);
        tableViewPart->setSelectionBehavior(QAbstractItemView::SelectRows);
        splitter_2->addWidget(tableViewPart);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        tableViewChem = new DbViewer(splitter);
        tableViewChem->setObjectName(QString::fromUtf8("tableViewChem"));
        tableViewChem->setEditTriggers(QAbstractItemView::NoEditTriggers);
        splitter->addWidget(tableViewChem);
        tableViewMech = new DbViewer(splitter);
        tableViewMech->setObjectName(QString::fromUtf8("tableViewMech"));
        tableViewMech->setEditTriggers(QAbstractItemView::NoEditTriggers);
        splitter->addWidget(tableViewMech);
        splitter_2->addWidget(splitter);

        verticalLayout->addWidget(splitter_2);

        buttonBox = new QDialogButtonBox(DialogCopy);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogCopy);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogCopy, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogCopy, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogCopy);
    } // setupUi

    void retranslateUi(QDialog *DialogCopy)
    {
        DialogCopy->setWindowTitle(QCoreApplication::translate("DialogCopy", "\320\230\320\274\320\277\320\276\321\200\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\267\320\275\320\260\321\207\320\265\320\275\320\270\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogCopy: public Ui_DialogCopy {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCOPY_H
