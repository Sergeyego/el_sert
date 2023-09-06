/********************************************************************************
** Form generated from reading UI file 'formdoc.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMDOC_H
#define UI_FORMDOC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "db/dbcombobox.h"
#include "db/dbdateedit.h"
#include "db/dbviewer.h"

QT_BEGIN_NAMESPACE

class Ui_FormDoc
{
public:
    QVBoxLayout *verticalLayout_7;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout;
    QPushButton *cmdUpd;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *checkBoxActive;
    DbViewer *tableViewDoc;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout;
    QPushButton *pushButtonView;
    DbComboBox *comboBoxVid;
    QLabel *labelSert;
    QPushButton *pushButtonDel;
    DbDateEdit *dateEditEnd;
    QLabel *label_3;
    QLineEdit *lineEditGtu;
    DbDateEdit *dateEditDoc;
    QLabel *label_4;
    QLineEdit *lineEditDoc;
    QLineEdit *lineEditNam;
    QLineEdit *lineEditNumNak;
    QLabel *label_2;
    QLabel *label_11;
    QLabel *label_10;
    QLabel *label_5;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *lineEditNumBl;
    QLabel *label;
    QLabel *label_7;
    DbComboBox *comboBoxVed;
    QLabel *label_6;
    QPushButton *pushButtonUpload;
    DbDateEdit *dateEditBeg;
    QPushButton *pushButtonSaveAs;
    DbViewer *tableViewTu;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *toolButtonCheckAll;
    QToolButton *toolButtonUnCheckAll;
    QSpacerItem *horizontalSpacer_3;
    QListView *listViewEn;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_2;
    DbViewer *tableViewElDim;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    DbViewer *tableViewEl;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBoxWire;
    QVBoxLayout *verticalLayout_8;
    DbViewer *tableViewWireDiam;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_9;
    DbViewer *tableViewWire;
    QHBoxLayout *horizontalLayoutMap;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *FormDoc)
    {
        if (FormDoc->objectName().isEmpty())
            FormDoc->setObjectName(QString::fromUtf8("FormDoc"));
        FormDoc->resize(1023, 736);
        verticalLayout_7 = new QVBoxLayout(FormDoc);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        splitter = new QSplitter(FormDoc);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout_6 = new QVBoxLayout(layoutWidget);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cmdUpd = new QPushButton(layoutWidget);
        cmdUpd->setObjectName(QString::fromUtf8("cmdUpd"));

        horizontalLayout->addWidget(cmdUpd);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_6->addLayout(horizontalLayout);

        checkBoxActive = new QCheckBox(layoutWidget);
        checkBoxActive->setObjectName(QString::fromUtf8("checkBoxActive"));
        checkBoxActive->setChecked(true);

        verticalLayout_6->addWidget(checkBoxActive);

        tableViewDoc = new DbViewer(layoutWidget);
        tableViewDoc->setObjectName(QString::fromUtf8("tableViewDoc"));
        tableViewDoc->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout_6->addWidget(tableViewDoc);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        verticalLayout_5 = new QVBoxLayout(layoutWidget1);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButtonView = new QPushButton(layoutWidget1);
        pushButtonView->setObjectName(QString::fromUtf8("pushButtonView"));
        pushButtonView->setEnabled(false);

        gridLayout->addWidget(pushButtonView, 6, 2, 1, 1);

        comboBoxVid = new DbComboBox(layoutWidget1);
        comboBoxVid->setObjectName(QString::fromUtf8("comboBoxVid"));
        comboBoxVid->setEditable(true);

        gridLayout->addWidget(comboBoxVid, 4, 1, 1, 1);

        labelSert = new QLabel(layoutWidget1);
        labelSert->setObjectName(QString::fromUtf8("labelSert"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelSert->sizePolicy().hasHeightForWidth());
        labelSert->setSizePolicy(sizePolicy);
        labelSert->setMinimumSize(QSize(100, 100));
        labelSert->setMaximumSize(QSize(100, 100));
        labelSert->setPixmap(QPixmap(QString::fromUtf8(":/images/notok.png")));
        labelSert->setScaledContents(true);

        gridLayout->addWidget(labelSert, 3, 2, 1, 1);

        pushButtonDel = new QPushButton(layoutWidget1);
        pushButtonDel->setObjectName(QString::fromUtf8("pushButtonDel"));

        gridLayout->addWidget(pushButtonDel, 7, 2, 1, 1);

        dateEditEnd = new DbDateEdit(layoutWidget1);
        dateEditEnd->setObjectName(QString::fromUtf8("dateEditEnd"));

        gridLayout->addWidget(dateEditEnd, 1, 3, 1, 1);

        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        lineEditGtu = new QLineEdit(layoutWidget1);
        lineEditGtu->setObjectName(QString::fromUtf8("lineEditGtu"));

        gridLayout->addWidget(lineEditGtu, 7, 1, 1, 1);

        dateEditDoc = new DbDateEdit(layoutWidget1);
        dateEditDoc->setObjectName(QString::fromUtf8("dateEditDoc"));

        gridLayout->addWidget(dateEditDoc, 3, 1, 1, 1);

        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        lineEditDoc = new QLineEdit(layoutWidget1);
        lineEditDoc->setObjectName(QString::fromUtf8("lineEditDoc"));

        gridLayout->addWidget(lineEditDoc, 0, 1, 1, 1);

        lineEditNam = new QLineEdit(layoutWidget1);
        lineEditNam->setObjectName(QString::fromUtf8("lineEditNam"));

        gridLayout->addWidget(lineEditNam, 1, 1, 1, 1);

        lineEditNumNak = new QLineEdit(layoutWidget1);
        lineEditNumNak->setObjectName(QString::fromUtf8("lineEditNumNak"));

        gridLayout->addWidget(lineEditNumNak, 6, 1, 1, 1);

        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_11 = new QLabel(layoutWidget1);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 7, 0, 1, 1);

        label_10 = new QLabel(layoutWidget1);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 2, 2, 1, 1);

        label_5 = new QLabel(layoutWidget1);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 1, 2, 1, 1);

        label_8 = new QLabel(layoutWidget1);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 5, 0, 1, 1);

        label_9 = new QLabel(layoutWidget1);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 6, 0, 1, 1);

        lineEditNumBl = new QLineEdit(layoutWidget1);
        lineEditNumBl->setObjectName(QString::fromUtf8("lineEditNumBl"));

        gridLayout->addWidget(lineEditNumBl, 5, 1, 1, 1);

        label = new QLabel(layoutWidget1);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_7 = new QLabel(layoutWidget1);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 4, 0, 1, 1);

        comboBoxVed = new DbComboBox(layoutWidget1);
        comboBoxVed->setObjectName(QString::fromUtf8("comboBoxVed"));
        comboBoxVed->setEditable(true);

        gridLayout->addWidget(comboBoxVed, 2, 1, 1, 1);

        label_6 = new QLabel(layoutWidget1);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 2, 0, 1, 1);

        pushButtonUpload = new QPushButton(layoutWidget1);
        pushButtonUpload->setObjectName(QString::fromUtf8("pushButtonUpload"));

        gridLayout->addWidget(pushButtonUpload, 4, 2, 1, 1);

        dateEditBeg = new DbDateEdit(layoutWidget1);
        dateEditBeg->setObjectName(QString::fromUtf8("dateEditBeg"));

        gridLayout->addWidget(dateEditBeg, 0, 3, 1, 1);

        pushButtonSaveAs = new QPushButton(layoutWidget1);
        pushButtonSaveAs->setObjectName(QString::fromUtf8("pushButtonSaveAs"));
        pushButtonSaveAs->setEnabled(false);

        gridLayout->addWidget(pushButtonSaveAs, 5, 2, 1, 1);

        tableViewTu = new DbViewer(layoutWidget1);
        tableViewTu->setObjectName(QString::fromUtf8("tableViewTu"));

        gridLayout->addWidget(tableViewTu, 2, 3, 6, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        toolButtonCheckAll = new QToolButton(layoutWidget1);
        toolButtonCheckAll->setObjectName(QString::fromUtf8("toolButtonCheckAll"));
        toolButtonCheckAll->setMinimumSize(QSize(25, 0));
        toolButtonCheckAll->setMaximumSize(QSize(25, 16777215));

        horizontalLayout_4->addWidget(toolButtonCheckAll);

        toolButtonUnCheckAll = new QToolButton(layoutWidget1);
        toolButtonUnCheckAll->setObjectName(QString::fromUtf8("toolButtonUnCheckAll"));
        toolButtonUnCheckAll->setMinimumSize(QSize(25, 0));
        toolButtonUnCheckAll->setMaximumSize(QSize(25, 16777215));

        horizontalLayout_4->addWidget(toolButtonUnCheckAll);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        gridLayout->addLayout(horizontalLayout_4, 7, 4, 1, 1);

        listViewEn = new QListView(layoutWidget1);
        listViewEn->setObjectName(QString::fromUtf8("listViewEn"));

        gridLayout->addWidget(listViewEn, 0, 4, 7, 1);


        verticalLayout_5->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox_3 = new QGroupBox(layoutWidget1);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(400, 0));
        verticalLayout_2 = new QVBoxLayout(groupBox_3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tableViewElDim = new DbViewer(groupBox_3);
        tableViewElDim->setObjectName(QString::fromUtf8("tableViewElDim"));

        verticalLayout_2->addWidget(tableViewElDim);


        horizontalLayout_2->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(layoutWidget1);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tableViewEl = new DbViewer(groupBox_2);
        tableViewEl->setObjectName(QString::fromUtf8("tableViewEl"));

        verticalLayout_3->addWidget(tableViewEl);


        horizontalLayout_2->addWidget(groupBox_2);


        verticalLayout_5->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        groupBoxWire = new QGroupBox(layoutWidget1);
        groupBoxWire->setObjectName(QString::fromUtf8("groupBoxWire"));
        verticalLayout_8 = new QVBoxLayout(groupBoxWire);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        tableViewWireDiam = new DbViewer(groupBoxWire);
        tableViewWireDiam->setObjectName(QString::fromUtf8("tableViewWireDiam"));

        verticalLayout_8->addWidget(tableViewWireDiam);


        horizontalLayout_3->addWidget(groupBoxWire);

        groupBox_5 = new QGroupBox(layoutWidget1);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        verticalLayout_9 = new QVBoxLayout(groupBox_5);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        tableViewWire = new DbViewer(groupBox_5);
        tableViewWire->setObjectName(QString::fromUtf8("tableViewWire"));

        verticalLayout_9->addWidget(tableViewWire);


        horizontalLayout_3->addWidget(groupBox_5);


        verticalLayout_5->addLayout(horizontalLayout_3);

        horizontalLayoutMap = new QHBoxLayout();
        horizontalLayoutMap->setObjectName(QString::fromUtf8("horizontalLayoutMap"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutMap->addItem(horizontalSpacer);


        verticalLayout_5->addLayout(horizontalLayoutMap);

        splitter->addWidget(layoutWidget1);

        verticalLayout_7->addWidget(splitter);

        QWidget::setTabOrder(checkBoxActive, tableViewDoc);
        QWidget::setTabOrder(tableViewDoc, lineEditDoc);
        QWidget::setTabOrder(lineEditDoc, lineEditNam);
        QWidget::setTabOrder(lineEditNam, dateEditBeg);
        QWidget::setTabOrder(dateEditBeg, dateEditEnd);
        QWidget::setTabOrder(dateEditEnd, tableViewElDim);
        QWidget::setTabOrder(tableViewElDim, tableViewEl);

        retranslateUi(FormDoc);

        QMetaObject::connectSlotsByName(FormDoc);
    } // setupUi

    void retranslateUi(QWidget *FormDoc)
    {
        FormDoc->setWindowTitle(QCoreApplication::translate("FormDoc", "Form", nullptr));
        cmdUpd->setText(QCoreApplication::translate("FormDoc", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        checkBoxActive->setText(QCoreApplication::translate("FormDoc", "\320\242\320\276\320\273\321\214\320\272\320\276 \320\264\320\265\320\271\321\201\321\202\320\262\321\203\321\216\321\211\320\270\320\265", nullptr));
        pushButtonView->setText(QCoreApplication::translate("FormDoc", "\320\237\321\200\320\276\321\201\320\274\320\276\321\202\321\200", nullptr));
        labelSert->setText(QString());
        pushButtonDel->setText(QCoreApplication::translate("FormDoc", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        label_3->setText(QCoreApplication::translate("FormDoc", "\320\224\320\260\321\202\320\260 \320\275\320\260\321\207\320\260\320\273\320\260 \320\264\320\265\320\271\321\201\321\202.", nullptr));
        label_4->setText(QCoreApplication::translate("FormDoc", "\320\224\320\260\321\202\320\260 \320\277\321\200\320\270\320\275\321\217\321\202\320\270\321\217", nullptr));
        label_2->setText(QCoreApplication::translate("FormDoc", "\320\235\320\260\320\267\320\262. \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202\320\260", nullptr));
        label_11->setText(QCoreApplication::translate("FormDoc", "\320\223\321\200\321\203\320\277\320\277\321\213 \321\202\320\265\321\205. \321\203\321\201\321\202\321\200.", nullptr));
        label_10->setText(QCoreApplication::translate("FormDoc", "\320\235\320\276\321\200\320\274\320\260\321\202\320\270\320\262. \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202.", nullptr));
        label_5->setText(QCoreApplication::translate("FormDoc", "\320\224\320\260\321\202\320\260 \320\276\320\272\320\276\320\275\321\207. \320\264\320\265\320\271\321\201\321\202.", nullptr));
        label_8->setText(QCoreApplication::translate("FormDoc", "\320\235\320\276\320\274\320\265\321\200 \320\261\320\273\320\260\320\275\320\272\320\260", nullptr));
        label_9->setText(QCoreApplication::translate("FormDoc", "\320\235\320\276\320\274\320\265\321\200 \320\275\320\260\320\272\320\273\320\265\320\271\320\272\320\270", nullptr));
        label->setText(QCoreApplication::translate("FormDoc", "\320\235\320\276\320\274\320\265\321\200 \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202\320\260", nullptr));
        label_7->setText(QCoreApplication::translate("FormDoc", "\320\222\320\270\320\264 \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202\320\260", nullptr));
        label_6->setText(QCoreApplication::translate("FormDoc", "\320\222\320\265\320\264\320\276\320\274\321\201\321\202\320\262\320\276", nullptr));
        pushButtonUpload->setText(QCoreApplication::translate("FormDoc", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214", nullptr));
        pushButtonSaveAs->setText(QCoreApplication::translate("FormDoc", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\272\320\260\320\272", nullptr));
        toolButtonCheckAll->setText(QCoreApplication::translate("FormDoc", "+", nullptr));
        toolButtonUnCheckAll->setText(QCoreApplication::translate("FormDoc", "-", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("FormDoc", "\320\255\320\273\320\265\320\272\321\202\321\200\320\276\320\264\321\213: \320\274\320\260\321\200\320\272\320\260 \320\270 \320\264\320\270\320\260\320\274\320\265\321\202\321\200", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("FormDoc", "\320\234\320\260\321\200\320\272\320\260", nullptr));
        groupBoxWire->setTitle(QCoreApplication::translate("FormDoc", "\320\237\321\200\320\276\320\262\320\276\320\273\320\276\320\272\320\260: \320\274\320\260\321\200\320\272\320\260 \320\270 \320\264\320\270\320\260\320\274\320\265\321\202\321\200", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("FormDoc", "\320\234\320\260\321\200\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormDoc: public Ui_FormDoc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMDOC_H
