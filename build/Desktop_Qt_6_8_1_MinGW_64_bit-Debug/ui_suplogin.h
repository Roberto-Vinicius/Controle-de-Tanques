/********************************************************************************
** Form generated from reading UI file 'suplogin.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUPLOGIN_H
#define UI_SUPLOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_SupLogin
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *labelLogin;
    QLineEdit *lineEditLogin;
    QLabel *labelPassword;
    QLineEdit *lineEditPassword;
    QLabel *labelServer;
    QLineEdit *lineEditServer;

    void setupUi(QDialog *SupLogin)
    {
        if (SupLogin->objectName().isEmpty())
            SupLogin->setObjectName("SupLogin");
        SupLogin->resize(240, 200);
        buttonBox = new QDialogButtonBox(SupLogin);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(20, 150, 200, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        labelLogin = new QLabel(SupLogin);
        labelLogin->setObjectName("labelLogin");
        labelLogin->setGeometry(QRect(20, 60, 60, 16));
        lineEditLogin = new QLineEdit(SupLogin);
        lineEditLogin->setObjectName("lineEditLogin");
        lineEditLogin->setGeometry(QRect(80, 60, 140, 22));
        labelPassword = new QLabel(SupLogin);
        labelPassword->setObjectName("labelPassword");
        labelPassword->setGeometry(QRect(20, 100, 60, 16));
        lineEditPassword = new QLineEdit(SupLogin);
        lineEditPassword->setObjectName("lineEditPassword");
        lineEditPassword->setGeometry(QRect(80, 100, 140, 22));
        lineEditPassword->setEchoMode(QLineEdit::Password);
        labelServer = new QLabel(SupLogin);
        labelServer->setObjectName("labelServer");
        labelServer->setGeometry(QRect(20, 20, 60, 16));
        lineEditServer = new QLineEdit(SupLogin);
        lineEditServer->setObjectName("lineEditServer");
        lineEditServer->setGeometry(QRect(80, 20, 140, 22));

        retranslateUi(SupLogin);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, SupLogin, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, SupLogin, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(SupLogin);
    } // setupUi

    void retranslateUi(QDialog *SupLogin)
    {
        SupLogin->setWindowTitle(QCoreApplication::translate("SupLogin", "Dialog", nullptr));
        labelLogin->setText(QCoreApplication::translate("SupLogin", "Login:", nullptr));
        labelPassword->setText(QCoreApplication::translate("SupLogin", "Password:", nullptr));
        labelServer->setText(QCoreApplication::translate("SupLogin", "Server (IP):", nullptr));
        lineEditServer->setText(QCoreApplication::translate("SupLogin", "127.0.0.1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SupLogin: public Ui_SupLogin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUPLOGIN_H
