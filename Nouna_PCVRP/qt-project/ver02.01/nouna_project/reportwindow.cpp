#include "reportwindow.h"
#include "ui_reportwindow.h"

reportWindow::reportWindow(QWidget *parent, const std::vector<stInterviewer> _interviewer,
                           std::vector<std::vector<std::pair<unsigned int, double> > > _ITimePlan):
    QMainWindow(parent), rw(new Ui::reportWindow), interviewer(_interviewer), ITimePlan(_ITimePlan)
{
    rw->setupUi(this);

    // get report structure
    report_HH_yearplan = saveHHSchedule(interviewer, ITimePlan);

    // fill report table
    rw->twSchedule_hh_itime->setRowCount(report_HH_yearplan.size());
    rw->twSchedule_hh_itime->setColumnCount(7);

    QStringList Header;
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
}

reportWindow::~reportWindow()
{
    delete rw;
}

/*
 * Save report
 */

void reportWindow::on_pbSaveReport_clicked()
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
