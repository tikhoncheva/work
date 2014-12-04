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
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qcustomplot/qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionVillages;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QScrollBar *horizontalScrollBar;
    QGroupBox *groupBoxPar;
    QLabel *labelNumOfVillages;
    QLabel *labelNumOfHh;
    QTextEdit *textEditN;
    QTextEdit *textEditH;
    QTextEdit *textEditR;
    QLabel *labelNumOfR;
    QPushButton *buttonPlot;
    QGroupBox *groupBox;
    QPushButton *pushButtonInitialSolution;
    QCustomPlot *widget;
    QScrollBar *verticalScrollBar;
    QGroupBox *groupBoxPlotSetting;
    QCheckBox *checkBoxVillageNames;
    QCheckBox *checkBoxShowRoutes;
    QCheckBox *checkBoxRainingSeazon;
    QFrame *frame;
    QPushButton *buttonOpenVillages;
    QLabel *labFile1;
    QLabel *labFile2;
    QLabel *labFile3;
    QPushButton *buttonOpenRoutes;
    QPushButton *buttonOpenHouseh;
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
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalScrollBar = new QScrollBar(centralWidget);
        horizontalScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
        horizontalScrollBar->setValue(0);
        horizontalScrollBar->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalScrollBar, 3, 2, 1, 1);

        groupBoxPar = new QGroupBox(centralWidget);
        groupBoxPar->setObjectName(QStringLiteral("groupBoxPar"));
        groupBoxPar->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBoxPar->sizePolicy().hasHeightForWidth());
        groupBoxPar->setSizePolicy(sizePolicy1);
        groupBoxPar->setMaximumSize(QSize(400, 300));
        labelNumOfVillages = new QLabel(groupBoxPar);
        labelNumOfVillages->setObjectName(QStringLiteral("labelNumOfVillages"));
        labelNumOfVillages->setGeometry(QRect(10, 30, 57, 15));
        labelNumOfHh = new QLabel(groupBoxPar);
        labelNumOfHh->setObjectName(QStringLiteral("labelNumOfHh"));
        labelNumOfHh->setGeometry(QRect(10, 50, 81, 16));
        textEditN = new QTextEdit(groupBoxPar);
        textEditN->setObjectName(QStringLiteral("textEditN"));
        textEditN->setGeometry(QRect(120, 30, 81, 21));
        textEditN->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEditN->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEditH = new QTextEdit(groupBoxPar);
        textEditH->setObjectName(QStringLiteral("textEditH"));
        textEditH->setGeometry(QRect(120, 50, 81, 21));
        textEditH->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEditH->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEditR = new QTextEdit(groupBoxPar);
        textEditR->setObjectName(QStringLiteral("textEditR"));
        textEditR->setGeometry(QRect(120, 70, 81, 21));
        textEditR->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEditR->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        labelNumOfR = new QLabel(groupBoxPar);
        labelNumOfR->setObjectName(QStringLiteral("labelNumOfR"));
        labelNumOfR->setGeometry(QRect(10, 70, 81, 16));
        buttonPlot = new QPushButton(groupBoxPar);
        buttonPlot->setObjectName(QStringLiteral("buttonPlot"));
        buttonPlot->setEnabled(false);
        buttonPlot->setGeometry(QRect(70, 110, 81, 23));
        buttonPlot->setStyleSheet(QLatin1String("#buttonPlot{\n"
"background: rgb(238, 238, 238);\n"
"}"));
        groupBox = new QGroupBox(groupBoxPar);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(209, 0, 191, 141));
        pushButtonInitialSolution = new QPushButton(groupBox);
        pushButtonInitialSolution->setObjectName(QStringLiteral("pushButtonInitialSolution"));
        pushButtonInitialSolution->setGeometry(QRect(20, 30, 101, 23));

        gridLayout->addWidget(groupBoxPar, 2, 0, 1, 1);

        widget = new QCustomPlot(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy2);
        widget->setStyleSheet(QLatin1String("#widget {\n"
"border: 2px solid gray;\n"
"border-radius: 10px;\n"
"background: white;\n"
"}"));

        gridLayout->addWidget(widget, 0, 2, 3, 1);

        verticalScrollBar = new QScrollBar(centralWidget);
        verticalScrollBar->setObjectName(QStringLiteral("verticalScrollBar"));
        verticalScrollBar->setValue(0);
        verticalScrollBar->setOrientation(Qt::Vertical);

        gridLayout->addWidget(verticalScrollBar, 0, 3, 3, 1);

        groupBoxPlotSetting = new QGroupBox(centralWidget);
        groupBoxPlotSetting->setObjectName(QStringLiteral("groupBoxPlotSetting"));
        groupBoxPlotSetting->setStyleSheet(QLatin1String("#groupBoxPlotSetting {\n"
"border: 2px solid gray;\n"
"border-radius: 10px;\n"
"}"));
        checkBoxVillageNames = new QCheckBox(groupBoxPlotSetting);
        checkBoxVillageNames->setObjectName(QStringLiteral("checkBoxVillageNames"));
        checkBoxVillageNames->setGeometry(QRect(10, 20, 191, 21));
        checkBoxShowRoutes = new QCheckBox(groupBoxPlotSetting);
        checkBoxShowRoutes->setObjectName(QStringLiteral("checkBoxShowRoutes"));
        checkBoxShowRoutes->setGeometry(QRect(10, 40, 111, 21));
        checkBoxRainingSeazon = new QCheckBox(groupBoxPlotSetting);
        checkBoxRainingSeazon->setObjectName(QStringLiteral("checkBoxRainingSeazon"));
        checkBoxRainingSeazon->setGeometry(QRect(10, 60, 131, 21));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(checkBoxRainingSeazon->sizePolicy().hasHeightForWidth());
        checkBoxRainingSeazon->setSizePolicy(sizePolicy3);

        gridLayout->addWidget(groupBoxPlotSetting, 1, 0, 1, 1);

        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setMaximumSize(QSize(400, 300));
        frame->setStyleSheet(QLatin1String("#frame {\n"
"border: 2px solid gray;\n"
"border-radius: 10px;\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        buttonOpenVillages = new QPushButton(frame);
        buttonOpenVillages->setObjectName(QStringLiteral("buttonOpenVillages"));
        buttonOpenVillages->setGeometry(QRect(110, 20, 91, 23));
        labFile1 = new QLabel(frame);
        labFile1->setObjectName(QStringLiteral("labFile1"));
        labFile1->setGeometry(QRect(20, 20, 91, 21));
        labFile1->setAutoFillBackground(false);
        labFile2 = new QLabel(frame);
        labFile2->setObjectName(QStringLiteral("labFile2"));
        labFile2->setGeometry(QRect(20, 70, 91, 21));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(labFile2->sizePolicy().hasHeightForWidth());
        labFile2->setSizePolicy(sizePolicy4);
        labFile2->setAutoFillBackground(false);
        labFile3 = new QLabel(frame);
        labFile3->setObjectName(QStringLiteral("labFile3"));
        labFile3->setGeometry(QRect(20, 110, 91, 21));
        labFile3->setAutoFillBackground(false);
        buttonOpenRoutes = new QPushButton(frame);
        buttonOpenRoutes->setObjectName(QStringLiteral("buttonOpenRoutes"));
        buttonOpenRoutes->setGeometry(QRect(110, 70, 91, 23));
        buttonOpenHouseh = new QPushButton(frame);
        buttonOpenHouseh->setObjectName(QStringLiteral("buttonOpenHouseh"));
        buttonOpenHouseh->setGeometry(QRect(110, 110, 91, 23));

        gridLayout->addWidget(frame, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

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

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionVillages->setText(QApplication::translate("MainWindow", "Villages", 0));
        groupBoxPar->setTitle(QApplication::translate("MainWindow", "Parameters", 0));
        labelNumOfVillages->setText(QApplication::translate("MainWindow", "Villages", 0));
        labelNumOfHh->setText(QApplication::translate("MainWindow", "Households", 0));
        labelNumOfR->setText(QApplication::translate("MainWindow", "Routes", 0));
        buttonPlot->setText(QApplication::translate("MainWindow", "plot graph", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Solution", 0));
        pushButtonInitialSolution->setText(QApplication::translate("MainWindow", "Initial Solution", 0));
        groupBoxPlotSetting->setTitle(QApplication::translate("MainWindow", "Plot Settings", 0));
        checkBoxVillageNames->setText(QApplication::translate("MainWindow", "Show names of the villages", 0));
        checkBoxShowRoutes->setText(QApplication::translate("MainWindow", "Show Routes", 0));
        checkBoxRainingSeazon->setText(QApplication::translate("MainWindow", "Raining Season", 0));
        buttonOpenVillages->setText(QApplication::translate("MainWindow", "Villages...", 0));
        labFile1->setText(QApplication::translate("MainWindow", "....", 0));
        labFile2->setText(QApplication::translate("MainWindow", "....", 0));
        labFile3->setText(QApplication::translate("MainWindow", "....", 0));
        buttonOpenRoutes->setText(QApplication::translate("MainWindow", "Routes...", 0));
        buttonOpenHouseh->setText(QApplication::translate("MainWindow", "Households...", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
