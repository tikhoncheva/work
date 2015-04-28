#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "hrd/datadef.h"
#include "hrd/readdata.h"
#include "hrd/collectdata.h"
#include "hrd/plot.h"

//#include "hrd/distmatrix.h"
//#include "hrd/dijkstra.h"

#include "hrd/initialsolution2.h"
#include "hrd/report.h"

#include "hrd/test.h"

//----------------

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
    ui->tableWidget_weekplans->setColumnCount(3);

    QStringList Header;
    Header << "Work time" << "#Villages" << "#Households";
    ui->tableWidget_weekplans->setHorizontalHeaderLabels(Header);

    ui->tableWidget_weekplans->setColumnWidth(0,80);
    ui->tableWidget_weekplans->setColumnWidth(1,80);
    ui->tableWidget_weekplans->setColumnWidth(2,100);

    // table of day plans
    ui->tableWidget_dayplans->setRowCount(5);
    ui->tableWidget_dayplans->setColumnCount(5);

    Header.clear();
    Header << "Day" << "Villages" << "#Hh" << "Househols" << "Itime" << "Ttime" << "Wtime";
    ui->tableWidget_dayplans->setHorizontalHeaderLabels(Header);

    ui->tableWidget_dayplans->setColumnWidth(0,30);
    ui->tableWidget_dayplans->setColumnWidth(1,100);
    ui->tableWidget_dayplans->setColumnWidth(2,30);
    ui->tableWidget_dayplans->setColumnWidth(3,130);
    ui->tableWidget_dayplans->setColumnWidth(4,50);

    // show report window
    connect(ui->pbShow_report, SIGNAL(clicked()), this, SLOT(showReportWindow()));

    // default file selection
    MainWindow::on_buttonOpenVillages_clicked();
    MainWindow::on_buttonOpenRoads_clicked();
    MainWindow::on_buttonOpenHouseh_clicked();
}

MainWindow::~MainWindow()
{
    // clear Villages
    Village.clear();
    std::vector<stVillage> Village2;
    Village.swap(Village2);

    // clear Roads
    Road.clear();
    std::vector<stVillage> Road2;
    Village.swap(Road2);


    // clear Household
    Household.clear();
    std::vector<stVillage> Household2;
    Village.swap(Household2);


    // clear Interviewer
    Interviewer.clear();
    std::vector<stVillage> Interviewer2;
    Village.swap(Interviewer2);

    for (unsigned int i=0; i< distmatrix.size(); ++i)
    {
        std::vector<uDist> dist_tmp;
        distmatrix[i].clear();
        distmatrix[i].swap(dist_tmp);
    }



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

    unsigned int weeks = constant::nweeks * constant::P;     // # week
    unsigned int k;         // Interviewer number


    k = atoi(strK.c_str());    // Interviewer number
    if (k < 1)
        k = 1;
    if (k > Interviewer.size())
        k = Interviewer.size();


    for (unsigned int w=0; w<weeks; ++w)
    {
        ui->tableWidget_weekplans->setItem(w, 0,
                                           new QTableWidgetItem(QString::number(Interviewer[k-1].routes_weeks[w].time)));
        ui->tableWidget_weekplans->setItem(w, 1,
                                           new QTableWidgetItem(QString::number(Interviewer[k-1].routes_weeks[w].villages.size())));
        ui->tableWidget_weekplans->setItem(w, 2,
                                          new QTableWidgetItem(QString::number(Interviewer[k-1].routes_weeks[w].households.size())));
    }


    ui->tableWidget_dayplans->selectionModel()->clearSelection();
    ui->tableWidget_dayplans->clearContents();

    ui->widget->replot();
}

/*
 * Plot interviewer's route by selecting the row of the tableWidget_weekplans
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

//    std::cout << "Week route: " << std::endl;
//    for (unsigned int i = 0; i< Interviewer[k-1].routes_weeks[week-1].villages.size(); ++i)
//         std::cout << Interviewer[k-1].routes_weeks[week-1].villages[i] + 101  << " " ;
//    std::cout << std::endl << std::endl;

//    std::cout << "Households: " << std::endl;
//    for (unsigned int i = 0; i< Interviewer[k-1].routes_weeks[week-1].households.size(); ++i)
//         std::cout << Interviewer[k-1].routes_weeks[week-1].households[i] + 10001  << " " ;
//    std::cout << std::endl << std::endl;

    // replot everything that already was on the plot
    if (ui->checkBoxVillageNames->isChecked())
        plot_labelsVillages(ui->widget, Village);
    if (ui->checkBoxVillageIDs->isChecked())
        plot_IDsVillages(ui->widget, Village);
    if (ui->checkBoxShowRoads->isChecked())
        plot_roads(ui->widget, Village, Road, distmatrix);

    plot_route_week(ui->widget, Village, Interviewer[k-1], week-1, predecessorsDry);
    plot_villages(ui->widget, Village);

    ui->widget->replot();

    ui->tableWidget_dayplans->clearContents();

    if (Interviewer[k-1].routes_days.empty())
        return;

    // show day schedules for this week
    for (unsigned int d=0; d<5; ++d)
    {
        unsigned int ind = (week-1)*5+d;

//        if (Interviewer[k-1].routes_days[ind].villages.empty())
//            continue;

        // Day
        ui->tableWidget_dayplans->setItem(d, 0, new QTableWidgetItem(QString::number(ind + 1)));
        // Villages
        ui->tableWidget_dayplans->setItem(d, 1, new QTableWidgetItem(QString::fromStdString(Interviewer[k-1].visVilToString(ind))));
        // #Hh
        ui->tableWidget_dayplans->setItem(d, 2, new QTableWidgetItem(QString::number(Interviewer[k-1].routes_days[ind].households.size())));
        // Households
        ui->tableWidget_dayplans->setItem(d, 3, new QTableWidgetItem(QString::fromStdString(Interviewer[k-1].visHhToString(ind))));
        //wtime
        ui->tableWidget_dayplans->setItem(d, 4, new QTableWidgetItem(QString::number(Interviewer[k-1].routes_days[ind].time)));
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

//    std::cout << "Households to visit on the day " << day << ": " << std::endl;
//    for (unsigned int i=0; i<Interviewer[k-1].routes_days[5*week + day-1].households.size(); ++i)
//        std::cout << Interviewer[k-1].routes_days[5*week + day-1].households[i] + 10001 << "  ";
//    std::cout << std::endl;

    ui->widget->replot();
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

//    initialsolution(Village,  // villages
//                    Household,               // households
//                    Interviewer,             // Interviewer
//                    timematrixDry,           // time matrix dry
//                    timematrixRain,           // time matrix rain
//                    village_households,
//                    TimeInfo,
//                    hhITimePlan_day);

    int success;

    success = initialsolution2(Village,  // villages
                    Household,               // households
                    Interviewer,             // Interviewer
                    timematrixDry,           // time matrix dry
                    timematrixRain,           // time matrix rain
                    village_households,
                    TimeInfo,
                    hhITimePlan_week);

//        saveHH_ITPlan_w (hhITimePlan_week, "../results/hh_itime_plan_week.txt");
//        saveHHSchedule2_w(Interviewer, hhITimePlan_week, "../results/hh_schedule_init.txt");
//        saveHH_ITPlan (hhITimePlan_day, "../results/hh_itime_plan.txt");
    //saveHHSchedule(Interviewer, hhITimePlan, "../results/hh_schedule_init.txt");

    if (success)
    {
        // run tests on the found solution
        clTest test(Interviewer, Household, hhITimePlan_week);
        test.run();

        ui->pushButtonShowRoute->setEnabled(true);
        ui->comboBoxInterviewer->setEnabled(true);
        ui->pbShow_report->setEnabled(true);    // show report
     }

    std::cout << "Find initial solution ... finished" << std::endl;
}

//---------------------------------------------------------------------------------------------------


/*
 * Show window with report forms
 */

void MainWindow::showReportWindow()
{
//    reportWindow *rw = new reportWindow(0, Interviewer, hhITimePlan_day,  0);
    // 0  hhITimePlan_day  day view
    // 1  hhITimePlan week week view
    reportWindow *rw = new reportWindow(this, Interviewer, hhITimePlan_week, 1, timematrixDry, timematrixRain);
    rw->showMinimized();
}
