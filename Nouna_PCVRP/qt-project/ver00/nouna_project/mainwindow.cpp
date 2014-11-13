#include <QFileDialog>
#include <QColor>
#include <limits>
//---------------------
#include "mainwindow.h"
#include "ui_mainwindow.h"
//---------------------
#include "datadef.h"
#include "readdata.h"
#include "add_func.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
        if (x[i]<xmin)
            xmin = x[i];
        if (x[i]>xmax)
            xmax = x[i];
        y[i] = Village[i].coord.second;

        if (y[i]<ymin)
            ymin = y[i];
        if (y[i]>ymax)
            ymax = y[i];

        QCPItemText *textLabel = new QCPItemText(ui->widget);
        textLabel->setText(QString::fromStdString(Village[i].name));
        textLabel->position->setCoords(x[i], y[i]+0.007);
        ui->widget->addItem(textLabel);
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

    for (unsigned int i=0; i< nR; ++i)
    {
        unsigned int startId  = Route[i].start;
        unsigned int endId  = Route[i].end;

        QVector<double>::iterator it_start = std::find(Village.begin(),Village.end(), startId);

        QVector<double>::iterator it_end = std::find(Village.begin(),Village.end(), endId);

    }

    // replot everything
    ui->widget->replot();
}
