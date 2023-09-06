/********************************************************************************
** Form generated from reading UI file 'formved.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMVED_H
#define UI_FORMVED_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "db/dbviewer.h"

QT_BEGIN_NAMESPACE

class Ui_FormVed
{
public:
    QVBoxLayout *verticalLayout_5;
    QPushButton *pushButtonUpd;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    DbViewer *tableViewVed;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelSimb;
    QHBoxLayout *horizontalLayout;
    QPushButton *cmdDel;
    QPushButton *cmdBrowse;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    DbViewer *tableViewDocVid;

    void setupUi(QWidget *FormVed)
    {
        if (FormVed->objectName().isEmpty())
            FormVed->setObjectName(QString::fromUtf8("FormVed"));
        FormVed->resize(1083, 690);
        verticalLayout_5 = new QVBoxLayout(FormVed);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        pushButtonUpd = new QPushButton(FormVed);
        pushButtonUpd->setObjectName(QString::fromUtf8("pushButtonUpd"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButtonUpd->sizePolicy().hasHeightForWidth());
        pushButtonUpd->setSizePolicy(sizePolicy);

        verticalLayout_5->addWidget(pushButtonUpd);

        groupBox = new QGroupBox(FormVed);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        tableViewVed = new DbViewer(groupBox);
        tableViewVed->setObjectName(QString::fromUtf8("tableViewVed"));
        tableViewVed->setSelectionMode(QAbstractItemView::SingleSelection);
        tableViewVed->setSelectionBehavior(QAbstractItemView::SelectRows);

        horizontalLayout_2->addWidget(tableViewVed);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        labelSimb = new QLabel(groupBox);
        labelSimb->setObjectName(QString::fromUtf8("labelSimb"));
        labelSimb->setMinimumSize(QSize(200, 200));
        labelSimb->setMaximumSize(QSize(200, 300));

        verticalLayout_2->addWidget(labelSimb);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cmdDel = new QPushButton(groupBox);
        cmdDel->setObjectName(QString::fromUtf8("cmdDel"));

        horizontalLayout->addWidget(cmdDel);

        cmdBrowse = new QPushButton(groupBox);
        cmdBrowse->setObjectName(QString::fromUtf8("cmdBrowse"));

        horizontalLayout->addWidget(cmdBrowse);


        verticalLayout_2->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout_2);


        verticalLayout_4->addLayout(horizontalLayout_2);


        verticalLayout_5->addWidget(groupBox);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        groupBox_2 = new QGroupBox(FormVed);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableViewDocVid = new DbViewer(groupBox_2);
        tableViewDocVid->setObjectName(QString::fromUtf8("tableViewDocVid"));

        verticalLayout->addWidget(tableViewDocVid);


        horizontalLayout_3->addWidget(groupBox_2);


        verticalLayout_5->addLayout(horizontalLayout_3);


        retranslateUi(FormVed);

        QMetaObject::connectSlotsByName(FormVed);
    } // setupUi

    void retranslateUi(QWidget *FormVed)
    {
        FormVed->setWindowTitle(QCoreApplication::translate("FormVed", "Form", nullptr));
        pushButtonUpd->setText(QCoreApplication::translate("FormVed", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FormVed", "\320\222\320\265\320\264\320\276\320\274\321\201\321\202\320\262\320\260", nullptr));
        labelSimb->setText(QString());
        cmdDel->setText(QCoreApplication::translate("FormVed", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\273\320\276\320\263\320\276\321\202\320\270\320\277", nullptr));
        cmdBrowse->setText(QCoreApplication::translate("FormVed", "\320\236\320\261\320\267\320\276\321\200", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("FormVed", "\320\222\320\270\320\264\321\213 \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202\320\276\320\262", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormVed: public Ui_FormVed {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMVED_H
