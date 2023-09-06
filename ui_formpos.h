/********************************************************************************
** Form generated from reading UI file 'formpos.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMPOS_H
#define UI_FORMPOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "db/dbviewer.h"

QT_BEGIN_NAMESPACE

class Ui_FormPos
{
public:
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButtonUpd;
    QHBoxLayout *horizontalLayout_2;
    DbViewer *tableViewPos;
    QVBoxLayout *verticalLayout;
    QLabel *labelPos;
    QHBoxLayout *horizontalLayout;
    QPushButton *cmdLoad;
    QPushButton *cmdDel;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *FormPos)
    {
        if (FormPos->objectName().isEmpty())
            FormPos->setObjectName(QString::fromUtf8("FormPos"));
        FormPos->resize(1073, 742);
        verticalLayout_2 = new QVBoxLayout(FormPos);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButtonUpd = new QPushButton(FormPos);
        pushButtonUpd->setObjectName(QString::fromUtf8("pushButtonUpd"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButtonUpd->sizePolicy().hasHeightForWidth());
        pushButtonUpd->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(pushButtonUpd);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        tableViewPos = new DbViewer(FormPos);
        tableViewPos->setObjectName(QString::fromUtf8("tableViewPos"));
        tableViewPos->setSelectionMode(QAbstractItemView::SingleSelection);
        tableViewPos->setSelectionBehavior(QAbstractItemView::SelectRows);

        horizontalLayout_2->addWidget(tableViewPos);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPos = new QLabel(FormPos);
        labelPos->setObjectName(QString::fromUtf8("labelPos"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelPos->sizePolicy().hasHeightForWidth());
        labelPos->setSizePolicy(sizePolicy1);
        labelPos->setMinimumSize(QSize(200, 200));

        verticalLayout->addWidget(labelPos);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cmdLoad = new QPushButton(FormPos);
        cmdLoad->setObjectName(QString::fromUtf8("cmdLoad"));

        horizontalLayout->addWidget(cmdLoad);

        cmdDel = new QPushButton(FormPos);
        cmdDel->setObjectName(QString::fromUtf8("cmdDel"));

        horizontalLayout->addWidget(cmdDel);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 359, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        retranslateUi(FormPos);

        QMetaObject::connectSlotsByName(FormPos);
    } // setupUi

    void retranslateUi(QWidget *FormPos)
    {
        FormPos->setWindowTitle(QCoreApplication::translate("FormPos", "Form", nullptr));
        pushButtonUpd->setText(QCoreApplication::translate("FormPos", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        labelPos->setText(QString());
        cmdLoad->setText(QCoreApplication::translate("FormPos", "\320\236\320\261\320\267\320\276\321\200", nullptr));
        cmdDel->setText(QCoreApplication::translate("FormPos", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormPos: public Ui_FormPos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMPOS_H
