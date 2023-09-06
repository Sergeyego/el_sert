/********************************************************************************
** Form generated from reading UI file 'checkform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHECKFORM_H
#define UI_CHECKFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CheckForm
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditCod;
    QPushButton *cmdGo;
    QTextEdit *textEdit;

    void setupUi(QWidget *CheckForm)
    {
        if (CheckForm->objectName().isEmpty())
            CheckForm->setObjectName(QString::fromUtf8("CheckForm"));
        CheckForm->resize(475, 167);
        verticalLayout = new QVBoxLayout(CheckForm);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CheckForm);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEditCod = new QLineEdit(CheckForm);
        lineEditCod->setObjectName(QString::fromUtf8("lineEditCod"));

        horizontalLayout->addWidget(lineEditCod);

        cmdGo = new QPushButton(CheckForm);
        cmdGo->setObjectName(QString::fromUtf8("cmdGo"));

        horizontalLayout->addWidget(cmdGo);


        verticalLayout->addLayout(horizontalLayout);

        textEdit = new QTextEdit(CheckForm);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout->addWidget(textEdit);


        retranslateUi(CheckForm);

        QMetaObject::connectSlotsByName(CheckForm);
    } // setupUi

    void retranslateUi(QWidget *CheckForm)
    {
        CheckForm->setWindowTitle(QCoreApplication::translate("CheckForm", "\320\237\321\200\320\276\320\262\320\265\321\200\320\270\321\202\321\214 \320\272\320\276\320\264 \320\277\320\276\320\264\320\273\320\270\320\275\320\275\320\276\321\201\321\202\320\270", nullptr));
        label->setText(QCoreApplication::translate("CheckForm", "\320\232\320\276\320\264 \320\277\320\276\320\264\320\273\320\270\320\275\320\275\320\276\321\201\321\202\320\270", nullptr));
        cmdGo->setText(QCoreApplication::translate("CheckForm", "\320\237\321\200\320\276\320\262\320\265\321\200\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CheckForm: public Ui_CheckForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHECKFORM_H
