#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include <hrd/report.h>
#include <hrd/interviewer.h>
#include <hrd/const.h>

#include <QFileDialog>
#include <QMainWindow>
#include <QString>

#include <math.h>


namespace Ui {
class reportWindow;
}

class reportWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit reportWindow(QMainWindow *parent = 0);
    reportWindow(QMainWindow *parent, const std::vector<stInterviewer> ,
                 std::vector<std::vector<std::pair<unsigned int, double> > > ,
                 std::vector<std::vector<double> >,
                 std::vector<std::vector<double> > );
    ~reportWindow();

private slots:
    void on_pbHHSchedule_clicked();
    void interviewerSchedule_weekSelected(int i, int);
    void on_comboBoxInterviewer_currentIndexChanged(int);

private:
    Ui::reportWindow *rw;
    const std::vector<stInterviewer> Interviewer;
    std::vector<std::vector<std::pair<unsigned int, double> > > ITimePlan;
    std::vector<std::vector<double> > distDry;
    std::vector<std::vector<double> > distRain;

    std::set<rowEntries> report_HH_yearplan;

    void show_interviewerSchedule();
};

#endif // REPORTWINDOW_H
