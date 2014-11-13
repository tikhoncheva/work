#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcustomplot/qcustomplot.h"
#include <QMainWindow>
//------

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_buttonOpenVillages_clicked();

    void on_buttonOpenRoutes_clicked();

    void on_buttonOpenHouseh_clicked();

    void on_buttonPlot_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
