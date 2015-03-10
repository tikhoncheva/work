#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include <hrd/report.h>

#include <QFileDialog>
#include <QMainWindow>


namespace Ui {
class reportWindow;
}

class reportWindow : public QMainWindow
{
    Q_OBJECT

public:
    reportWindow(QWidget *parent, const std::vector<stInterviewer> _interviewer,
                 std::vector<std::vector<std::pair<unsigned int, double> > > _ITimePlan);
    ~reportWindow();

private slots:
    void on_pbSaveReport_clicked();

private:
    Ui::reportWindow *rw;
    const std::vector<stInterviewer> interviewer;
    std::vector<std::vector<std::pair<unsigned int, double> > > ITimePlan;

    std::set<writeFormat2> report_HH_yearplan;
};

#endif // REPORTWINDOW_H
