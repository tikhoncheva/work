#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>

namespace Ui {
class testWindow;
}

class testWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit testWindow(QMainWindow *parent = 0);
    testWindow(QMainWindow *parent, const std::string _text);
    ~testWindow();

private:
    Ui::testWindow *tw;
    const std::string text;
};

#endif // TESTWINDOW_H
