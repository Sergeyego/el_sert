/********************************************************************************
** Form generated from reading UI file 'formship.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMSHIP_H
#define UI_FORMSHIP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "db/dbdateedit.h"

QT_BEGIN_NAMESPACE

class Ui_FormShip
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    DbDateEdit *dateEditBeg;
    QLabel *label_2;
    DbDateEdit *dateEditEnd;
    QPushButton *cmdUpd;
    QSpacerItem *horizontalSpacer_3;
    QSplitter *splitterVert;
    QSplitter *splitter;
    QTableView *tableViewShip;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QTableView *tableViewShipData;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *cmdPrintAll;
    QPushButton *cmdSaveAll;
    QPushButton *cmdMultipagePdf;
    QSpacerItem *horizontalSpacer;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayoutSert;

    void setupUi(QWidget *FormShip)
    {
        if (FormShip->objectName().isEmpty())
            FormShip->setObjectName(QString::fromUtf8("FormShip"));
        FormShip->resize(1090, 751);
        verticalLayout = new QVBoxLayout(FormShip);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(FormShip);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        dateEditBeg = new DbDateEdit(FormShip);
        dateEditBeg->setObjectName(QString::fromUtf8("dateEditBeg"));
        dateEditBeg->setCalendarPopup(true);

        horizontalLayout->addWidget(dateEditBeg);

        label_2 = new QLabel(FormShip);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_2);

        dateEditEnd = new DbDateEdit(FormShip);
        dateEditEnd->setObjectName(QString::fromUtf8("dateEditEnd"));
        dateEditEnd->setCalendarPopup(true);

        horizontalLayout->addWidget(dateEditEnd);

        cmdUpd = new QPushButton(FormShip);
        cmdUpd->setObjectName(QString::fromUtf8("cmdUpd"));

        horizontalLayout->addWidget(cmdUpd);

        horizontalSpacer_3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout);

        splitterVert = new QSplitter(FormShip);
        splitterVert->setObjectName(QString::fromUtf8("splitterVert"));
        splitterVert->setOrientation(Qt::Horizontal);
        splitter = new QSplitter(splitterVert);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        tableViewShip = new QTableView(splitter);
        tableViewShip->setObjectName(QString::fromUtf8("tableViewShip"));
        tableViewShip->setSelectionMode(QAbstractItemView::SingleSelection);
        tableViewShip->setSelectionBehavior(QAbstractItemView::SelectRows);
        splitter->addWidget(tableViewShip);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        tableViewShipData = new QTableView(layoutWidget);
        tableViewShipData->setObjectName(QString::fromUtf8("tableViewShipData"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableViewShipData->sizePolicy().hasHeightForWidth());
        tableViewShipData->setSizePolicy(sizePolicy1);
        tableViewShipData->setSelectionMode(QAbstractItemView::SingleSelection);
        tableViewShipData->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_2->addWidget(tableViewShipData);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        cmdPrintAll = new QPushButton(layoutWidget);
        cmdPrintAll->setObjectName(QString::fromUtf8("cmdPrintAll"));

        horizontalLayout_2->addWidget(cmdPrintAll);

        cmdSaveAll = new QPushButton(layoutWidget);
        cmdSaveAll->setObjectName(QString::fromUtf8("cmdSaveAll"));

        horizontalLayout_2->addWidget(cmdSaveAll);

        cmdMultipagePdf = new QPushButton(layoutWidget);
        cmdMultipagePdf->setObjectName(QString::fromUtf8("cmdMultipagePdf"));

        horizontalLayout_2->addWidget(cmdMultipagePdf);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_2);

        splitter->addWidget(layoutWidget);
        splitterVert->addWidget(splitter);
        verticalLayoutWidget = new QWidget(splitterVert);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutSert = new QVBoxLayout(verticalLayoutWidget);
        verticalLayoutSert->setObjectName(QString::fromUtf8("verticalLayoutSert"));
        verticalLayoutSert->setContentsMargins(0, 0, 0, 0);
        splitterVert->addWidget(verticalLayoutWidget);

        verticalLayout->addWidget(splitterVert);


        retranslateUi(FormShip);

        QMetaObject::connectSlotsByName(FormShip);
    } // setupUi

    void retranslateUi(QWidget *FormShip)
    {
        FormShip->setWindowTitle(QCoreApplication::translate("FormShip", "Form", nullptr));
        label->setText(QCoreApplication::translate("FormShip", "C", nullptr));
        label_2->setText(QCoreApplication::translate("FormShip", "\320\237\320\276", nullptr));
        cmdUpd->setText(QCoreApplication::translate("FormShip", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        cmdPrintAll->setText(QCoreApplication::translate("FormShip", "\320\237\320\265\321\207\320\260\321\202\321\214 \320\262\321\201\320\265", nullptr));
        cmdSaveAll->setText(QCoreApplication::translate("FormShip", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\262\321\201\320\265", nullptr));
        cmdMultipagePdf->setText(QCoreApplication::translate("FormShip", "\320\241\320\276\321\205\321\200. \320\274\320\275\320\276\320\263\320\276\321\201\321\202\321\200\320\260\320\275. PDF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormShip: public Ui_FormShip {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMSHIP_H
