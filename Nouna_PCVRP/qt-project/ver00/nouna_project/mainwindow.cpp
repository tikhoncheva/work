#include <QFileDialog>
#include <QColor>
#include <QPointF>
#include <limits>
//---------------------
#include "mainwindow.h"
#include "ui_mainwindow.h"
//---------------------
#include "datadef.h"
#include "distmatrix.h"
#include "readdata.h"
#include "add_func.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // configurate scrollbars
    ui->horizontalScrollBar->setRange(-500, 500);
    ui->verticalScrollBar->setRange(-500, 500);

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

// plot original Graph
void MainWindow::on_buttonPlot_clicked()
{
    ui->widget->clearGraphs();

    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(Qt::white);
    myScatter.setSize(5);

    // create graph and assign data to it:
    ui->widget->addGraph();
    ui->widget->graph(0)->setScatterStyle(myScatter);
    ui->widget->graph(0)->setLineStyle( QCPGraph::lsNone);
    // give the axes some labels:
    ui->widget->xAxis->setLabel("longitude");
    ui->widget->yAxis->setLabel("latitude");

    QVector<double> x(nV), y(nV); // initialize with entries 0..100
    double xmin,xmax;
    double ymin, ymax;
    QVector<double>::iterator it;

    for (unsigned int i=0; i< nV; ++i)
    {
        x[i] = Village[i].coord.first;
        y[i] = Village[i].coord.second;
    }

    QVector<double>::iterator it_min =  std::min_element(x.begin(), x.end());
    xmin = *it_min;
    QVector<double>::iterator it_max =  std::max_element(x.begin(), x.end());
    xmax = *it_max;

    it_min =  std::min_element(y.begin(), y.end());
    ymin = *it_min;
    it_max =  std::max_element(y.begin(), y.end());
    ymax = *it_max;

    ui->widget->graph(0)->setData(x, y);
    // set axes ranges, so we see all data:
    ui->widget->xAxis->setRange(xmin-0.1, xmax+0.1);
    ui->widget->yAxis->setRange(ymin-0.1, ymax+0.1);


    // plot edges
    //std::vector<std::vector<double> > distmatrix;
    distmatrix = compute_distmatrix(Village, Route);

    // replot everything
    ui->widget->replot();
}

/*
 * Show names of the villages on the map
 */
void MainWindow::on_checkBoxVillageNames_clicked()
{

    if (ui->checkBoxVillageNames->isChecked())
    {
        QVector<double> x(nV), y(nV); // initialize with entries 0..100

        for (unsigned int i=0; i< nV; ++i)
        {
            x[i] = Village[i].coord.first;
            y[i] = Village[i].coord.second;

            QCPItemText *textLabel = new QCPItemText(ui->widget);
            textLabel->setText(QString::fromStdString(Village[i].name));
            textLabel->position->setCoords(x[i], y[i]+0.007);
            ui->widget->addItem(textLabel);
        }

        // replot everything
        ui->widget->replot();
    } else {
        ui->widget->clearItems();
        if (ui->checkBoxShowRoutes->isChecked())
        {
            QPointF p1, p2;

            for (unsigned int i=0; i< nV; ++i)
            {
                p1.setX(Village[i].coord.first);
                p1.setY(Village[i].coord.second);

                for (unsigned int j=0; j<nV; ++j)
                {
                    if (distmatrix[i][j]!=0)
                    {
                        p2.setX(Village[j].coord.first);
                        p2.setY(Village[j].coord.second);

                        QCPItemLine *arrow = new QCPItemLine(ui->widget);
                        ui->widget->addItem(arrow);
                        arrow->start->setCoords(p1);
                        arrow->end->setCoords(p2);
                    }
                }
            }
        }
        ui->widget->replot();
    }
}

/*
 * Show Routes on the map
 */
void MainWindow::on_checkBoxShowRoutes_clicked()
{
    if (ui->checkBoxShowRoutes->isChecked())
    {
        QPointF p1, p2;

        for (unsigned int i=0; i< nV; ++i)
        {
            p1.setX(Village[i].coord.first);
            p1.setY(Village[i].coord.second);

            for (unsigned int j=0; j<nV; ++j)
            {
                if (distmatrix[i][j]!=0)
                {
                    p2.setX(Village[j].coord.first);
                    p2.setY(Village[j].coord.second);

                    QCPItemLine *arrow = new QCPItemLine(ui->widget);
                    ui->widget->addItem(arrow);
                    arrow->start->setCoords(p1);
                    arrow->end->setCoords(p2);
                }
            }
        }
        // replot everything
        ui->widget->replot();
    } else {
        ui->widget->clearItems();
        if (ui->checkBoxVillageNames->isChecked())
        {
            QVector<double> x(nV), y(nV); // initialize with entries 0..100

            for (unsigned int i=0; i< nV; ++i)
            {
                x[i] = Village[i].coord.first;
                y[i] = Village[i].coord.second;

                QCPItemText *textLabel = new QCPItemText(ui->widget);
                textLabel->setText(QString::fromStdString(Village[i].name));
                textLabel->position->setCoords(x[i], y[i]+0.007);
                ui->widget->addItem(textLabel);
            }
            // replot everything
            ui->widget->replot();
        }
    }
}

/*
 * Is it now a raining season?
 */
void MainWindow::on_checkBoxRainingSeazon_clicked()
{

}


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

