/********************************************************************************
** Form generated from reading UI file 'ZanataTool.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZANATATOOL_H
#define UI_ZANATATOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ZanataToolClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *btnAddInput;
    QPushButton *btnClearInput;
    QLabel *lblAddInputHint;
    QSpacerItem *verticalSpacer;
    QListWidget *lstInputList;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnOutputFolder;
    QLineEdit *textOutputFolder;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *checkTruncOutput;
    QCheckBox *checkOpenAfterFinished;
    QPushButton *btnWork;

    void setupUi(QMainWindow *ZanataToolClass)
    {
        if (ZanataToolClass->objectName().isEmpty())
            ZanataToolClass->setObjectName(QStringLiteral("ZanataToolClass"));
        ZanataToolClass->resize(563, 525);
        centralWidget = new QWidget(ZanataToolClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        btnAddInput = new QPushButton(centralWidget);
        btnAddInput->setObjectName(QStringLiteral("btnAddInput"));

        verticalLayout->addWidget(btnAddInput);

        btnClearInput = new QPushButton(centralWidget);
        btnClearInput->setObjectName(QStringLiteral("btnClearInput"));

        verticalLayout->addWidget(btnClearInput);

        lblAddInputHint = new QLabel(centralWidget);
        lblAddInputHint->setObjectName(QStringLiteral("lblAddInputHint"));

        verticalLayout->addWidget(lblAddInputHint);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        lstInputList = new QListWidget(centralWidget);
        lstInputList->setObjectName(QStringLiteral("lstInputList"));

        horizontalLayout->addWidget(lstInputList);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        btnOutputFolder = new QPushButton(centralWidget);
        btnOutputFolder->setObjectName(QStringLiteral("btnOutputFolder"));

        horizontalLayout_2->addWidget(btnOutputFolder);

        textOutputFolder = new QLineEdit(centralWidget);
        textOutputFolder->setObjectName(QStringLiteral("textOutputFolder"));

        horizontalLayout_2->addWidget(textOutputFolder);


        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        checkTruncOutput = new QCheckBox(centralWidget);
        checkTruncOutput->setObjectName(QStringLiteral("checkTruncOutput"));
        checkTruncOutput->setChecked(true);

        verticalLayout_2->addWidget(checkTruncOutput);

        checkOpenAfterFinished = new QCheckBox(centralWidget);
        checkOpenAfterFinished->setObjectName(QStringLiteral("checkOpenAfterFinished"));
        checkOpenAfterFinished->setChecked(true);
        checkOpenAfterFinished->setTristate(false);

        verticalLayout_2->addWidget(checkOpenAfterFinished);


        verticalLayout_3->addLayout(verticalLayout_2);

        btnWork = new QPushButton(centralWidget);
        btnWork->setObjectName(QStringLiteral("btnWork"));

        verticalLayout_3->addWidget(btnWork);


        verticalLayout_4->addLayout(verticalLayout_3);

        ZanataToolClass->setCentralWidget(centralWidget);

        retranslateUi(ZanataToolClass);

        QMetaObject::connectSlotsByName(ZanataToolClass);
    } // setupUi

    void retranslateUi(QMainWindow *ZanataToolClass)
    {
        ZanataToolClass->setWindowTitle(QApplication::translate("ZanataToolClass", "ZanataTool", nullptr));
        btnAddInput->setText(QApplication::translate("ZanataToolClass", "\346\267\273\345\212\240PO\346\226\207\344\273\266\345\244\271", nullptr));
        btnClearInput->setText(QApplication::translate("ZanataToolClass", "\346\270\205\347\251\272", nullptr));
        lblAddInputHint->setText(QApplication::translate("ZanataToolClass", "\350\257\267\346\214\211\347\205\247\345\205\210\346\227\247\345\220\216\346\226\260\n"
"\347\232\204\351\241\272\345\272\217\346\267\273\345\212\240", nullptr));
        btnOutputFolder->setText(QApplication::translate("ZanataToolClass", "\351\200\211\346\213\251YML\350\276\223\345\207\272\346\226\207\344\273\266\345\244\271", nullptr));
        checkTruncOutput->setText(QApplication::translate("ZanataToolClass", "\346\270\205\347\251\272\350\276\223\345\207\272\346\226\207\344\273\266\345\244\271", nullptr));
        checkOpenAfterFinished->setText(QApplication::translate("ZanataToolClass", "\345\256\214\346\210\220\345\220\216\346\211\223\345\274\200\350\276\223\345\207\272\346\226\207\344\273\266\345\244\271", nullptr));
        btnWork->setText(QApplication::translate("ZanataToolClass", "\345\220\210\345\271\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ZanataToolClass: public Ui_ZanataToolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZANATATOOL_H
