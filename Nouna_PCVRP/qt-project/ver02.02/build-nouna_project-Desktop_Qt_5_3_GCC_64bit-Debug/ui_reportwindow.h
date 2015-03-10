/********************************************************************************
** Form generated from reading UI file 'reportwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPORTWINDOW_H
#define UI_REPORTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_reportWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QTableWidget *twSchedule_hh_itime;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbSaveReport;
    QWidget *tab_2;
    QGridLayout *gridLayout_3;
    QTableWidget *twDistances_Dry;
    QWidget *tab_3;
    QGridLayout *gridLayout_2;
    QTableWidget *twDistances_Rain;
    QWidget *tab;

    void setupUi(QMainWindow *reportWindow)
    {
        if (reportWindow->objectName().isEmpty())
            reportWindow->setObjectName(QStringLiteral("reportWindow"));
        reportWindow->resize(730, 525);
        centralwidget = new QWidget(reportWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tab_1 = new QWidget();
        tab_1->setObjectName(QStringLiteral("tab_1"));
        verticalLayout = new QVBoxLayout(tab_1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        twSchedule_hh_itime = new QTableWidget(tab_1);
        twSchedule_hh_itime->setObjectName(QStringLiteral("twSchedule_hh_itime"));

        gridLayout->addWidget(twSchedule_hh_itime, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        pbSaveReport = new QPushButton(tab_1);
        pbSaveReport->setObjectName(QStringLiteral("pbSaveReport"));

        gridLayout->addWidget(pbSaveReport, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_3 = new QGridLayout(tab_2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        twDistances_Dry = new QTableWidget(tab_2);
        twDistances_Dry->setObjectName(QStringLiteral("twDistances_Dry"));

        gridLayout_3->addWidget(twDistances_Dry, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        gridLayout_2 = new QGridLayout(tab_3);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        twDistances_Rain = new QTableWidget(tab_3);
        twDistances_Rain->setObjectName(QStringLiteral("twDistances_Rain"));

        gridLayout_2->addWidget(twDistances_Rain, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tabWidget->addTab(tab, QString());

        verticalLayout_2->addWidget(tabWidget);

        reportWindow->setCentralWidget(centralwidget);

        retranslateUi(reportWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(reportWindow);
    } // setupUi

    void retranslateUi(QMainWindow *reportWindow)
    {
        reportWindow->setWindowTitle(QApplication::translate("reportWindow", "Report", 0));
        pbSaveReport->setText(QApplication::translate("reportWindow", "Save", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QApplication::translate("reportWindow", "HH Year Schedule", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("reportWindow", "Distances Dry Season", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("reportWindow", "Distances Rain Season", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("reportWindow", "Page", 0));
    } // retranslateUi

};

namespace Ui {
    class reportWindow: public Ui_reportWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPORTWINDOW_H
