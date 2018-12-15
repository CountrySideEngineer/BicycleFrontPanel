#include "bicyclefrontmonitormainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BicycleFrontMonitorMainWindow w;
#ifdef PC_BUILD_CONFIG
    //Screen size of RapsberryPi display.
    w.setFixedSize(480, 320);
    w.show();
#else
    w.showFullScreen();
#endif

    return a.exec();
}
