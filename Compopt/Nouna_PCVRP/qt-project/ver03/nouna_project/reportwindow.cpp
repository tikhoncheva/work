#include "reportwindow.h"
#include "ui_reportwindow.h"

/* ------------------------------------------------------------------------------------------------
 * Initialization
 * ------------------------------------------------------------------------------------------------
 */
reportWindow::reportWindow(QMainWindow *parent, const std::vector<stInterviewer> _Interviewer,
                           std::vector<std::vector<std::pair<unsigned int, double> > > _ITimePlan, bool _planType,
                           std::vector<std::vector<double> >  _distDry,
                           std::vector<std::vector<double> >  _distRain):
    QMainWindow(parent), rw(new Ui::reportWindow), Interviewer(_Interviewer), ITimePlan(_ITimePlan), planType(_planType),
    distDry(_distDry),distRain(_distRain)
{
    QStringList Header;

    rw->setupUi(this);

    /* -------------------------------------
     * HH Schedule
     * ------------------------------------
     */

    // get report structure
    if (planType == 0) // save
        report_HH_yearplan = saveHHSchedule_dayview_d(Interviewer, ITimePlan); // day time plan is given
    else // save
        report_HH_yearplan = saveHHSchedule_weekview_d(Interviewer, ITimePlan); // week time plan is given



    // fill report table
    rw->twSchedule_hh_itime->setRowCount(report_HH_yearplan.size());
    rw->twSchedule_hh_itime->setColumnCount(7);

    Header.clear();
    Header << "HH ID"<<" "<<"itime" << " " << "day" << " " << "interviewer ID";
    rw->twSchedule_hh_itime->setHorizontalHeaderLabels(Header);


    rw->twSchedule_hh_itime->setColumnWidth(1,10);
    rw->twSchedule_hh_itime->setColumnWidth(3,10);
    rw->twSchedule_hh_itime->setColumnWidth(4,200);
    rw->twSchedule_hh_itime->setColumnWidth(5,10);

    unsigned int count = 0;
    for (std::set<writeFormat2> ::iterator
         it=report_HH_yearplan.begin(); it!=report_HH_yearplan.end(); ++it)
    {
        rw->twSchedule_hh_itime->setItem(count, 0,
                                         new QTableWidgetItem(QString::number((*it).hhID)));
        rw->twSchedule_hh_itime->setItem(count, 2,
                                         new QTableWidgetItem(QString::fromStdString((*it).itime)));
        rw->twSchedule_hh_itime->setItem(count, 4,
                                         new QTableWidgetItem(QString::fromStdString((*it).day)));
        rw->twSchedule_hh_itime->setItem(count, 6,
                                         new QTableWidgetItem(QString::fromStdString((*it).Interviewer)));
        ++count;
    }

    /* -------------------------------
     * Distance matrices
     * -------------------------------
     */

    Header.clear();

    rw->twDistances_Dry->setColumnCount(distDry.size());   rw->twDistances_Dry->setRowCount(distDry.size());
    rw->twDistances_Rain->setColumnCount(distRain.size()); rw->twDistances_Rain->setRowCount(distDry.size());
    for (unsigned int j=0; j<distDry[0].size(); ++j)
    {
        Header << QString::number(j + 101);
        rw->twDistances_Dry->setColumnWidth(j,40);
        rw->twDistances_Rain->setColumnWidth(j,40);
//        rw->twDistances_Dry->setItem(0,j, new QTableWidgetItem(QString::number(j)));
        for (unsigned int i=0; i<distDry.size(); ++i)
        {
            rw->twDistances_Dry->setItem (i, j, new QTableWidgetItem(QString::number(distDry[i][j])) );
            rw->twDistances_Rain->setItem(i, j, new QTableWidgetItem(QString::number(distRain[i][j])) );
        }
    }

    rw->twDistances_Dry->setHorizontalHeaderLabels(Header);
    rw->twDistances_Dry->setVerticalHeaderLabels(Header);

    rw->twDistances_Rain->setHorizontalHeaderLabels(Header);
    rw->twDistances_Rain->setVerticalHeaderLabels(Header);

    /* ----------------------------
     * Interviewer Schedule
     * ----------------------------
     */

    for (unsigned int i =1; i<Interviewer.size(); ++i)
       rw->comboBoxInterviewer->addItem(QString::number(i+1));

    // selection of the table rows in Interviewer Schedule
    connect(rw->tableWidget_weekplans, SIGNAL(cellClicked(int, int)), this, SLOT(interviewerSchedule_weekSelected(int, int)));

    // table of week plans
    rw->tableWidget_weekplans->setRowCount(constant::nweeks * constant::P);
    rw->tableWidget_weekplans->setColumnCount(4);

    Header.clear();
    Header << "Week" << "Work time (min)" << "#Villages" << "#Households";
    rw->tableWidget_weekplans->setHorizontalHeaderLabels(Header);

    rw->tableWidget_weekplans->setColumnWidth(0,50);
    rw->tableWidget_weekplans->setColumnWidth(1,130);
//    rw->tableWidget_weekplans->setColumnWidth(1,80);
//    rw->tableWidget_weekplans->setColumnWidth(2,100);

    // table of day plans
    rw->tableWidget_dayplans->setRowCount(5);
    rw->tableWidget_dayplans->setColumnCount(5);

    Header.clear();
    Header << "Day" << "Villages" << "#Hh" << "Househols" << "Itime (min)" << "Ttime" << "Wtime";
    rw->tableWidget_dayplans->setHorizontalHeaderLabels(Header);

    rw->tableWidget_dayplans->setColumnWidth(0,50);
    rw->tableWidget_dayplans->setColumnWidth(1,200);
    rw->tableWidget_dayplans->setColumnWidth(2,30);
    rw->tableWidget_dayplans->setColumnWidth(3,300);
    rw->tableWidget_dayplans->setColumnWidth(4,100);

    // show week schedule of the first interviewer
    show_interviewerSchedule();
}

reportWindow::~reportWindow()
{
    delete rw;
}

/* ------------------------------------------------------------------------------------------------
 * Save hh schedule
 * ------------------------------------------------------------------------------------------------
 */
void reportWindow::on_pbHHSchedule_clicked()
{

    // select file name
    QString fileName = QFileDialog::getSaveFileName(this,
             "Save year schedule for households", "",
             "*.csv");
    if (!fileName.endsWith(".csv"))
        fileName += ".csv";

    std::ofstream file(fileName.toStdString().c_str());	// file to open
    assert(file.is_open() && "ERROR saveHHSchedule: File cannot be opened to write");

    file << "hh_id" << std::setw(20) << "itime" << std::setw(40)
         << "day"  << std::setw(20)  << "Interviewer ID" << std::endl;

    for (std::set<writeFormat2> ::iterator
         it=report_HH_yearplan.begin(); it!=report_HH_yearplan.end(); ++it)
    {
        file << (*it).hhID << std::setw(20) << (*it).itime << std::setw(40)
             << (*it).day  << std::setw(20) << (*it).Interviewer << std::endl;
    }

    file.close();

}
/* ------------------------------------------------------------------------------------------------
 * Show interviewer s schedules
 * ------------------------------------------------------------------------------------------------
 */

void reportWindow::on_comboBoxInterviewer_currentIndexChanged(int )
{
    show_interviewerSchedule();
}

void reportWindow::show_interviewerSchedule()
{
    unsigned int nWeeks = constant::nweeks * constant::P;     // # week

    QString qstrK;
    std::string strK;
    unsigned int k;         // Interviewer ID

    qstrK = rw->comboBoxInterviewer->currentText();
    strK = qstrK.toStdString();
    k = atoi(strK.c_str());    // Interviewer number

    for (unsigned int w=0; w<nWeeks; ++w)
    {

        rw->tableWidget_weekplans->setItem(w, 0,
                                           new QTableWidgetItem(QString::number(w+1)));

        rw->tableWidget_weekplans->setItem(w, 1,
                                           new QTableWidgetItem(QString::number(Interviewer[k-1].routes_weeks[w].time)));
        rw->tableWidget_weekplans->setItem(w, 2,
                                           new QTableWidgetItem(QString::number(Interviewer[k-1].routes_weeks[w].villages.size())));
        rw->tableWidget_weekplans->setItem(w, 3,
                                          new QTableWidgetItem(QString::number(Interviewer[k-1].routes_weeks[w].households.size())));
    }


    rw->tableWidget_dayplans->selectionModel()->clearSelection();
    rw->tableWidget_dayplans->clearContents();
}

/* ------------------------------------------------------------------------------------------------
 * Show week schedule by selecting the week
 * ------------------------------------------------------------------------------------------------
 */
void reportWindow::interviewerSchedule_weekSelected(int i, int)
{
    int week = i + 1;

    QString qstrK;
    std::string strK;
    unsigned int k;         // Interviewer ID

    qstrK = rw->comboBoxInterviewer->currentText();
    strK = qstrK.toStdString();
    k = atoi(strK.c_str());    // Interviewer number



//    std::cout << "Week route: " << std::endl;

    rw->tableWidget_dayplans->clearContents();

    if (Interviewer[k-1].routes_days.empty())
        return;

    // show day schedules for this week
    for (unsigned int d=0; d<5; ++d)
    {
        unsigned int ind = (week-1)*5+d;

//        if (Interviewer[k-1].routes_days[ind].villages.empty())
//            continue;

        // Day
        rw->tableWidget_dayplans->setItem(d, 0, new QTableWidgetItem(QString::number(ind + 1)));
        // Villages
        rw->tableWidget_dayplans->setItem(d, 1, new QTableWidgetItem(QString::fromStdString(Interviewer[k-1].visVilToString(ind))));
        // #Hh
        rw->tableWidget_dayplans->setItem(d, 2, new QTableWidgetItem(QString::number(Interviewer[k-1].routes_days[ind].households.size())));
        // Households
        rw->tableWidget_dayplans->setItem(d, 3, new QTableWidgetItem(QString::fromStdString(Interviewer[k-1].visHhToString(ind))));
        //wtime
        rw->tableWidget_dayplans->setItem(d, 4, new QTableWidgetItem(QString::number(Interviewer[k-1].routes_days[ind].time)));
    }
    rw->tableWidget_dayplans->selectionModel()->clearSelection();

}
