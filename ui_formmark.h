/********************************************************************************
** Form generated from reading UI file 'formmark.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMMARK_H
#define UI_FORMMARK_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
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

class Ui_FormMark
{
public:
    QVBoxLayout *verticalLayout_10;
    QSplitter *splitter;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_6;
    QPushButton *pushButtonUpd;
    DbViewer *tableViewMark;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLabel *label_9;
    QCheckBox *checkBoxKat;
    QLabel *label_10;
    QLabel *label_8;
    DbComboBox *comboBoxISO;
    DbComboBox *comboBoxVid;
    DbComboBox *comboBoxAWS;
    QLabel *label_14;
    QLineEdit *lineEditMark;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEditPr;
    QLineEdit *lineEditMarkSert;
    QLineEdit *lineEditInd;
    QLabel *label_12;
    QLabel *label_4;
    QLineEdit *lineEditVl;
    DbComboBox *comboBoxGroup;
    QLabel *label_11;
    DbComboBox *comboBoxGost;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_3;
    DbComboBox *comboBoxProv;
    DbComboBox *comboBoxPol;
    DbComboBox *comboBoxBukv;
    QLabel *label_15;
    DbComboBox *comboBoxZnam;
    QLabel *label_13;
    QLabel *label_21;
    QPlainTextEdit *plainTextEditDescrCat;
    QLabel *labelPix;
    QPlainTextEdit *plainTextEditDescrSpec;
    QLabel *label_22;
    QPlainTextEdit *plainTextEditDescrFeature;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_24;
    DbViewer *tableViewPack;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_23;
    DbViewer *tableViewPlav;
    QVBoxLayout *verticalLayout_11;
    QLabel *label_30;
    DbViewer *tableViewProvol;
    QGroupBox *groupBoxVar;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_25;
    DbComboBox *comboBoxVar;
    QToolButton *toolButtonEdtVar;
    QPushButton *pushButtonCreVar;
    QPushButton *pushButtonSaveVar;
    QPushButton *pushButtonDelVar;
    QPushButton *pushButtonCopy;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout_2;
    QLineEdit *lineEditVarZnam;
    QLineEdit *lineEditProcVar;
    QLabel *label_26;
    QLabel *label_16;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_27;
    QPlainTextEdit *plainTextEditDescr;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_19;
    DbViewer *tableViewAmp;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_12;
    QLabel *label_20;
    DbViewer *tableViewGost;
    QVBoxLayout *verticalLayout;
    QLabel *label_17;
    DbViewer *tableViewChem;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_18;
    DbViewer *tableViewMech;
    QHBoxLayout *horizontalLayoutLabel;
    QLabel *label_28;
    DbDateEdit *dateEdit;
    QLabel *label_29;
    DbComboBox *comboBoxDiam;
    QCheckBox *checkBoxOrder;
    QPushButton *cmdLbl;
    QPushButton *cmdLblSmall;
    QPushButton *cmdLblSmall2;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayoutMap;
    QSpacerItem *horizontalSpacer;
    QPushButton *cmdExt;

    void setupUi(QWidget *FormMark)
    {
        if (FormMark->objectName().isEmpty())
            FormMark->setObjectName(QString::fromUtf8("FormMark"));
        FormMark->resize(1114, 845);
        verticalLayout_10 = new QVBoxLayout(FormMark);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        splitter = new QSplitter(FormMark);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        verticalLayoutWidget = new QWidget(splitter);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        pushButtonUpd = new QPushButton(verticalLayoutWidget);
        pushButtonUpd->setObjectName(QString::fromUtf8("pushButtonUpd"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButtonUpd->sizePolicy().hasHeightForWidth());
        pushButtonUpd->setSizePolicy(sizePolicy);

        verticalLayout_6->addWidget(pushButtonUpd);

        tableViewMark = new DbViewer(verticalLayoutWidget);
        tableViewMark->setObjectName(QString::fromUtf8("tableViewMark"));
        tableViewMark->setSelectionMode(QAbstractItemView::SingleSelection);
        tableViewMark->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_6->addWidget(tableViewMark);

        splitter->addWidget(verticalLayoutWidget);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout_5 = new QVBoxLayout(layoutWidget);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 1, 2, 1, 1);

        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 2, 2, 1, 1);

        checkBoxKat = new QCheckBox(layoutWidget);
        checkBoxKat->setObjectName(QString::fromUtf8("checkBoxKat"));

        gridLayout->addWidget(checkBoxKat, 3, 5, 1, 1);

        label_10 = new QLabel(layoutWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 2, 4, 1, 1);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 2, 0, 1, 1);

        comboBoxISO = new DbComboBox(layoutWidget);
        comboBoxISO->setObjectName(QString::fromUtf8("comboBoxISO"));
        comboBoxISO->setEditable(true);

        gridLayout->addWidget(comboBoxISO, 2, 1, 1, 1);

        comboBoxVid = new DbComboBox(layoutWidget);
        comboBoxVid->setObjectName(QString::fromUtf8("comboBoxVid"));
        comboBoxVid->setEditable(true);

        gridLayout->addWidget(comboBoxVid, 1, 1, 1, 1);

        comboBoxAWS = new DbComboBox(layoutWidget);
        comboBoxAWS->setObjectName(QString::fromUtf8("comboBoxAWS"));
        comboBoxAWS->setEditable(true);

        gridLayout->addWidget(comboBoxAWS, 2, 3, 1, 1);

        label_14 = new QLabel(layoutWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout->addWidget(label_14, 4, 2, 1, 1);

        lineEditMark = new QLineEdit(layoutWidget);
        lineEditMark->setObjectName(QString::fromUtf8("lineEditMark"));

        gridLayout->addWidget(lineEditMark, 0, 1, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        lineEditPr = new QLineEdit(layoutWidget);
        lineEditPr->setObjectName(QString::fromUtf8("lineEditPr"));

        gridLayout->addWidget(lineEditPr, 4, 3, 1, 1);

        lineEditMarkSert = new QLineEdit(layoutWidget);
        lineEditMarkSert->setObjectName(QString::fromUtf8("lineEditMarkSert"));

        gridLayout->addWidget(lineEditMarkSert, 0, 3, 1, 1);

        lineEditInd = new QLineEdit(layoutWidget);
        lineEditInd->setObjectName(QString::fromUtf8("lineEditInd"));

        gridLayout->addWidget(lineEditInd, 4, 5, 1, 1);

        label_12 = new QLabel(layoutWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 4, 4, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        lineEditVl = new QLineEdit(layoutWidget);
        lineEditVl->setObjectName(QString::fromUtf8("lineEditVl"));

        gridLayout->addWidget(lineEditVl, 4, 1, 1, 1);

        comboBoxGroup = new DbComboBox(layoutWidget);
        comboBoxGroup->setObjectName(QString::fromUtf8("comboBoxGroup"));
        comboBoxGroup->setEditable(true);

        gridLayout->addWidget(comboBoxGroup, 0, 5, 1, 1);

        label_11 = new QLabel(layoutWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 3, 0, 1, 1);

        comboBoxGost = new DbComboBox(layoutWidget);
        comboBoxGost->setObjectName(QString::fromUtf8("comboBoxGost"));
        comboBoxGost->setEditable(true);

        gridLayout->addWidget(comboBoxGost, 1, 5, 1, 1);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 3, 2, 1, 1);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 1, 4, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 4, 1, 1);

        comboBoxProv = new DbComboBox(layoutWidget);
        comboBoxProv->setObjectName(QString::fromUtf8("comboBoxProv"));
        comboBoxProv->setEditable(true);

        gridLayout->addWidget(comboBoxProv, 1, 3, 1, 1);

        comboBoxPol = new DbComboBox(layoutWidget);
        comboBoxPol->setObjectName(QString::fromUtf8("comboBoxPol"));
        comboBoxPol->setEditable(true);

        gridLayout->addWidget(comboBoxPol, 3, 3, 1, 2);

        comboBoxBukv = new DbComboBox(layoutWidget);
        comboBoxBukv->setObjectName(QString::fromUtf8("comboBoxBukv"));
        comboBoxBukv->setEditable(true);

        gridLayout->addWidget(comboBoxBukv, 3, 1, 1, 1);

        label_15 = new QLabel(layoutWidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout->addWidget(label_15, 5, 0, 1, 1);

        comboBoxZnam = new DbComboBox(layoutWidget);
        comboBoxZnam->setObjectName(QString::fromUtf8("comboBoxZnam"));
        comboBoxZnam->setEditable(true);

        gridLayout->addWidget(comboBoxZnam, 2, 5, 1, 1);

        label_13 = new QLabel(layoutWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout->addWidget(label_13, 4, 0, 1, 1);

        label_21 = new QLabel(layoutWidget);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_21->sizePolicy().hasHeightForWidth());
        label_21->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_21, 6, 0, 1, 1);

        plainTextEditDescrCat = new QPlainTextEdit(layoutWidget);
        plainTextEditDescrCat->setObjectName(QString::fromUtf8("plainTextEditDescrCat"));
        plainTextEditDescrCat->setMaximumSize(QSize(16777215, 70));

        gridLayout->addWidget(plainTextEditDescrCat, 5, 1, 1, 4);

        labelPix = new QLabel(layoutWidget);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));
        labelPix->setMinimumSize(QSize(70, 70));
        labelPix->setMaximumSize(QSize(70, 70));
        labelPix->setContextMenuPolicy(Qt::NoContextMenu);

        gridLayout->addWidget(labelPix, 5, 5, 1, 1);

        plainTextEditDescrSpec = new QPlainTextEdit(layoutWidget);
        plainTextEditDescrSpec->setObjectName(QString::fromUtf8("plainTextEditDescrSpec"));
        plainTextEditDescrSpec->setMaximumSize(QSize(16777215, 50));

        gridLayout->addWidget(plainTextEditDescrSpec, 6, 1, 1, 1);

        label_22 = new QLabel(layoutWidget);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        sizePolicy1.setHeightForWidth(label_22->sizePolicy().hasHeightForWidth());
        label_22->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_22, 6, 2, 1, 1);

        plainTextEditDescrFeature = new QPlainTextEdit(layoutWidget);
        plainTextEditDescrFeature->setObjectName(QString::fromUtf8("plainTextEditDescrFeature"));
        plainTextEditDescrFeature->setMaximumSize(QSize(16777215, 50));

        gridLayout->addWidget(plainTextEditDescrFeature, 6, 3, 1, 3);


        verticalLayout_5->addLayout(gridLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_24 = new QLabel(layoutWidget);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        verticalLayout_3->addWidget(label_24);

        tableViewPack = new DbViewer(layoutWidget);
        tableViewPack->setObjectName(QString::fromUtf8("tableViewPack"));
        tableViewPack->setMinimumSize(QSize(550, 0));

        verticalLayout_3->addWidget(tableViewPack);


        horizontalLayout_3->addLayout(verticalLayout_3);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        label_23 = new QLabel(layoutWidget);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        verticalLayout_7->addWidget(label_23);

        tableViewPlav = new DbViewer(layoutWidget);
        tableViewPlav->setObjectName(QString::fromUtf8("tableViewPlav"));

        verticalLayout_7->addWidget(tableViewPlav);


        horizontalLayout_3->addLayout(verticalLayout_7);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        label_30 = new QLabel(layoutWidget);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        verticalLayout_11->addWidget(label_30);

        tableViewProvol = new DbViewer(layoutWidget);
        tableViewProvol->setObjectName(QString::fromUtf8("tableViewProvol"));

        verticalLayout_11->addWidget(tableViewProvol);


        horizontalLayout_3->addLayout(verticalLayout_11);


        verticalLayout_5->addLayout(horizontalLayout_3);

        groupBoxVar = new QGroupBox(layoutWidget);
        groupBoxVar->setObjectName(QString::fromUtf8("groupBoxVar"));
        verticalLayout_9 = new QVBoxLayout(groupBoxVar);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_25 = new QLabel(groupBoxVar);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        horizontalLayout_6->addWidget(label_25);

        comboBoxVar = new DbComboBox(groupBoxVar);
        comboBoxVar->setObjectName(QString::fromUtf8("comboBoxVar"));
        comboBoxVar->setMinimumSize(QSize(200, 0));

        horizontalLayout_6->addWidget(comboBoxVar);

        toolButtonEdtVar = new QToolButton(groupBoxVar);
        toolButtonEdtVar->setObjectName(QString::fromUtf8("toolButtonEdtVar"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(toolButtonEdtVar->sizePolicy().hasHeightForWidth());
        toolButtonEdtVar->setSizePolicy(sizePolicy2);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/key.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButtonEdtVar->setIcon(icon);

        horizontalLayout_6->addWidget(toolButtonEdtVar);

        pushButtonCreVar = new QPushButton(groupBoxVar);
        pushButtonCreVar->setObjectName(QString::fromUtf8("pushButtonCreVar"));

        horizontalLayout_6->addWidget(pushButtonCreVar);

        pushButtonSaveVar = new QPushButton(groupBoxVar);
        pushButtonSaveVar->setObjectName(QString::fromUtf8("pushButtonSaveVar"));

        horizontalLayout_6->addWidget(pushButtonSaveVar);

        pushButtonDelVar = new QPushButton(groupBoxVar);
        pushButtonDelVar->setObjectName(QString::fromUtf8("pushButtonDelVar"));

        horizontalLayout_6->addWidget(pushButtonDelVar);

        pushButtonCopy = new QPushButton(groupBoxVar);
        pushButtonCopy->setObjectName(QString::fromUtf8("pushButtonCopy"));

        horizontalLayout_6->addWidget(pushButtonCopy);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);


        verticalLayout_9->addLayout(horizontalLayout_6);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        lineEditVarZnam = new QLineEdit(groupBoxVar);
        lineEditVarZnam->setObjectName(QString::fromUtf8("lineEditVarZnam"));
        lineEditVarZnam->setMaximumSize(QSize(150, 16777215));

        gridLayout_2->addWidget(lineEditVarZnam, 0, 1, 1, 1);

        lineEditProcVar = new QLineEdit(groupBoxVar);
        lineEditProcVar->setObjectName(QString::fromUtf8("lineEditProcVar"));
        lineEditProcVar->setMaximumSize(QSize(150, 16777215));

        gridLayout_2->addWidget(lineEditProcVar, 1, 1, 1, 1);

        label_26 = new QLabel(groupBoxVar);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setMaximumSize(QSize(130, 16777215));

        gridLayout_2->addWidget(label_26, 0, 0, 1, 1);

        label_16 = new QLabel(groupBoxVar);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMaximumSize(QSize(130, 16777215));

        gridLayout_2->addWidget(label_16, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 2, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_27 = new QLabel(groupBoxVar);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        verticalLayout_4->addWidget(label_27);

        plainTextEditDescr = new QPlainTextEdit(groupBoxVar);
        plainTextEditDescr->setObjectName(QString::fromUtf8("plainTextEditDescr"));
        plainTextEditDescr->setMinimumSize(QSize(300, 0));

        verticalLayout_4->addWidget(plainTextEditDescr);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        label_19 = new QLabel(groupBoxVar);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        verticalLayout_8->addWidget(label_19);

        tableViewAmp = new DbViewer(groupBoxVar);
        tableViewAmp->setObjectName(QString::fromUtf8("tableViewAmp"));
        tableViewAmp->setMinimumSize(QSize(350, 0));

        verticalLayout_8->addWidget(tableViewAmp);


        horizontalLayout->addLayout(verticalLayout_8);


        verticalLayout_9->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        label_20 = new QLabel(groupBoxVar);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        verticalLayout_12->addWidget(label_20);

        tableViewGost = new DbViewer(groupBoxVar);
        tableViewGost->setObjectName(QString::fromUtf8("tableViewGost"));

        verticalLayout_12->addWidget(tableViewGost);


        horizontalLayout_2->addLayout(verticalLayout_12);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_17 = new QLabel(groupBoxVar);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        verticalLayout->addWidget(label_17);

        tableViewChem = new DbViewer(groupBoxVar);
        tableViewChem->setObjectName(QString::fromUtf8("tableViewChem"));

        verticalLayout->addWidget(tableViewChem);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_18 = new QLabel(groupBoxVar);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        verticalLayout_2->addWidget(label_18);

        tableViewMech = new DbViewer(groupBoxVar);
        tableViewMech->setObjectName(QString::fromUtf8("tableViewMech"));

        verticalLayout_2->addWidget(tableViewMech);


        horizontalLayout_2->addLayout(verticalLayout_2);


        verticalLayout_9->addLayout(horizontalLayout_2);


        verticalLayout_5->addWidget(groupBoxVar);

        horizontalLayoutLabel = new QHBoxLayout();
        horizontalLayoutLabel->setObjectName(QString::fromUtf8("horizontalLayoutLabel"));
        label_28 = new QLabel(layoutWidget);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        horizontalLayoutLabel->addWidget(label_28);

        dateEdit = new DbDateEdit(layoutWidget);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));

        horizontalLayoutLabel->addWidget(dateEdit);

        label_29 = new QLabel(layoutWidget);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        horizontalLayoutLabel->addWidget(label_29);

        comboBoxDiam = new DbComboBox(layoutWidget);
        comboBoxDiam->setObjectName(QString::fromUtf8("comboBoxDiam"));

        horizontalLayoutLabel->addWidget(comboBoxDiam);

        checkBoxOrder = new QCheckBox(layoutWidget);
        checkBoxOrder->setObjectName(QString::fromUtf8("checkBoxOrder"));

        horizontalLayoutLabel->addWidget(checkBoxOrder);

        cmdLbl = new QPushButton(layoutWidget);
        cmdLbl->setObjectName(QString::fromUtf8("cmdLbl"));

        horizontalLayoutLabel->addWidget(cmdLbl);

        cmdLblSmall = new QPushButton(layoutWidget);
        cmdLblSmall->setObjectName(QString::fromUtf8("cmdLblSmall"));

        horizontalLayoutLabel->addWidget(cmdLblSmall);

        cmdLblSmall2 = new QPushButton(layoutWidget);
        cmdLblSmall2->setObjectName(QString::fromUtf8("cmdLblSmall2"));

        horizontalLayoutLabel->addWidget(cmdLblSmall2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutLabel->addItem(horizontalSpacer_3);


        verticalLayout_5->addLayout(horizontalLayoutLabel);

        horizontalLayoutMap = new QHBoxLayout();
        horizontalLayoutMap->setObjectName(QString::fromUtf8("horizontalLayoutMap"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutMap->addItem(horizontalSpacer);

        cmdExt = new QPushButton(layoutWidget);
        cmdExt->setObjectName(QString::fromUtf8("cmdExt"));

        horizontalLayoutMap->addWidget(cmdExt);


        verticalLayout_5->addLayout(horizontalLayoutMap);

        splitter->addWidget(layoutWidget);

        verticalLayout_10->addWidget(splitter);

        QWidget::setTabOrder(tableViewMark, lineEditMark);
        QWidget::setTabOrder(lineEditMark, lineEditMarkSert);
        QWidget::setTabOrder(lineEditMarkSert, comboBoxGroup);
        QWidget::setTabOrder(comboBoxGroup, comboBoxVid);
        QWidget::setTabOrder(comboBoxVid, comboBoxProv);
        QWidget::setTabOrder(comboBoxProv, comboBoxGost);
        QWidget::setTabOrder(comboBoxGost, comboBoxISO);
        QWidget::setTabOrder(comboBoxISO, comboBoxAWS);
        QWidget::setTabOrder(comboBoxAWS, comboBoxBukv);
        QWidget::setTabOrder(comboBoxBukv, comboBoxPol);
        QWidget::setTabOrder(comboBoxPol, lineEditVl);
        QWidget::setTabOrder(lineEditVl, lineEditPr);
        QWidget::setTabOrder(lineEditPr, lineEditInd);
        QWidget::setTabOrder(lineEditInd, tableViewChem);
        QWidget::setTabOrder(tableViewChem, tableViewMech);

        retranslateUi(FormMark);

        QMetaObject::connectSlotsByName(FormMark);
    } // setupUi

    void retranslateUi(QWidget *FormMark)
    {
        FormMark->setWindowTitle(QCoreApplication::translate("FormMark", "Form", nullptr));
        pushButtonUpd->setText(QCoreApplication::translate("FormMark", "\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        label_5->setText(QCoreApplication::translate("FormMark", "\320\237\321\200\320\276\320\262\320\276\320\273\320\276\320\272\320\260", nullptr));
        label_9->setText(QCoreApplication::translate("FormMark", "\320\242\320\270\320\277 \320\277\320\276 AWS", nullptr));
        checkBoxKat->setText(QCoreApplication::translate("FormMark", "\320\232\320\260\321\202\320\260\320\273\320\276\320\263", nullptr));
        label_10->setText(QCoreApplication::translate("FormMark", "\320\227\320\275\320\260\320\274\320\265\320\275\320\260\321\202\320\265\320\273\321\214", nullptr));
        label_8->setText(QCoreApplication::translate("FormMark", "\320\242\320\270\320\277 \320\277\320\276 ISO", nullptr));
        label_14->setText(QCoreApplication::translate("FormMark", "\320\240\320\265\320\266\320\270\320\274 \320\277\320\276\320\262\321\202. \320\277\321\200\320\276\320\272.", nullptr));
        label->setText(QCoreApplication::translate("FormMark", "\320\234\320\260\321\200\320\272\320\260", nullptr));
        label_2->setText(QCoreApplication::translate("FormMark", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \320\264\320\273\321\217 \321\201\320\265\321\200\321\202.", nullptr));
        label_12->setText(QCoreApplication::translate("FormMark", "\320\230\320\275\320\264\320\265\320\272\321\201 \321\201\320\276\321\200\321\202\320\270\321\200\320\276\320\262\320\272\320\270", nullptr));
        label_4->setText(QCoreApplication::translate("FormMark", "\320\222\320\270\320\264", nullptr));
        label_11->setText(QCoreApplication::translate("FormMark", "\320\221\321\203\320\272\320\262. \320\276\320\261\320\276\320\267\320\275", nullptr));
        label_6->setText(QCoreApplication::translate("FormMark", "\320\237\320\276\320\273\320\276\320\266. \320\277\321\200\320\270 \321\201\320\262\320\260\321\200\320\272\320\265", nullptr));
        label_7->setText(QCoreApplication::translate("FormMark", "\320\242\320\270\320\277 \320\277\320\276 \320\223\320\236\320\241\320\242", nullptr));
        label_3->setText(QCoreApplication::translate("FormMark", "\320\223\321\200\321\203\320\277\320\277\320\260", nullptr));
        label_15->setText(QCoreApplication::translate("FormMark", "\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265 \320\264\320\273\321\217 \320\272\320\260\321\202\320\260\320\273\320\276\320\263\320\260", nullptr));
        label_13->setText(QCoreApplication::translate("FormMark", "\320\224\320\276\320\277. \321\201\320\276\320\264. \320\262\320\273\320\260\320\263\320\270, %", nullptr));
        label_21->setText(QCoreApplication::translate("FormMark", "\320\236\321\201\320\276\320\261\321\213\320\265 \321\201\320\262\320\276\320\271\321\201\321\202\320\262\320\260", nullptr));
        labelPix->setText(QString());
        label_22->setText(QCoreApplication::translate("FormMark", "\320\236\321\201\320\276\320\261\320\265\320\275\320\275\320\276\321\201\321\202\320\270 \321\201\320\262\320\260\321\200\320\272\320\270", nullptr));
        label_24->setText(QCoreApplication::translate("FormMark", "\320\243\320\277\320\260\320\272\320\276\320\262\320\272\320\260, \321\210\321\202\321\200\320\270\321\205\320\272\320\276\320\264", nullptr));
        label_23->setText(QCoreApplication::translate("FormMark", "\320\245\320\260\321\200\320\260\320\272\321\202\320\265\321\200\320\270\321\201\321\202\320\270\320\272\320\270 \320\277\320\273\320\260\320\262\320\273\320\265\320\275\320\270\321\217", nullptr));
        label_30->setText(QCoreApplication::translate("FormMark", "\320\224\320\276\320\277. \320\277\321\200\320\276\320\262\320\276\320\273\320\276\320\272\320\260", nullptr));
        groupBoxVar->setTitle(QCoreApplication::translate("FormMark", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213 \320\262\320\260\321\200\320\270\320\260\320\275\321\202\320\260", nullptr));
        label_25->setText(QCoreApplication::translate("FormMark", "\320\222\320\260\321\200\320\270\320\260\320\275\321\202", nullptr));
#if QT_CONFIG(tooltip)
        toolButtonEdtVar->setToolTip(QCoreApplication::translate("FormMark", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\262\320\260\321\200\320\270\320\260\320\275\321\202\321\213", nullptr));
#endif // QT_CONFIG(tooltip)
        toolButtonEdtVar->setText(QString());
        pushButtonCreVar->setText(QCoreApplication::translate("FormMark", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", nullptr));
        pushButtonSaveVar->setText(QCoreApplication::translate("FormMark", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        pushButtonDelVar->setText(QCoreApplication::translate("FormMark", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        pushButtonCopy->setText(QCoreApplication::translate("FormMark", "\320\227\320\260\320\277\320\276\320\273\320\275\320\270\321\202\321\214", nullptr));
        label_26->setText(QCoreApplication::translate("FormMark", "\320\227\320\275\320\260\320\274\320\265\320\275\320\260\321\202\320\265\320\273\321\214", nullptr));
        label_16->setText(QCoreApplication::translate("FormMark", "\320\240\320\265\320\266\320\270\320\274 \320\277\320\276\320\262\321\202. \320\277\321\200\320\276\320\272.", nullptr));
        label_27->setText(QCoreApplication::translate("FormMark", "\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265", nullptr));
        label_19->setText(QCoreApplication::translate("FormMark", "\320\242\320\276\320\272\320\270", nullptr));
        label_20->setText(QCoreApplication::translate("FormMark", "\320\235\320\276\321\200\320\274\320\260\321\202\320\270\320\262\320\275\320\260\321\217 \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202\320\260\321\206\320\270\321\217", nullptr));
        label_17->setText(QCoreApplication::translate("FormMark", "\320\245\320\270\320\274\320\270\321\217", nullptr));
        label_18->setText(QCoreApplication::translate("FormMark", "\320\234\320\265\321\205\320\260\320\275\320\270\320\272\320\260", nullptr));
        label_28->setText(QCoreApplication::translate("FormMark", "\320\224\320\260\321\202\320\260 \321\215\321\202\320\270\320\272\320\265\321\202\320\272\320\270:", nullptr));
        label_29->setText(QCoreApplication::translate("FormMark", "\320\224\320\270\320\260\320\274.", nullptr));
        checkBoxOrder->setText(QCoreApplication::translate("FormMark", "\320\227\320\260\320\272\320\260\320\267", nullptr));
        cmdLbl->setText(QCoreApplication::translate("FormMark", "\320\255\321\202\320\270\320\272\320\265\321\202\320\272\320\260", nullptr));
        cmdLblSmall->setText(QCoreApplication::translate("FormMark", "\320\241\320\276\320\272\321\200. \321\215\321\202\320\270\320\272\320\265\321\202\320\272\320\260", nullptr));
        cmdLblSmall2->setText(QCoreApplication::translate("FormMark", "\320\241\320\276\320\272\321\200. \321\215\321\202\320\270\320\272\320\265\321\202\320\272\320\260 2", nullptr));
        cmdExt->setText(QCoreApplication::translate("FormMark", "\320\222\321\213\320\263\321\200\321\203\320\267\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormMark: public Ui_FormMark {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMMARK_H
