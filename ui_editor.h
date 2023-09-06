/********************************************************************************
** Form generated from reading UI file 'editor.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITOR_H
#define UI_EDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "editor.h"

QT_BEGIN_NAMESPACE

class Ui_Editor
{
public:
    QAction *actiobBold;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayoutTool;
    QHBoxLayout *horizontalLayout_2;
    TextEdit *textEdit;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayoutBox;
    QPushButton *pushButtonSertDef;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *cmd_print;
    QPushButton *cmd_pdf;
    QPushButton *pushButtonHtml;
    QComboBox *comboBoxType;
    QRadioButton *radioButtonRus;
    QRadioButton *radioButtonEn;
    QRadioButton *radioButtonMix;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *Editor)
    {
        if (Editor->objectName().isEmpty())
            Editor->setObjectName(QString::fromUtf8("Editor"));
        Editor->setWindowModality(Qt::ApplicationModal);
        Editor->resize(1024, 750);
        actiobBold = new QAction(Editor);
        actiobBold->setObjectName(QString::fromUtf8("actiobBold"));
        verticalLayout_2 = new QVBoxLayout(Editor);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayoutTool = new QVBoxLayout();
        verticalLayoutTool->setObjectName(QString::fromUtf8("verticalLayoutTool"));

        verticalLayout_2->addLayout(verticalLayoutTool);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        textEdit = new TextEdit(Editor);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setMinimumSize(QSize(830, 0));
        textEdit->setMaximumSize(QSize(830, 16777215));

        horizontalLayout_2->addWidget(textEdit);

        scrollArea = new QScrollArea(Editor);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 169, 691));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayoutBox = new QVBoxLayout();
        verticalLayoutBox->setObjectName(QString::fromUtf8("verticalLayoutBox"));

        verticalLayout->addLayout(verticalLayoutBox);

        pushButtonSertDef = new QPushButton(scrollAreaWidgetContents);
        pushButtonSertDef->setObjectName(QString::fromUtf8("pushButtonSertDef"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButtonSertDef->sizePolicy().hasHeightForWidth());
        pushButtonSertDef->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(pushButtonSertDef);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout_2->addWidget(scrollArea);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cmd_print = new QPushButton(Editor);
        cmd_print->setObjectName(QString::fromUtf8("cmd_print"));

        horizontalLayout->addWidget(cmd_print);

        cmd_pdf = new QPushButton(Editor);
        cmd_pdf->setObjectName(QString::fromUtf8("cmd_pdf"));

        horizontalLayout->addWidget(cmd_pdf);

        pushButtonHtml = new QPushButton(Editor);
        pushButtonHtml->setObjectName(QString::fromUtf8("pushButtonHtml"));

        horizontalLayout->addWidget(pushButtonHtml);

        comboBoxType = new QComboBox(Editor);
        comboBoxType->setObjectName(QString::fromUtf8("comboBoxType"));

        horizontalLayout->addWidget(comboBoxType);

        radioButtonRus = new QRadioButton(Editor);
        radioButtonRus->setObjectName(QString::fromUtf8("radioButtonRus"));
        radioButtonRus->setChecked(true);

        horizontalLayout->addWidget(radioButtonRus);

        radioButtonEn = new QRadioButton(Editor);
        radioButtonEn->setObjectName(QString::fromUtf8("radioButtonEn"));

        horizontalLayout->addWidget(radioButtonEn);

        radioButtonMix = new QRadioButton(Editor);
        radioButtonMix->setObjectName(QString::fromUtf8("radioButtonMix"));

        horizontalLayout->addWidget(radioButtonMix);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(Editor);

        QMetaObject::connectSlotsByName(Editor);
    } // setupUi

    void retranslateUi(QWidget *Editor)
    {
        Editor->setWindowTitle(QCoreApplication::translate("Editor", "\320\241\320\265\321\200\321\202\320\270\321\204\320\270\320\272\320\260\321\202", nullptr));
        actiobBold->setText(QCoreApplication::translate("Editor", "bold", nullptr));
        pushButtonSertDef->setText(QCoreApplication::translate("Editor", "\320\237\320\276 \321\203\320\274\320\276\320\273\321\207\320\260\320\275\320\270\321\216", nullptr));
        cmd_print->setText(QCoreApplication::translate("Editor", "\320\237\320\265\321\207\320\260\321\202\321\214", nullptr));
        cmd_pdf->setText(QCoreApplication::translate("Editor", "PDF", nullptr));
        pushButtonHtml->setText(QCoreApplication::translate("Editor", "HTML", nullptr));
        radioButtonRus->setText(QCoreApplication::translate("Editor", "\320\240\321\203\321\201\321\201\320\272\320\270\320\271", nullptr));
        radioButtonEn->setText(QCoreApplication::translate("Editor", "\320\220\320\275\320\263\320\273\320\270\320\271\321\201\320\272\320\270\320\271", nullptr));
        radioButtonMix->setText(QCoreApplication::translate("Editor", "\320\241\320\274\320\265\321\210\320\260\320\275\320\275\321\213\320\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Editor: public Ui_Editor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITOR_H
