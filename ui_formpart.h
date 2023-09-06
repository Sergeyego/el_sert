/********************************************************************************
** Form generated from reading UI file 'formpart.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMPART_H
#define UI_FORMPART_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "db/dbcombobox.h"
#include "db/dbdateedit.h"
#include "db/dbviewer.h"

QT_BEGIN_NAMESPACE

class Ui_FormPart
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    DbDateEdit *dateEditBeg;
    QLabel *label_2;
    DbDateEdit *dateEditEnd;
    QPushButton *cmdUpd;
    QSpacerItem *horizontalSpacer_3;
    QSplitter *splitterVert;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QTableView *tableViewPart;
    QHBoxLayout *horizontalLayout_9;
    QCheckBox *checkBoxOnly;
    DbComboBox *comboBoxMar;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_8;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_8;
    QTableView *tableViewAdd;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    DbViewer *tableViewSertMechx;
    QHBoxLayout *horizontalLayout_2;
    QListView *listViewGost;
    QLabel *label_3;
    QLineEdit *lineEditSrcZnam;
    QToolButton *cmdCopyZnam;
    QLineEdit *lineEditZnam;
    QPushButton *cmdSaveZnam;
    QPushButton *pushButtonCopy;
    QHBoxLayout *horizontalLayout_10;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *checkBoxOk;
    QLabel *label_4;
    QPlainTextEdit *textEditPrim;
    QLabel *label_5;
    QPlainTextEdit *textEditPrimProd;
    QPushButton *cmdSavePrim;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_4;
    DbViewer *tableViewSrcChem;
    QToolButton *cmdCopyChem;
    DbViewer *tableViewSertChem;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_12;
    QHBoxLayout *horizontalLayout_5;
    DbViewer *tableViewSrcMech;
    QVBoxLayout *verticalLayout_7;
    QSpacerItem *verticalSpacer_2;
    QToolButton *cmdCopyMech;
    QToolButton *cmdCopyMechForward;
    QSpacerItem *verticalSpacer;
    DbViewer *tableViewSertMech;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *cmdCheck;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *cmdLbl;
    QPushButton *cmdLblSmall;
    QPushButton *cmdLblSmall2;
    QPushButton *cmdPartSert;

    void setupUi(QWidget *FormPart)
    {
        if (FormPart->objectName().isEmpty())
            FormPart->setObjectName(QString::fromUtf8("FormPart"));
        FormPart->resize(1143, 774);
        verticalLayout_2 = new QVBoxLayout(FormPart);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(FormPart);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        dateEditBeg = new DbDateEdit(FormPart);
        dateEditBeg->setObjectName(QString::fromUtf8("dateEditBeg"));
        dateEditBeg->setCalendarPopup(true);

        horizontalLayout->addWidget(dateEditBeg);

        label_2 = new QLabel(FormPart);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        dateEditEnd = new DbDateEdit(FormPart);
        dateEditEnd->setObjectName(QString::fromUtf8("dateEditEnd"));
        dateEditEnd->setCalendarPopup(true);

        horizontalLayout->addWidget(dateEditEnd);

        cmdUpd = new QPushButton(FormPart);
        cmdUpd->setObjectName(QString::fromUtf8("cmdUpd"));

        horizontalLayout->addWidget(cmdUpd);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout);

        splitterVert = new QSplitter(FormPart);
        splitterVert->setObjectName(QString::fromUtf8("splitterVert"));
        splitterVert->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitterVert);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tableViewPart = new QTableView(layoutWidget);
        tableViewPart->setObjectName(QString::fromUtf8("tableViewPart"));
        tableViewPart->setMinimumSize(QSize(400, 0));
        tableViewPart->setSelectionMode(QAbstractItemView::SingleSelection);
        tableViewPart->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(tableViewPart);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        checkBoxOnly = new QCheckBox(layoutWidget);
        checkBoxOnly->setObjectName(QString::fromUtf8("checkBoxOnly"));

        horizontalLayout_9->addWidget(checkBoxOnly);

        comboBoxMar = new DbComboBox(layoutWidget);
        comboBoxMar->setObjectName(QString::fromUtf8("comboBoxMar"));

        horizontalLayout_9->addWidget(comboBoxMar);


        verticalLayout->addLayout(horizontalLayout_9);

        splitterVert->addWidget(layoutWidget);
        layoutWidget_2 = new QWidget(splitterVert);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        verticalLayout_9 = new QVBoxLayout(layoutWidget_2);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        groupBox_4 = new QGroupBox(layoutWidget_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setMaximumSize(QSize(16777215, 170));
        verticalLayout_8 = new QVBoxLayout(groupBox_4);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        tableViewAdd = new QTableView(groupBox_4);
        tableViewAdd->setObjectName(QString::fromUtf8("tableViewAdd"));

        verticalLayout_8->addWidget(tableViewAdd);


        horizontalLayout_8->addWidget(groupBox_4);

        groupBox_3 = new QGroupBox(layoutWidget_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMaximumSize(QSize(330, 170));
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        tableViewSertMechx = new DbViewer(groupBox_3);
        tableViewSertMechx->setObjectName(QString::fromUtf8("tableViewSertMechx"));

        verticalLayout_5->addWidget(tableViewSertMechx);


        horizontalLayout_8->addWidget(groupBox_3);


        verticalLayout_9->addLayout(horizontalLayout_8);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        listViewGost = new QListView(layoutWidget_2);
        listViewGost->setObjectName(QString::fromUtf8("listViewGost"));
        listViewGost->setMaximumSize(QSize(220, 80));

        horizontalLayout_2->addWidget(listViewGost);

        label_3 = new QLabel(layoutWidget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        lineEditSrcZnam = new QLineEdit(layoutWidget_2);
        lineEditSrcZnam->setObjectName(QString::fromUtf8("lineEditSrcZnam"));
        lineEditSrcZnam->setReadOnly(true);

        horizontalLayout_2->addWidget(lineEditSrcZnam);

        cmdCopyZnam = new QToolButton(layoutWidget_2);
        cmdCopyZnam->setObjectName(QString::fromUtf8("cmdCopyZnam"));

        horizontalLayout_2->addWidget(cmdCopyZnam);

        lineEditZnam = new QLineEdit(layoutWidget_2);
        lineEditZnam->setObjectName(QString::fromUtf8("lineEditZnam"));

        horizontalLayout_2->addWidget(lineEditZnam);

        cmdSaveZnam = new QPushButton(layoutWidget_2);
        cmdSaveZnam->setObjectName(QString::fromUtf8("cmdSaveZnam"));

        horizontalLayout_2->addWidget(cmdSaveZnam);

        pushButtonCopy = new QPushButton(layoutWidget_2);
        pushButtonCopy->setObjectName(QString::fromUtf8("pushButtonCopy"));

        horizontalLayout_2->addWidget(pushButtonCopy);


        verticalLayout_9->addLayout(horizontalLayout_2);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));

        verticalLayout_9->addLayout(horizontalLayout_10);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        checkBoxOk = new QCheckBox(layoutWidget_2);
        checkBoxOk->setObjectName(QString::fromUtf8("checkBoxOk"));

        verticalLayout_4->addWidget(checkBoxOk);

        label_4 = new QLabel(layoutWidget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_4->addWidget(label_4);

        textEditPrim = new QPlainTextEdit(layoutWidget_2);
        textEditPrim->setObjectName(QString::fromUtf8("textEditPrim"));
        textEditPrim->setMaximumSize(QSize(170, 150));

        verticalLayout_4->addWidget(textEditPrim);

        label_5 = new QLabel(layoutWidget_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_4->addWidget(label_5);

        textEditPrimProd = new QPlainTextEdit(layoutWidget_2);
        textEditPrimProd->setObjectName(QString::fromUtf8("textEditPrimProd"));
        textEditPrimProd->setMaximumSize(QSize(170, 150));

        verticalLayout_4->addWidget(textEditPrimProd);

        cmdSavePrim = new QPushButton(layoutWidget_2);
        cmdSavePrim->setObjectName(QString::fromUtf8("cmdSavePrim"));

        verticalLayout_4->addWidget(cmdSavePrim);


        horizontalLayout_6->addLayout(verticalLayout_4);

        groupBox = new QGroupBox(layoutWidget_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 300));
        verticalLayout_6 = new QVBoxLayout(groupBox);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        tableViewSrcChem = new DbViewer(groupBox);
        tableViewSrcChem->setObjectName(QString::fromUtf8("tableViewSrcChem"));
        tableViewSrcChem->setMinimumSize(QSize(320, 0));
        tableViewSrcChem->setDragEnabled(false);
        tableViewSrcChem->setDragDropMode(QAbstractItemView::NoDragDrop);
        tableViewSrcChem->setDefaultDropAction(Qt::CopyAction);
        tableViewSrcChem->setSelectionMode(QAbstractItemView::SingleSelection);

        horizontalLayout_4->addWidget(tableViewSrcChem);

        cmdCopyChem = new QToolButton(groupBox);
        cmdCopyChem->setObjectName(QString::fromUtf8("cmdCopyChem"));

        horizontalLayout_4->addWidget(cmdCopyChem);

        tableViewSertChem = new DbViewer(groupBox);
        tableViewSertChem->setObjectName(QString::fromUtf8("tableViewSertChem"));
        tableViewSertChem->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_4->addWidget(tableViewSertChem);


        verticalLayout_6->addLayout(horizontalLayout_4);


        horizontalLayout_6->addWidget(groupBox);


        verticalLayout_9->addLayout(horizontalLayout_6);

        groupBox_2 = new QGroupBox(layoutWidget_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_12 = new QHBoxLayout(groupBox_2);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        tableViewSrcMech = new DbViewer(groupBox_2);
        tableViewSrcMech->setObjectName(QString::fromUtf8("tableViewSrcMech"));

        horizontalLayout_5->addWidget(tableViewSrcMech);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_2);

        cmdCopyMech = new QToolButton(groupBox_2);
        cmdCopyMech->setObjectName(QString::fromUtf8("cmdCopyMech"));

        verticalLayout_7->addWidget(cmdCopyMech);

        cmdCopyMechForward = new QToolButton(groupBox_2);
        cmdCopyMechForward->setObjectName(QString::fromUtf8("cmdCopyMechForward"));

        verticalLayout_7->addWidget(cmdCopyMechForward);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer);


        horizontalLayout_5->addLayout(verticalLayout_7);

        tableViewSertMech = new DbViewer(groupBox_2);
        tableViewSertMech->setObjectName(QString::fromUtf8("tableViewSertMech"));

        horizontalLayout_5->addWidget(tableViewSertMech);


        horizontalLayout_12->addLayout(horizontalLayout_5);


        verticalLayout_9->addWidget(groupBox_2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        cmdCheck = new QPushButton(layoutWidget_2);
        cmdCheck->setObjectName(QString::fromUtf8("cmdCheck"));

        horizontalLayout_7->addWidget(cmdCheck);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);

        cmdLbl = new QPushButton(layoutWidget_2);
        cmdLbl->setObjectName(QString::fromUtf8("cmdLbl"));

        horizontalLayout_7->addWidget(cmdLbl);

        cmdLblSmall = new QPushButton(layoutWidget_2);
        cmdLblSmall->setObjectName(QString::fromUtf8("cmdLblSmall"));

        horizontalLayout_7->addWidget(cmdLblSmall);

        cmdLblSmall2 = new QPushButton(layoutWidget_2);
        cmdLblSmall2->setObjectName(QString::fromUtf8("cmdLblSmall2"));

        horizontalLayout_7->addWidget(cmdLblSmall2);

        cmdPartSert = new QPushButton(layoutWidget_2);
        cmdPartSert->setObjectName(QString::fromUtf8("cmdPartSert"));

        horizontalLayout_7->addWidget(cmdPartSert);


        verticalLayout_9->addLayout(horizontalLayout_7);

        splitterVert->addWidget(layoutWidget_2);

        verticalLayout_2->addWidget(splitterVert);


        retranslateUi(FormPart);

        QMetaObject::connectSlotsByName(FormPart);
    } // setupUi

    void retranslateUi(QWidget *FormPart)
    {
        FormPart->setWindowTitle(QCoreApplication::translate("FormPart", "\320\244\320\276\321\200\320\274\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \320\270 \320\277\320\265\321\207\320\260\321\202\321\214 \321\201\320\265\321\200\321\202\320\270\321\204\320\270\320\272\320\260\321\202\320\276\320\262 \320\272\320\260\321\207\320\265\321\201\321\202\320\262\320\260", nullptr));
        label->setText(QCoreApplication::translate("FormPart", "C", nullptr));
        label_2->setText(QCoreApplication::translate("FormPart", "\320\237\320\276", nullptr));
        cmdUpd->setText(QCoreApplication::translate("FormPart", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        checkBoxOnly->setText(QCoreApplication::translate("FormPart", "\320\242\320\276\320\273\321\214\320\272\320\276 \320\274\320\260\321\200\320\272\320\260", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("FormPart", "\320\236\321\202\320\263\321\200\321\203\320\267\320\272\320\270", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("FormPart", "\320\241\320\277\320\265\321\206\320\270\320\260\320\273\321\214\320\275\321\213\320\265 \321\201\320\262\320\276\320\271\321\201\321\202\320\262\320\260 \320\274\320\265\321\202\320\260\320\273\320\273\320\260 \321\210\320\262\320\260", nullptr));
        label_3->setText(QCoreApplication::translate("FormPart", "\320\227\320\275\320\260\320\274.", nullptr));
        cmdCopyZnam->setText(QCoreApplication::translate("FormPart", "->", nullptr));
        cmdSaveZnam->setText(QCoreApplication::translate("FormPart", "\320\241\320\276\321\205\321\200. \320\267\320\275\320\260\320\274.", nullptr));
        pushButtonCopy->setText(QCoreApplication::translate("FormPart", "\320\230\320\274\320\277\320\276\321\200. \320\267\320\275\320\260\321\207.", nullptr));
        checkBoxOk->setText(QCoreApplication::translate("FormPart", "\320\230\321\201\320\277\321\213\321\202\320\260\320\275\320\270\321\217 \320\262 \320\275\320\276\321\200\320\274\320\265", nullptr));
        label_4->setText(QCoreApplication::translate("FormPart", "\320\237\321\200\320\270\320\274\320\265\321\207\320\260\320\275\320\270\320\265:", nullptr));
        label_5->setText(QCoreApplication::translate("FormPart", "\320\237\321\200\320\270\320\274\320\265\321\207\320\260\320\275\320\270\320\265 \321\201\320\261\321\213\321\202:", nullptr));
        cmdSavePrim->setText(QCoreApplication::translate("FormPart", "\320\241\320\276\321\205\321\200. \320\277\321\200\320\270\320\274\320\265\321\207\320\260\320\275\320\270\321\217", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FormPart", "\320\245\320\270\320\274\320\270\321\217", nullptr));
        cmdCopyChem->setText(QCoreApplication::translate("FormPart", "->", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("FormPart", "\320\234\320\265\321\205\320\260\320\275\320\270\320\272\320\260", nullptr));
        cmdCopyMech->setText(QCoreApplication::translate("FormPart", "->", nullptr));
        cmdCopyMechForward->setText(QCoreApplication::translate("FormPart", "<-", nullptr));
        cmdCheck->setText(QCoreApplication::translate("FormPart", "\320\237\321\200\320\276\320\262\320\265\321\200\320\270\321\202\321\214 \320\272\320\276\320\264", nullptr));
        cmdLbl->setText(QCoreApplication::translate("FormPart", "\320\255\321\202\320\270\320\272\320\265\321\202\320\272\320\260", nullptr));
        cmdLblSmall->setText(QCoreApplication::translate("FormPart", "\320\241\320\276\320\272\321\200. \321\215\321\202\320\270\320\272\320\265\321\202\320\272\320\260", nullptr));
        cmdLblSmall2->setText(QCoreApplication::translate("FormPart", "\320\241\320\276\320\272\321\200. \321\215\321\202\320\270\320\272\320\265\321\202\320\272\320\260 2", nullptr));
        cmdPartSert->setText(QCoreApplication::translate("FormPart", "\320\241\320\265\321\200\321\202\320\270\321\204\320\270\320\272\320\260\321\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormPart: public Ui_FormPart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMPART_H
