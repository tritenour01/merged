#include <QApplication>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    window* w = window::getInstance();
    w->show();

    return a.exec();
}
