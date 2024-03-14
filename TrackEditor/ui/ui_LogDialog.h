/********************************************************************************
** Form generated from reading UI file 'LogDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGDIALOG_H
#define UI_LOGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LogDialog
{
public:
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *txDebug;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbClose;

    void setupUi(QDialog *LogDialog)
    {
        if (LogDialog->objectName().isEmpty())
            LogDialog->setObjectName(QString::fromUtf8("LogDialog"));
        LogDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(LogDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        txDebug = new QPlainTextEdit(LogDialog);
        txDebug->setObjectName(QString::fromUtf8("txDebug"));
        txDebug->setReadOnly(true);

        verticalLayout->addWidget(txDebug);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbClose = new QPushButton(LogDialog);
        pbClose->setObjectName(QString::fromUtf8("pbClose"));

        horizontalLayout->addWidget(pbClose);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(LogDialog);

        QMetaObject::connectSlotsByName(LogDialog);
    } // setupUi

    void retranslateUi(QDialog *LogDialog)
    {
        LogDialog->setWindowTitle(QCoreApplication::translate("LogDialog", "Debug Log", nullptr));
        pbClose->setText(QCoreApplication::translate("LogDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LogDialog: public Ui_LogDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGDIALOG_H
