#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QColor>
#include <QPointF>

#include <limits>
#include <sstream>

#include "reportwindow.h"
#include "qcustomplot/qcustomplot.h"
//---------------------



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

    void on_buttonOpenRoads_clicked();

    void on_buttonOpenHouseh_clicked();

    void on_buttonPlot_clicked();

    void on_checkBoxVillageNames_clicked();

    void on_checkBoxShowRoads_clicked();

    void on_checkBoxRainingSeazon_clicked();

    void on_horizontalScrollBar_valueChanged(int value);

    void on_verticalScrollBar_valueChanged(int value);

    void xAxisChanged(QCPRange range);

    void yAxisChanged(QCPRange range);

    void on_pushButtonInitialSolution_clicked();

    void on_pushButtonShowRoute_pressed();

    void on_checkBoxVillageIDs_clicked();

    void weekSelected(int, int);

    void daySelected(int, int);

    void on_pushButtonShowRoute_clicked();

    void showReportWindow();

private:
    Ui::MainWindow *ui;
    Ui::reportWindow *rw;
};

#endif // MAINWINDOW_H
