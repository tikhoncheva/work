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

    // initialize axis range und zooming
    ui->widget->xAxis->setRange(-4.1, -3.2, Qt::AlignCenter);
    ui->widget->yAxis->setRange(12.4, 13, Qt::AlignCenter);
    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // draw routes by selecting table row
    connect(ui->tableWidget_weekplans, SIGNAL(cellClicked(int, int)), this, SLOT(weekSelected(int, int)));
    connect(ui->tableWidget_dayplans, SIGNAL(cellClicked(int, int)), this, SLOT(daySelected(int, int)));

    // table of week plans
    ui->tableWidget_weekplans->setRowCount(constant::nweeks * constant::P);
    ui->tableWidget_weekplans->setColumnCount(4);

    QStringList Header;
    Header << "Week"<<"Work time"<<"#Villages" << "#Households";
    ui->tableWidget_weekplans->setHorizontalHeaderLabels(Header);


    ui->tableWidget_weekplans->setColumnWidth(0,45);
    ui->tableWidget_weekplans->setColumnWidth(1,80);
    ui->tableWidget_weekplans->setColumnWidth(2,70);
    ui->tableWidget_weekplans->setColumnWidth(3,90);

    // table of day plans
    ui->tableWidget_dayplans->setRowCount(5);
    ui->tableWidget_dayplans->setColumnCount(4);

    Header.clear();
    Header << "Day"<<"Work time"<<"Villages" << "Households";
    ui->tableWidget_dayplans->setHorizontalHeaderLabels(Header);

//    ui->tableWidget_dayplans->setItem(0,0, new QTableWidgetItem(QString::number(1)));
//    ui->tableWidget_dayplans->setItem(1,0, new QTableWidgetItem(QString::number(2)));
//    ui->tableWidget_dayplans->setItem(2,0, new QTableWidgetItem(QString::number(3)));
//    ui->tableWidget_dayplans->setItem(3,0, new QTableWidgetItem(QString::number(4)));
//    ui->tableWidget_dayplans->setItem(4,0, new QTableWidgetItem(QString::number(5)));

    ui->tableWidget_dayplans->setColumnWidth(0,45);
    ui->tableWidget_dayplans->setColumnWidth(1,80);
    ui->tableWidget_dayplans->setColumnWidth(2,70);
    ui->tableWidget_dayplans->setColumnWidth(3,90);

    // default file selection
    MainWindow::on_buttonOpenVillages_clicked();
    MainWindow::on_buttonOpenRoads_clicked();
    MainWindow::on_buttonOpenHouseh_clicked();
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
    QString qfileName = "../data/villages.csv";/*QFileDialog::getOpenFileName(this, tr("Open File"),
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
            //ui->groupBoxPlotSetting ->setEnabled(true);
            //ui->tab_initsolution->setEnabled(true);

            for (unsigned int i =0; i<Interviewer.size(); ++i)
                ui->comboBoxInterviewer->addItem(QString::number(i+1));

            // plot edges
            plot_villages(ui->widget, Village);
        }
    }
}

void MainWindow::on_buttonOpenRoads_clicked()
{
    QString qfileName = "../data/routes.csv";/*QFileDialog::getOpenFileName(this, tr("Open File"),
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
            //ui->groupBoxPlotSetting ->setEnabled(true);
            //ui->tab_initsolution->setEnabled(true);

            for (unsigned int i =0; i<Interviewer.size(); ++i)
                ui->comboBoxInterviewer->addItem(QString::number(i+1));

            // plot edges
            plot_villages(ui->widget, Village);
        }
    }
}

void MainWindow::on_buttonOpenHouseh_clicked()
{
    QString qfileName = "../data/households.csv";/*QFileDialog::getOpenFileName(this, tr("Open File"),
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
            //ui->groupBoxPlotSetting ->setEnabled(true);
            //ui->tab_initsolution->setEnabled(true);

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
        plot_roads(ui->widget, Village, Road, distmatrix);

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
        plot_roads(ui->widget, Village, Road, distmatrix);

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
        plot_roads(ui->widget, Village, Road, distmatrix);
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

void MainWindow::on_pushButtonShowRoute_clicked()
{
    ui->widget->clearItems();

    // replot everything that already was on the plot
    if (ui->checkBoxVillageNames->isChecked())
        plot_labelsVillages(ui->widget, Village);
    if (ui->checkBoxVillageIDs->isChecked())
        plot_IDsVillages(ui->widget, Village);
    if (ui->checkBoxShowRoads->isChecked())
        plot_roads(ui->widget, Village, Road, distmatrix);

    plot_villages(ui->widget, Village);

    QString qstrK = ui->comboBoxInterviewer->currentText();
    std::string strK = qstrK.toStdString();

    unsigned int week = constant::nweeks * constant::P;     // # week
    unsigned int k;         // Interviewer number


    k = atoi(strK.c_str());    // Interviewer number
    if (k < 1)
        k = 1;
    if (k > Interviewer.size())
        k = Interviewer.size();

    for (unsigned int w=0; w<week; ++w)
    {
        ui->tableWidget_weekplans->setItem(w, 0, new QTableWidgetItem(QString::number(w+1)));
        ui->tableWidget_weekplans->setItem(w, 1, new QTableWidgetItem(QString::number(Interviewer[k-1].routes_weeks[w].time)));
        ui->tableWidget_weekplans->setItem(w, 2, new QTableWidgetItem(QString::number(Interviewer[k-1].routes_weeks[w].villages.size()-2)));
        ui->tableWidget_weekplans->setItem(w, 3, new QTableWidgetItem(QString::number(Interviewer[k-1].routes_weeks[w].households.size())));
    }

    ui->tableWidget_dayplans->selectionModel()->clearSelection();
    ui->tableWidget_dayplans->clearContents();

    ui->widget->replot();
}

/*
 * Plot interviewer's route beim selecting the row of the tableWidget_weekplans
 */

void MainWindow::weekSelected(int i, int)
{
    int week = i + 1;

    QString qstrK = ui->comboBoxInterviewer->currentText();
    std::string strK = qstrK.toStdString();

    unsigned int k = atoi(strK.c_str());    // Interviewer number
    if (k < 1)
        k = 1;
    if (k > Interviewer.size())
        k = Interviewer.size();

    ui->widget->clearItems();

    std::cout << "Week route: " << std::endl;
    for (unsigned int i = 0; i< Interviewer[k-1].routes_weeks[week-1].villages.size(); ++i)
         std::cout << Interviewer[k-1].routes_weeks[week-1].villages[i] + 101  << " " ;
    std::cout << std::endl << std::endl;

    std::cout << "Households: " << std::endl;
    for (unsigned int i = 0; i< Interviewer[k-1].routes_weeks[week-1].households.size(); ++i)
         std::cout << Interviewer[k-1].routes_weeks[week-1].households[i] + 10001  << " " ;
    std::cout << std::endl << std::endl;

    // replot everything that already was on the plot
    if (ui->checkBoxVillageNames->isChecked())
        plot_labelsVillages(ui->widget, Village);
    if (ui->checkBoxVillageIDs->isChecked())
        plot_IDsVillages(ui->widget, Village);
    if (ui->checkBoxShowRoads->isChecked())
        plot_roads(ui->widget, Village, Road, distmatrix);

    plot_route_week(ui->widget, Village, Interviewer[k-1], week, predecessorsDry);
    plot_villages(ui->widget, Village);

    ui->widget->replot();

    ui->tableWidget_dayplans->clearContents();

    if (Interviewer[k-1].routes_days.empty())
        return;

    // show day schedules for this week
    for (unsigned int d=0; d<5; ++d)
    {
        unsigned int ind = (week-1)*5+d;

        if (Interviewer[k-1].routes_days[ind].villages.empty())
            continue;

        ui->tableWidget_dayplans->setItem(d, 0, new QTableWidgetItem(QString::number(d+1)));

        ui->tableWidget_dayplans->setItem(d, 1, new QTableWidgetItem(QString::number(Interviewer[k-1].routes_days[ind].time)));

        ui->tableWidget_dayplans->setItem(d, 2, new QTableWidgetItem(QString::fromStdString(Interviewer[k-1].visVilToString(ind))));

//        ui->tableWidget_dayplans->setItem(d, 3, new QTableWidgetItem(QString::fromStdString(Interviewer[k-1].visHhToString(ind))));
        ui->tableWidget_dayplans->setItem(d, 3, new QTableWidgetItem(QString::number(Interviewer[k-1].routes_days[ind].households.size())));
    }
    ui->tableWidget_dayplans->selectionModel()->clearSelection();

}


void MainWindow::daySelected(int i, int)
{
    int week = ui->tableWidget_weekplans->currentRow();
    int day = i+1;

    QString qstrK = ui->comboBoxInterviewer->currentText();
    std::string strK = qstrK.toStdString();
    unsigned int k = atoi(strK.c_str());    // Interviewer number
    if (k < 1)
        k = 1;
    if (k > Interviewer.size())
        k = Interviewer.size();

    // replot everything that already was on the plot
    if (ui->checkBoxVillageNames->isChecked())
        plot_labelsVillages(ui->widget, Village);
    if (ui->checkBoxVillageIDs->isChecked())
        plot_IDsVillages(ui->widget, Village);
    if (ui->checkBoxShowRoads->isChecked())
        plot_roads(ui->widget, Village, Road, distmatrix);

    plot_route_day(ui->widget, Village, Interviewer[k-1], 5*week + day, predecessorsDry);
    plot_villages(ui->widget, Village);

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
 * Zooming the plot widget with mouse
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
        plot_roads(ui->widget, Village, Road, distmatrix);


    initialsolution(Village,  // villages
                    Household,               // households
                    Interviewer,             // Interviewer
                    timematrixDry,           // time matrix dry
                    timematrixRain,           // time matrix rain
                    village_household,
                    TimeInfo);

    ui->pushButtonShowRoute->setEnabled(true);
    ui->comboBoxInterviewer->setEnabled(true);

    std::cout << "Find initial solution ... finished" << std::endl;
}

//---------------------------------------------------------------------------------------------------





