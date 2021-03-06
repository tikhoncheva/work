#include "mainwindow.h"
#include "ui_mainwindow.h"
//---------------------
#include "hrd/datadef.h"
#include "hrd/readdata.h"
#include "hrd/collectdata.h"
#include "hrd/plot.h"
//#include "hrd/distmatrix.h"
//#include "hrd/dijkstra2.h"

#include "hrd/initialsolution.h"

std::string returnFilename (const std::string& str)
{
    unsigned int found = str.find_last_of("/\\");
    return  str.substr(found+1);
}

/*
 * ------------------------------------------------------------------------------------
 */


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

    MainWindow::on_buttonOpenVillages_clicked();
    MainWindow::on_buttonOpenRoads_clicked();
    MainWindow::on_buttonOpenHouseh_clicked();

    ui->comboBoxDay->addItem("all");
    for (unsigned int i = 0;i<80; ++i)
        ui->comboBoxDay->addItem(QString::number(i+1));

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
    QString qfileName = "villages.csv";/*QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.csv)"));*/
    std::string filePath = qfileName.toStdString();
    std::string fileName = returnFilename(filePath);

    if (!fileName.empty())
    {
        ui->labFile1->setText(QString::fromStdString(fileName));

        // read data
        Village = readdata_villages(filePath);
        nV = Village.size();

        ui->textEditN->setText(QString::number(nV));

        if(nV && nH && nR)
        {
            collectdata_routine(Village, Road, Household); // calculate all important values
            ui->groupBoxPlotSetting ->setEnabled(true);
            ui->groupBoxInitialSolution ->setEnabled(true);

            for (unsigned int i =0; i<Interviewer.size(); ++i)
                ui->comboBoxInterviewer->addItem(QString::number(i+1));

            // plot edges
            plot_villages(ui->widget, Village);
        }
    }
}

void MainWindow::on_buttonOpenRoads_clicked()
{
    QString qfileName = "routes.csv";/*QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.csv)"));*/
    std::string filePath = qfileName.toStdString();
    std::string fileName = returnFilename(filePath);

    if (!fileName.empty())
    {
        ui->labFile2->setText(QString::fromStdString(fileName));

        // read data
        Road = readdata_Roads(filePath);
        nR = Road.size();

        ui->textEditR->setText(QString::number(nR));

        if(nV && nH && nR)
        {
            collectdata_routine(Village, Road, Household); // calculate all important values
            ui->groupBoxPlotSetting ->setEnabled(true);
            ui->groupBoxInitialSolution ->setEnabled(true);

            for (unsigned int i =0; i<Interviewer.size(); ++i)
                ui->comboBoxInterviewer->addItem(QString::number(i+1));

            // plot edges
            plot_villages(ui->widget, Village);
        }
    }
}

void MainWindow::on_buttonOpenHouseh_clicked()
{
    QString qfileName = "households.csv";/*QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.csv)"));*/
    std::string filePath = qfileName.toStdString();
    std::string fileName = returnFilename(filePath);

    if (!fileName.empty())
    {
        ui->labFile3->setText(QString::fromStdString(fileName));

        // read data
        Household = readdata_households(filePath);
        nH = Household.size();

        ui->textEditH->setText(QString::number(nH));

        if(nV && nH && nR)
        {
            collectdata_routine(Village, Road, Household); // calculate all important values
            ui->groupBoxPlotSetting ->setEnabled(true);
            ui->groupBoxInitialSolution ->setEnabled(true);

            for (unsigned int i =0; i<Interviewer.size(); ++i)
                ui->comboBoxInterviewer->addItem(QString::number(i+1));

            // plot edges
            plot_villages(ui->widget, Village);
        }
    }
}

//---------------------------------------------------------------------------------------------------

// plot original Graph
void MainWindow::on_buttonPlot_clicked()
{
    // plot edges
    plot_villages(ui->widget, Village);

//    ui->pushButtonInitialSolution->setEnabled(true);
}

//---------------------------------------------------------------------------------------------------

/*
 * Show names of the villages on the map
 */
void MainWindow::on_checkBoxVillageNames_clicked()
{

    ui->widget->clearItems();

    if (ui->checkBoxVillageNames->isChecked())
    {
        ui->checkBoxVillageIDs->setChecked(false);
        plot_labelsVillages(ui->widget, Village);
    }

    if (ui->checkBoxShowRoads->isChecked())
        plot_roads(ui->widget, Village, Road, distmatrix, false);

    ui->widget->replot();
}
//---------------------------------------------------------------------------------------------------

/*
 * Show IDs of the villages on the map
 */

void MainWindow::on_checkBoxVillageIDs_clicked()
{
    ui->widget->clearItems();

    if (ui->checkBoxVillageIDs->isChecked())
    {
        ui->checkBoxVillageNames->setChecked(false);
        plot_IDsVillages(ui->widget, Village);
    }

    if (ui->checkBoxShowRoads->isChecked())
        plot_roads(ui->widget, Village, Road, distmatrix, false);

    ui->widget->replot();
}


//---------------------------------------------------------------------------------------------------

/*
 * Show Roads on the map
 */
void MainWindow::on_checkBoxShowRoads_clicked()
{
    if (ui->checkBoxShowRoads->isChecked())
    {
        plot_roads(ui->widget, Village, Road, distmatrix, false);
    } else {
        ui->widget->clearItems();

        if (ui->checkBoxVillageNames->isChecked())
            plot_labelsVillages(ui->widget, Village);

        if (ui->checkBoxVillageIDs->isChecked())
            plot_IDsVillages(ui->widget, Village);

        ui->widget->replot();
    }
}


//---------------------------------------------------------------------------------------------------
/*
 * Show routes
 */

void MainWindow::on_pushButtonShowRoute_pressed()
{
    ui->widget->clearItems();
    plot_villages(ui->widget, Village);

    QString qstrK = ui->comboBoxInterviewer->currentText();
    QString qstrD = ui->comboBoxDay->currentText();
    std::string strK = qstrK.toStdString();
    std::string strD = qstrD.toStdString();

    unsigned int day;     // Day
    unsigned int k;         // Interviewer number

    ui->textEditRouteInfo->clear();

    k = atoi(strK.c_str());    // Interviewer number
    if (k < 1)
        k = 1;
    if (k > Interviewer.size())
        k = Interviewer.size();

    if (strD == "all")
    {
        day = 80;
        plot_routes(ui->widget, Village, Interviewer[k-1],predecessorsDry);

        std::stringstream ss;
        for (unsigned int d=0; d<day; ++d)
        {
            ss << "Day:" << d+1 << "  ";
            ss << "Work time:" <<Interviewer[k-1].routes[d].time << "  ";
            ss << "#VisCitys:" << Interviewer[k-1].routes[d].villages.size()-2 << "  ";
            ss << "#VisHh:" <<Interviewer[k-1].routes[d].households.size() << " ";
            ss << "\n";

            ui->textEditRouteInfo->setText(QString::fromStdString(ss.str()));
        }

    }
    else
    {
        day = atoi(strD.c_str());    // Day
        // plot route for the day Day and Interviewer k
        plot_route(ui->widget, Village, Interviewer[k-1], day, predecessorsDry);

        std::stringstream ss;
        ss << "Day:" << day << "  ";
        ss << "Work time:" <<Interviewer[k-1].routes[day-1].time << "  ";
        ss << "\n\n";

        ss << "#VisCitys:" << Interviewer[k-1].routes[day-1].villages.size()-2 << "  ";
        ss << "\n";
        for (unsigned int v=0; v<Interviewer[k-1].routes[day-1].villages.size(); ++v)
            ss << Interviewer[k-1].routes[day-1].villages[v] + 101 << " ";
        ss << "\n\n";

        ss << "#VisHh:" << Interviewer[k-1].routes[day-1].households.size() << " ";
        ss << "\n";
        for (unsigned int h=0; h<Interviewer[k-1].routes[day-1].households.size(); ++h)
            ss << Interviewer[k-1].routes[day-1].households[h] + 10001<< " ";
        ss << "\n";

        ui->textEditRouteInfo->setText(QString::fromStdString(ss.str()));

    }

    // replot everything that already was on the plot
    if (ui->checkBoxVillageNames->isChecked())
        plot_labelsVillages(ui->widget, Village);
    if (ui->checkBoxVillageIDs->isChecked())
        plot_IDsVillages(ui->widget, Village);
    if (ui->checkBoxShowRoads->isChecked())
        plot_roads(ui->widget, Village, Road, distmatrix, false);

    ui->widget->replot();



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
/*
 * Initial solution
 */

void MainWindow::on_pushButtonInitialSolution_clicked()
{
    ui->widget->clearItems();
    // replot everything that already was on the plot
    if (ui->checkBoxVillageNames->isChecked())
        plot_labelsVillages(ui->widget, Village);
    if (ui->checkBoxVillageIDs->isChecked())
        plot_IDsVillages(ui->widget, Village);
    if (ui->checkBoxShowRoads->isChecked())
        plot_roads(ui->widget, Village, Road, distmatrix, false);


    initialsolution(Village,  // villages
                    Household,               // households
                    Interviewer,             // Interviewer
                    Cleaner,                 // Cleaner how visited all unvisited hh
                    timematrixDry,           // timematrixdry
                    village_household);

    ui->pushButtonShowRoute->setEnabled(true);
}

//---------------------------------------------------------------------------------------------------



