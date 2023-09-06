/********************************************************************************
** Form generated from reading UI file 'dbrelationeditdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBRELATIONEDITDIALOG_H
#define UI_DBRELATIONEDITDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include "db/dbviewer.h"

QT_BEGIN_NAMESPACE

class Ui_DbRelationEditDialog
{
public:
    QVBoxLayout *verticalLayout;
    DbViewer *tableView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DbRelationEditDialog)
    {
        if (DbRelationEditDialog->objectName().isEmpty())
            DbRelationEditDialog->setObjectName(QString::fromUtf8("DbRelationEditDialog"));
        DbRelationEditDialog->resize(500, 600);
        verticalLayout = new QVBoxLayout(DbRelationEditDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableView = new DbViewer(DbRelationEditDialog);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        verticalLayout->addWidget(tableView);

        buttonBox = new QDialogButtonBox(DbRelationEditDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DbRelationEditDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), DbRelationEditDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DbRelationEditDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(DbRelationEditDialog);
    } // setupUi

    void retranslateUi(QDialog *DbRelationEditDialog)
    {
        DbRelationEditDialog->setWindowTitle(QCoreApplication::translate("DbRelationEditDialog", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \321\201\320\277\320\270\321\201\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DbRelationEditDialog: public Ui_DbRelationEditDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBRELATIONEDITDIALOG_H
