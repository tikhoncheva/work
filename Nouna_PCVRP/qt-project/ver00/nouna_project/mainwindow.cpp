#include <QFileDialog>
#include <QColor>
#include <QPointF>
#include <limits>
//---------------------
#include "mainwindow.h"
#include "ui_mainwindow.h"
//---------------------
#include "hrd/datadef.h"
#include "hrd/distmatrix.h"
#include "hrd/readdata.h"
#include "hrd/add_func.h"
#include "hrd/plot.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // configurate scrollbars
    ui->horizontalScrollBar->setRange(-410, -290);
    ui->verticalScrollBar->setRange(-1300, -1240);

    // create connection between axes and scroll bars:
    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

    // initialize axis range (and scroll bar positions via signals we just connected):
    ui->widget->xAxis->setRange(-4.1, -3.2, Qt::AlignCenter);
    ui->widget->yAxis->setRange(12.4, 13, Qt::AlignCenter);

    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//---------------------------------------------------------------------------------------------------

/*
 * Read data from files
 */

void MainWindow::on_buttonOpenVillages_clicked()
{
    QString qfileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.csv)"));
    std::string filePath = qfileName.toStdString();
    std::string fileName = returnFilename(filePath);

    ui->labFile1->setText(QString::fromStdString(fileName));

    // read data
    Village = readdata_villages(filePath);
    nV = Village.size();

    ui->textEditN->setText(QString::number(nV));

    if(nV && nH && nR)
        ui->buttonPlot->setEnabled(true);
}

void MainWindow::on_buttonOpenRoutes_clicked()
{
    QString qfileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.csv)"));
    std::string filePath = qfileName.toStdString();
    std::string fileName = returnFilename(filePath);

    ui->labFile2->setText(QString::fromStdString(fileName));

    // read data
    Route = readdata_routes(filePath);
    nR = Route.size();

    ui->textEditR->setText(QString::number(nR));

    if(nV && nH && nR)
        ui->buttonPlot->setEnabled(true);
}

void MainWindow::on_buttonOpenHouseh_clicked()
{
    QString qfileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.csv)"));
    std::string filePath = qfileName.toStdString();
    std::string fileName = returnFilename(filePath);

    ui->labFile3->setText(QString::fromStdString(fileName));

    // read data
    Household = readdata_households(filePath);
    nH = Household.size();

    ui->textEditH->setText(QString::number(nH));

    if(nV && nH && nR)
        ui->buttonPlot->setEnabled(true);
}

//---------------------------------------------------------------------------------------------------

// plot original Graph
void MainWindow::on_buttonPlot_clicked()
{
    // plot edges
    //std::vector<std::vector<double> > distmatrix;
    distmatrix = compute_distmatrix(Village, Route);

    plot_villages(ui->widget, Village);
}

//---------------------------------------------------------------------------------------------------

/*
 * Show names of the villages on the map
 */
void MainWindow::on_checkBoxVillageNames_clicked()
{

    if (ui->checkBoxVillageNames->isChecked())
    {
         plot_labelsVillages(ui->widget, Village);
    } else
    {
        ui->widget->clearItems();
        if (ui->checkBoxShowRoutes->isChecked())
        {
            plot_routes(ui->widget, Village, Route, distmatrix, false);
        }
        ui->widget->replot();
    }
}

//---------------------------------------------------------------------------------------------------

/*
 * Show Routes on the map
 */
void MainWindow::on_checkBoxShowRoutes_clicked()
{
    if (ui->checkBoxShowRoutes->isChecked())
    {
        plot_routes(ui->widget, Village, Route, distmatrix, false);
    } else {
        ui->widget->clearItems();
        if (ui->checkBoxVillageNames->isChecked())
        {
           plot_labelsVillages(ui->widget, Village);
        }
        ui->widget->replot();
    }
}

//---------------------------------------------------------------------------------------------------

/*
 * Is it now a raining season?
 */
void MainWindow::on_checkBoxRainingSeazon_clicked()
{

}

//---------------------------------------------------------------------------------------------------

/*
 * Scrolling the plot widget
 */
void MainWindow::on_horizontalScrollBar_valueChanged(int value)
{
    if (qAbs(ui->widget->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
    {
        ui->widget->xAxis->setRange(value/100.0, ui->widget->xAxis->range().size(), Qt::AlignCenter);
        ui->widget->replot();
    }

}

void MainWindow::on_verticalScrollBar_valueChanged(int value)
{
    if (qAbs(ui->widget->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
    {
        ui->widget->yAxis->setRange(-value/100.0, ui->widget->yAxis->range().size(), Qt::AlignCenter);
        ui->widget->replot();
    }
}

//---------------------------------------------------------------------------------------------------

/*
 * Zooming with mouse
 */
void MainWindow::xAxisChanged(QCPRange range)
{
  ui->horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
  ui->horizontalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

void MainWindow::yAxisChanged(QCPRange range)
{
  ui->verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
  ui->verticalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

//---------------------------------------------------------------------------------------------------
