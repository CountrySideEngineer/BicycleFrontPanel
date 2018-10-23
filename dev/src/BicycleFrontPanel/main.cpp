#include "bicyclefrontmonitormainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BicycleFrontMonitorMainWindow w;
#if 0
    w.show();
#else
    w.showFullScreen();
#endif

    return a.exec();
}
