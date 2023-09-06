/********************************************************************************
** Form generated from reading UI file 'dblogin.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBLOGIN_H
#define UI_DBLOGIN_H

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

class Ui_DbLogin
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelImg;
    QToolButton *cmdShowOpt;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *edtUser;
    QLabel *label_2;
    QLineEdit *edtPasswd;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *cmdConnect;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLineEdit *edtHost;
    QLabel *label_4;
    QSpinBox *edtPort;
    QLabel *label_5;

    void setupUi(QDialog *DbLogin)
    {
        if (DbLogin->objectName().isEmpty())
            DbLogin->setObjectName(QString::fromUtf8("DbLogin"));
        DbLogin->resize(484, 249);
        DbLogin->setMinimumSize(QSize(484, 0));
        DbLogin->setMaximumSize(QSize(484, 261));
        DbLogin->setSizeGripEnabled(false);
        verticalLayout_3 = new QVBoxLayout(DbLogin);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        labelImg = new QLabel(DbLogin);
        labelImg->setObjectName(QString::fromUtf8("labelImg"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelImg->sizePolicy().hasHeightForWidth());
        labelImg->setSizePolicy(sizePolicy);
        labelImg->setMinimumSize(QSize(170, 80));
        labelImg->setMaximumSize(QSize(170, 80));
        labelImg->setScaledContents(true);

        verticalLayout_2->addWidget(labelImg);

        cmdShowOpt = new QToolButton(DbLogin);
        cmdShowOpt->setObjectName(QString::fromUtf8("cmdShowOpt"));
        cmdShowOpt->setCheckable(true);

        verticalLayout_2->addWidget(cmdShowOpt);


        horizontalLayout_3->addLayout(verticalLayout_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(DbLogin);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        edtUser = new QLineEdit(DbLogin);
        edtUser->setObjectName(QString::fromUtf8("edtUser"));

        gridLayout->addWidget(edtUser, 0, 1, 1, 1);

        label_2 = new QLabel(DbLogin);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        edtPasswd = new QLineEdit(DbLogin);
        edtPasswd->setObjectName(QString::fromUtf8("edtPasswd"));
        edtPasswd->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(edtPasswd, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        cmdConnect = new QPushButton(DbLogin);
        cmdConnect->setObjectName(QString::fromUtf8("cmdConnect"));

        horizontalLayout_2->addWidget(cmdConnect);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);


        verticalLayout_3->addLayout(horizontalLayout_3);

        groupBox = new QGroupBox(DbLogin);
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
        QWidget::setTabOrder(edtUser, edtPasswd);
        QWidget::setTabOrder(edtPasswd, cmdConnect);
        QWidget::setTabOrder(cmdConnect, cmdShowOpt);
        QWidget::setTabOrder(cmdShowOpt, edtHost);
        QWidget::setTabOrder(edtHost, edtPort);

        retranslateUi(DbLogin);

        QMetaObject::connectSlotsByName(DbLogin);
    } // setupUi

    void retranslateUi(QDialog *DbLogin)
    {
        cmdShowOpt->setText(QCoreApplication::translate("DbLogin", "\320\264\320\276\320\277\320\276\320\273\320\275\320\270\321\202\320\265\320\273\321\214\320\275\320\276 ->", nullptr));
        label->setText(QCoreApplication::translate("DbLogin", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\214", nullptr));
        label_2->setText(QCoreApplication::translate("DbLogin", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        cmdConnect->setText(QCoreApplication::translate("DbLogin", "\320\222\321\205\320\276\320\264", nullptr));
        groupBox->setTitle(QCoreApplication::translate("DbLogin", "\320\264\320\276\320\277\320\276\320\273\320\275\320\270\321\202\320\265\320\273\321\214\320\275\321\213\320\265 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", nullptr));
        label_4->setText(QCoreApplication::translate("DbLogin", "\320\245\320\276\321\201\321\202", nullptr));
        label_5->setText(QCoreApplication::translate("DbLogin", "\320\237\320\276\321\200\321\202", nullptr));
        (void)DbLogin;
    } // retranslateUi

};

namespace Ui {
    class DbLogin: public Ui_DbLogin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBLOGIN_H
