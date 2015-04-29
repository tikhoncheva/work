#include "testwindow.h"
#include "ui_testwindow.h"

testWindow::testWindow(QMainWindow *parent, const std::string _text) :
    QMainWindow(parent), tw(new Ui::testWindow), text(_text)
{
    tw->setupUi(this);

    tw->textBrowser->append(QString::fromStdString(_text));
}

testWindow::~testWindow()
{
    delete tw;
}
