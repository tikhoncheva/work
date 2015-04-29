/********************************************************************************
** Form generated from reading UI file 'testwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTWINDOW_H
#define UI_TESTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_testWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTextBrowser *textBrowser;
    QMenuBar *menubar;

    void setupUi(QMainWindow *testWindow)
    {
        if (testWindow->objectName().isEmpty())
            testWindow->setObjectName(QStringLiteral("testWindow"));
        testWindow->resize(574, 460);
        testWindow->setFocusPolicy(Qt::NoFocus);
        testWindow->setInputMethodHints(Qt::ImhMultiLine);
        centralwidget = new QWidget(testWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        gridLayout->addWidget(textBrowser, 0, 0, 1, 1);

        testWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(testWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 574, 20));
        testWindow->setMenuBar(menubar);

        retranslateUi(testWindow);

        QMetaObject::connectSlotsByName(testWindow);
    } // setupUi

    void retranslateUi(QMainWindow *testWindow)
    {
        testWindow->setWindowTitle(QApplication::translate("testWindow", "Results of test", 0));
    } // retranslateUi

};

namespace Ui {
    class testWindow: public Ui_testWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTWINDOW_H
