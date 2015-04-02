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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
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
    QGridLayout *gridLayout_4;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QTableWidget *twSchedule_hh_itime;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbHHSchedule;
    QWidget *tab_4;
    QGridLayout *gridLayout_5;
    QLabel *label_Interviewer;
    QLabel *label_wSchedule;
    QTableWidget *tableWidget_dayplans;
    QComboBox *comboBoxInterviewer;
    QLabel *label_dSchedule;
    QTableWidget *tableWidget_weekplans;
    QWidget *tab_2;
    QGridLayout *gridLayout_3;
    QTableWidget *twDistances_Dry;
    QWidget *tab_3;
    QGridLayout *gridLayout_2;
    QTableWidget *twDistances_Rain;

    void setupUi(QMainWindow *reportWindow)
    {
        if (reportWindow->objectName().isEmpty())
            reportWindow->setObjectName(QStringLiteral("reportWindow"));
        reportWindow->resize(732, 550);
        centralwidget = new QWidget(reportWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout_4 = new QGridLayout(centralwidget);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
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

        pbHHSchedule = new QPushButton(tab_1);
        pbHHSchedule->setObjectName(QStringLiteral("pbHHSchedule"));

        gridLayout->addWidget(pbHHSchedule, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        tabWidget->addTab(tab_1, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        gridLayout_5 = new QGridLayout(tab_4);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        label_Interviewer = new QLabel(tab_4);
        label_Interviewer->setObjectName(QStringLiteral("label_Interviewer"));

        gridLayout_5->addWidget(label_Interviewer, 0, 1, 1, 1);

        label_wSchedule = new QLabel(tab_4);
        label_wSchedule->setObjectName(QStringLiteral("label_wSchedule"));

        gridLayout_5->addWidget(label_wSchedule, 0, 0, 1, 1);

        tableWidget_dayplans = new QTableWidget(tab_4);
        tableWidget_dayplans->setObjectName(QStringLiteral("tableWidget_dayplans"));
        tableWidget_dayplans->setMaximumSize(QSize(16777215, 16777215));
        tableWidget_dayplans->setInputMethodHints(Qt::ImhMultiLine);
        tableWidget_dayplans->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_dayplans->setTextElideMode(Qt::ElideMiddle);

        gridLayout_5->addWidget(tableWidget_dayplans, 3, 0, 1, 3);

        comboBoxInterviewer = new QComboBox(tab_4);
        comboBoxInterviewer->setObjectName(QStringLiteral("comboBoxInterviewer"));
        comboBoxInterviewer->setEnabled(true);

        gridLayout_5->addWidget(comboBoxInterviewer, 0, 2, 1, 1);

        label_dSchedule = new QLabel(tab_4);
        label_dSchedule->setObjectName(QStringLiteral("label_dSchedule"));

        gridLayout_5->addWidget(label_dSchedule, 2, 0, 1, 1);

        tableWidget_weekplans = new QTableWidget(tab_4);
        tableWidget_weekplans->setObjectName(QStringLiteral("tableWidget_weekplans"));
        tableWidget_weekplans->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setPointSize(9);
        tableWidget_weekplans->setFont(font);
        tableWidget_weekplans->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_weekplans->setTextElideMode(Qt::ElideMiddle);
        tableWidget_weekplans->verticalHeader()->setVisible(false);

        gridLayout_5->addWidget(tableWidget_weekplans, 1, 0, 1, 3);

        tabWidget->addTab(tab_4, QString());
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

        gridLayout_4->addWidget(tabWidget, 0, 0, 1, 1);

        reportWindow->setCentralWidget(centralwidget);

        retranslateUi(reportWindow);

        tabWidget->setCurrentIndex(1);
        comboBoxInterviewer->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(reportWindow);
    } // setupUi

    void retranslateUi(QMainWindow *reportWindow)
    {
        reportWindow->setWindowTitle(QApplication::translate("reportWindow", "Report", 0));
        pbHHSchedule->setText(QApplication::translate("reportWindow", "Save", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QApplication::translate("reportWindow", "HH Year Schedule", 0));
        label_Interviewer->setText(QApplication::translate("reportWindow", "Interviewer", 0));
        label_wSchedule->setText(QApplication::translate("reportWindow", "Weekly schedule", 0));
        comboBoxInterviewer->clear();
        comboBoxInterviewer->insertItems(0, QStringList()
         << QApplication::translate("reportWindow", "1", 0)
        );
        label_dSchedule->setText(QApplication::translate("reportWindow", "Day schedule", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("reportWindow", "Interviewer Schedule", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("reportWindow", "Distances Dry Season", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("reportWindow", "Distances Rain Season", 0));
    } // retranslateUi

};

namespace Ui {
    class reportWindow: public Ui_reportWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPORTWINDOW_H
