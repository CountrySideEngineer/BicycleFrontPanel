#include "bicyclefrontmonitormainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BicycleFrontMonitorMainWindow w;
    w.show();

    return a.exec();
}
