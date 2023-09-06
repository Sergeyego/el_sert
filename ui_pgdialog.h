/********************************************************************************
** Form generated from reading UI file 'pgdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PGDIALOG_H
#define UI_PGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PgDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QToolButton *cmdShowOpt;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLineEdit *edtPasswd;
    QLabel *labelUser;
    QLineEdit *edtUser;
    QLabel *labelPass;
    QLabel *labelKey;
    QLineEdit *lineEditKey;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *cmdConnect;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLineEdit *edtHost;
    QLabel *label_4;
    QSpinBox *edtPort;
    QLabel *label_5;

    void setupUi(QDialog *PgDialog)
    {
        if (PgDialog->objectName().isEmpty())
            PgDialog->setObjectName(QString::fromUtf8("PgDialog"));
        PgDialog->resize(484, 261);
        PgDialog->setMinimumSize(QSize(484, 0));
        PgDialog->setMaximumSize(QSize(484, 261));
        PgDialog->setSizeGripEnabled(false);
        verticalLayout_3 = new QVBoxLayout(PgDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_3 = new QLabel(PgDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(200, 100));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/images/simb_labl.png")));
        label_3->setScaledContents(true);

        verticalLayout_2->addWidget(label_3);

        cmdShowOpt = new QToolButton(PgDialog);
        cmdShowOpt->setObjectName(QString::fromUtf8("cmdShowOpt"));
        cmdShowOpt->setCheckable(true);

        verticalLayout_2->addWidget(cmdShowOpt);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        edtPasswd = new QLineEdit(PgDialog);
        edtPasswd->setObjectName(QString::fromUtf8("edtPasswd"));
        edtPasswd->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(edtPasswd, 2, 1, 1, 1);

        labelUser = new QLabel(PgDialog);
        labelUser->setObjectName(QString::fromUtf8("labelUser"));

        gridLayout->addWidget(labelUser, 1, 0, 1, 1);

        edtUser = new QLineEdit(PgDialog);
        edtUser->setObjectName(QString::fromUtf8("edtUser"));

        gridLayout->addWidget(edtUser, 1, 1, 1, 1);

        labelPass = new QLabel(PgDialog);
        labelPass->setObjectName(QString::fromUtf8("labelPass"));

        gridLayout->addWidget(labelPass, 2, 0, 1, 1);

        labelKey = new QLabel(PgDialog);
        labelKey->setObjectName(QString::fromUtf8("labelKey"));

        gridLayout->addWidget(labelKey, 0, 0, 1, 1);

        lineEditKey = new QLineEdit(PgDialog);
        lineEditKey->setObjectName(QString::fromUtf8("lineEditKey"));
        lineEditKey->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(lineEditKey, 0, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        cmdConnect = new QPushButton(PgDialog);
        cmdConnect->setObjectName(QString::fromUtf8("cmdConnect"));

        horizontalLayout_2->addWidget(cmdConnect);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);


        verticalLayout_3->addLayout(horizontalLayout_3);

        groupBox = new QGroupBox(PgDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        edtHost = new QLineEdit(groupBox);
        edtHost->setObjectName(QString::fromUtf8("edtHost"));

        gridLayout_2->addWidget(edtHost, 0, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        edtPort = new QSpinBox(groupBox);
        edtPort->setObjectName(QString::fromUtf8("edtPort"));
        edtPort->setMaximum(65535);
        edtPort->setValue(5432);

        gridLayout_2->addWidget(edtPort, 1, 1, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox);

        verticalLayout_3->setStretch(0, 2);
        verticalLayout_3->setStretch(1, 1);
        QWidget::setTabOrder(lineEditKey, edtUser);
        QWidget::setTabOrder(edtUser, edtPasswd);
        QWidget::setTabOrder(edtPasswd, cmdConnect);
        QWidget::setTabOrder(cmdConnect, cmdShowOpt);
        QWidget::setTabOrder(cmdShowOpt, edtHost);
        QWidget::setTabOrder(edtHost, edtPort);

        retranslateUi(PgDialog);

        QMetaObject::connectSlotsByName(PgDialog);
    } // setupUi

    void retranslateUi(QDialog *PgDialog)
    {
        label_3->setText(QString());
        cmdShowOpt->setText(QCoreApplication::translate("PgDialog", "\320\264\320\276\320\277\320\276\320\273\320\275\320\270\321\202\320\265\320\273\321\214\320\275\320\276 ->", nullptr));
        edtPasswd->setText(QString());
        labelUser->setText(QCoreApplication::translate("PgDialog", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\214", nullptr));
        edtUser->setText(QString());
        labelPass->setText(QCoreApplication::translate("PgDialog", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        labelKey->setText(QCoreApplication::translate("PgDialog", "\320\232\320\273\321\216\321\207", nullptr));
        lineEditKey->setText(QString());
        cmdConnect->setText(QCoreApplication::translate("PgDialog", "\320\222\321\205\320\276\320\264", nullptr));
        groupBox->setTitle(QCoreApplication::translate("PgDialog", "\320\264\320\276\320\277\320\276\320\273\320\275\320\270\321\202\320\265\320\273\321\214\320\275\321\213\320\265 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", nullptr));
        label_4->setText(QCoreApplication::translate("PgDialog", "\320\245\320\276\321\201\321\202", nullptr));
        label_5->setText(QCoreApplication::translate("PgDialog", "\320\237\320\276\321\200\321\202", nullptr));
        (void)PgDialog;
    } // retranslateUi

};

namespace Ui {
    class PgDialog: public Ui_PgDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PGDIALOG_H
