/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <qcustomplot/qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionVillages;
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout;
    QCustomPlot *widget;
    QScrollBar *horizontalScrollBar;
    QScrollBar *verticalScrollBar;
    QTabWidget *tabWidget;
    QWidget *tab_7;
    QFrame *frame;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_4;
    QTextEdit *textEditR;
    QTextEdit *textEditH;
    QPushButton *buttonOpenHouseh;
    QLabel *labFile1;
    QLabel *labFile2;
    QPushButton *buttonOpenVillages;
    QLabel *labFile3;
    QPushButton *buttonOpenRoads;
    QTextEdit *textEditN;
    QGroupBox *groupBoxPlotSetting;
    QCheckBox *checkBoxShowRoads;
    QCheckBox *checkBoxRainingSeazon;
    QCheckBox *checkBoxVillageIDs;
    QPushButton *buttonPlot;
    QCheckBox *checkBoxVillageNames;
    QWidget *tab_8;
    QGroupBox *groupBoxInitialSolution;
    QTextEdit *textEditRouteInfo;
    QLabel *labelSelectDay;
    QLabel *labelSelectInterviewer;
    QPushButton *pushButtonShowRoute;
    QPushButton *pushButtonInitialSolution;
    QComboBox *comboBoxDay;
    QComboBox *comboBoxInterviewer;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(883, 544);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionVillages = new QAction(MainWindow);
        actionVillages->setObjectName(QStringLiteral("actionVillages"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget = new QCustomPlot(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setStyleSheet(QLatin1String("#widget {\n"
"border: 2px solid gray;\n"
"border-radius: 10px;\n"
"background: white;\n"
"}"));

        gridLayout->addWidget(widget, 0, 1, 1, 1);

        horizontalScrollBar = new QScrollBar(centralWidget);
        horizontalScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(horizontalScrollBar->sizePolicy().hasHeightForWidth());
        horizontalScrollBar->setSizePolicy(sizePolicy1);
        horizontalScrollBar->setValue(0);
        horizontalScrollBar->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalScrollBar, 1, 1, 1, 1);

        verticalScrollBar = new QScrollBar(centralWidget);
        verticalScrollBar->setObjectName(QStringLiteral("verticalScrollBar"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(verticalScrollBar->sizePolicy().hasHeightForWidth());
        verticalScrollBar->setSizePolicy(sizePolicy2);
        verticalScrollBar->setValue(0);
        verticalScrollBar->setOrientation(Qt::Vertical);

        gridLayout->addWidget(verticalScrollBar, 0, 2, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setEnabled(true);
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(100);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy3);
        tabWidget->setMaximumSize(QSize(350, 16777215));
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        frame = new QFrame(tab_7);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(10, 10, 321, 141));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy4);
        frame->setMaximumSize(QSize(400, 300));
        frame->setStyleSheet(QLatin1String("#frame {\n"
"border: 2px solid gray;\n"
"border-radius: 10px;\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayoutWidget_2 = new QWidget(frame);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 10, 301, 111));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        textEditR = new QTextEdit(gridLayoutWidget_2);
        textEditR->setObjectName(QStringLiteral("textEditR"));
        textEditR->setEnabled(false);
        sizePolicy4.setHeightForWidth(textEditR->sizePolicy().hasHeightForWidth());
        textEditR->setSizePolicy(sizePolicy4);
        textEditR->setMaximumSize(QSize(80, 20));
        textEditR->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEditR->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout_4->addWidget(textEditR, 1, 2, 1, 1);

        textEditH = new QTextEdit(gridLayoutWidget_2);
        textEditH->setObjectName(QStringLiteral("textEditH"));
        textEditH->setEnabled(false);
        sizePolicy4.setHeightForWidth(textEditH->sizePolicy().hasHeightForWidth());
        textEditH->setSizePolicy(sizePolicy4);
        textEditH->setMaximumSize(QSize(80, 20));
        textEditH->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEditH->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout_4->addWidget(textEditH, 2, 2, 1, 1);

        buttonOpenHouseh = new QPushButton(gridLayoutWidget_2);
        buttonOpenHouseh->setObjectName(QStringLiteral("buttonOpenHouseh"));
        sizePolicy4.setHeightForWidth(buttonOpenHouseh->sizePolicy().hasHeightForWidth());
        buttonOpenHouseh->setSizePolicy(sizePolicy4);
        buttonOpenHouseh->setMaximumSize(QSize(90, 30));

        gridLayout_4->addWidget(buttonOpenHouseh, 2, 1, 1, 1);

        labFile1 = new QLabel(gridLayoutWidget_2);
        labFile1->setObjectName(QStringLiteral("labFile1"));
        sizePolicy4.setHeightForWidth(labFile1->sizePolicy().hasHeightForWidth());
        labFile1->setSizePolicy(sizePolicy4);
        labFile1->setMaximumSize(QSize(100, 30));
        labFile1->setAutoFillBackground(false);

        gridLayout_4->addWidget(labFile1, 0, 0, 1, 1);

        labFile2 = new QLabel(gridLayoutWidget_2);
        labFile2->setObjectName(QStringLiteral("labFile2"));
        sizePolicy4.setHeightForWidth(labFile2->sizePolicy().hasHeightForWidth());
        labFile2->setSizePolicy(sizePolicy4);
        labFile2->setMaximumSize(QSize(100, 30));
        labFile2->setAutoFillBackground(false);

        gridLayout_4->addWidget(labFile2, 1, 0, 1, 1);

        buttonOpenVillages = new QPushButton(gridLayoutWidget_2);
        buttonOpenVillages->setObjectName(QStringLiteral("buttonOpenVillages"));
        sizePolicy4.setHeightForWidth(buttonOpenVillages->sizePolicy().hasHeightForWidth());
        buttonOpenVillages->setSizePolicy(sizePolicy4);
        buttonOpenVillages->setMaximumSize(QSize(90, 30));

        gridLayout_4->addWidget(buttonOpenVillages, 0, 1, 1, 1);

        labFile3 = new QLabel(gridLayoutWidget_2);
        labFile3->setObjectName(QStringLiteral("labFile3"));
        sizePolicy4.setHeightForWidth(labFile3->sizePolicy().hasHeightForWidth());
        labFile3->setSizePolicy(sizePolicy4);
        labFile3->setMaximumSize(QSize(100, 30));
        labFile3->setAutoFillBackground(false);

        gridLayout_4->addWidget(labFile3, 2, 0, 1, 1);

        buttonOpenRoads = new QPushButton(gridLayoutWidget_2);
        buttonOpenRoads->setObjectName(QStringLiteral("buttonOpenRoads"));
        sizePolicy4.setHeightForWidth(buttonOpenRoads->sizePolicy().hasHeightForWidth());
        buttonOpenRoads->setSizePolicy(sizePolicy4);
        buttonOpenRoads->setMaximumSize(QSize(90, 30));

        gridLayout_4->addWidget(buttonOpenRoads, 1, 1, 1, 1);

        textEditN = new QTextEdit(gridLayoutWidget_2);
        textEditN->setObjectName(QStringLiteral("textEditN"));
        textEditN->setEnabled(false);
        sizePolicy4.setHeightForWidth(textEditN->sizePolicy().hasHeightForWidth());
        textEditN->setSizePolicy(sizePolicy4);
        textEditN->setMaximumSize(QSize(80, 20));
        textEditN->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEditN->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout_4->addWidget(textEditN, 0, 2, 1, 1);

        groupBoxPlotSetting = new QGroupBox(tab_7);
        groupBoxPlotSetting->setObjectName(QStringLiteral("groupBoxPlotSetting"));
        groupBoxPlotSetting->setEnabled(true);
        groupBoxPlotSetting->setGeometry(QRect(10, 160, 321, 171));
        groupBoxPlotSetting->setStyleSheet(QLatin1String("#groupBoxPlotSetting {\n"
"border: 2px solid gray;\n"
"border-radius: 10px;\n"
"}"));
        checkBoxShowRoads = new QCheckBox(groupBoxPlotSetting);
        checkBoxShowRoads->setObjectName(QStringLiteral("checkBoxShowRoads"));
        checkBoxShowRoads->setGeometry(QRect(10, 90, 111, 21));
        checkBoxRainingSeazon = new QCheckBox(groupBoxPlotSetting);
        checkBoxRainingSeazon->setObjectName(QStringLiteral("checkBoxRainingSeazon"));
        checkBoxRainingSeazon->setGeometry(QRect(10, 110, 131, 21));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(checkBoxRainingSeazon->sizePolicy().hasHeightForWidth());
        checkBoxRainingSeazon->setSizePolicy(sizePolicy5);
        checkBoxVillageIDs = new QCheckBox(groupBoxPlotSetting);
        checkBoxVillageIDs->setObjectName(QStringLiteral("checkBoxVillageIDs"));
        checkBoxVillageIDs->setEnabled(true);
        checkBoxVillageIDs->setGeometry(QRect(10, 50, 191, 21));
        buttonPlot = new QPushButton(groupBoxPlotSetting);
        buttonPlot->setObjectName(QStringLiteral("buttonPlot"));
        buttonPlot->setEnabled(true);
        buttonPlot->setGeometry(QRect(120, 20, 81, 23));
        buttonPlot->setStyleSheet(QLatin1String("#buttonPlot{\n"
"background: rgb(238, 238, 238);\n"
"}"));
        checkBoxVillageNames = new QCheckBox(groupBoxPlotSetting);
        checkBoxVillageNames->setObjectName(QStringLiteral("checkBoxVillageNames"));
        checkBoxVillageNames->setEnabled(true);
        checkBoxVillageNames->setGeometry(QRect(10, 70, 191, 21));
        tabWidget->addTab(tab_7, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QStringLiteral("tab_8"));
        groupBoxInitialSolution = new QGroupBox(tab_8);
        groupBoxInitialSolution->setObjectName(QStringLiteral("groupBoxInitialSolution"));
        groupBoxInitialSolution->setEnabled(true);
        groupBoxInitialSolution->setGeometry(QRect(5, 10, 340, 301));
        textEditRouteInfo = new QTextEdit(groupBoxInitialSolution);
        textEditRouteInfo->setObjectName(QStringLiteral("textEditRouteInfo"));
        textEditRouteInfo->setGeometry(QRect(5, 110, 332, 181));
        labelSelectDay = new QLabel(groupBoxInitialSolution);
        labelSelectDay->setObjectName(QStringLiteral("labelSelectDay"));
        labelSelectDay->setGeometry(QRect(20, 70, 31, 21));
        labelSelectInterviewer = new QLabel(groupBoxInitialSolution);
        labelSelectInterviewer->setObjectName(QStringLiteral("labelSelectInterviewer"));
        labelSelectInterviewer->setGeometry(QRect(100, 70, 71, 21));
        pushButtonShowRoute = new QPushButton(groupBoxInitialSolution);
        pushButtonShowRoute->setObjectName(QStringLiteral("pushButtonShowRoute"));
        pushButtonShowRoute->setEnabled(false);
        pushButtonShowRoute->setGeometry(QRect(220, 70, 91, 23));
        pushButtonInitialSolution = new QPushButton(groupBoxInitialSolution);
        pushButtonInitialSolution->setObjectName(QStringLiteral("pushButtonInitialSolution"));
        pushButtonInitialSolution->setEnabled(true);
        pushButtonInitialSolution->setGeometry(QRect(100, 30, 101, 23));
        comboBoxDay = new QComboBox(groupBoxInitialSolution);
        comboBoxDay->setObjectName(QStringLiteral("comboBoxDay"));
        comboBoxDay->setGeometry(QRect(50, 70, 45, 21));
        sizePolicy4.setHeightForWidth(comboBoxDay->sizePolicy().hasHeightForWidth());
        comboBoxDay->setSizePolicy(sizePolicy4);
        comboBoxDay->setMaximumSize(QSize(45, 30));
        comboBoxDay->setInputMethodHints(Qt::ImhNone);
        comboBoxDay->setMaxCount(81);
        comboBoxDay->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
        comboBoxInterviewer = new QComboBox(groupBoxInitialSolution);
        comboBoxInterviewer->setObjectName(QStringLiteral("comboBoxInterviewer"));
        comboBoxInterviewer->setGeometry(QRect(170, 70, 43, 23));
        tabWidget->addTab(tab_8, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 883, 20));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionVillages->setText(QApplication::translate("MainWindow", "Villages", 0));
        buttonOpenHouseh->setText(QApplication::translate("MainWindow", "Households...", 0));
        labFile1->setText(QApplication::translate("MainWindow", "....", 0));
        labFile2->setText(QApplication::translate("MainWindow", "....", 0));
        buttonOpenVillages->setText(QApplication::translate("MainWindow", "Villages...", 0));
        labFile3->setText(QApplication::translate("MainWindow", "....", 0));
        buttonOpenRoads->setText(QApplication::translate("MainWindow", "Roads...", 0));
        groupBoxPlotSetting->setTitle(QApplication::translate("MainWindow", "Plot Settings", 0));
        checkBoxShowRoads->setText(QApplication::translate("MainWindow", "Show Roads", 0));
        checkBoxRainingSeazon->setText(QApplication::translate("MainWindow", "Raining Season", 0));
        checkBoxVillageIDs->setText(QApplication::translate("MainWindow", "Show IDs of the villages", 0));
        buttonPlot->setText(QApplication::translate("MainWindow", "plot graph", 0));
        checkBoxVillageNames->setText(QApplication::translate("MainWindow", "Show names of the villages", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_7), QApplication::translate("MainWindow", "General", 0));
        groupBoxInitialSolution->setTitle(QString());
        labelSelectDay->setText(QApplication::translate("MainWindow", "Day", 0));
        labelSelectInterviewer->setText(QApplication::translate("MainWindow", "Interviewer", 0));
        pushButtonShowRoute->setText(QApplication::translate("MainWindow", "show routes", 0));
        pushButtonInitialSolution->setText(QApplication::translate("MainWindow", "Initial Solution", 0));
        comboBoxDay->setCurrentText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_8), QApplication::translate("MainWindow", "Initial Solution", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
