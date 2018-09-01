#include "bicyclefrontmonitormainwindow.h"
#include "ui_bicyclefrontmonitormainwindow.h"

BicycleFrontMonitorMainWindow::BicycleFrontMonitorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BicycleFrontMonitorMainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(480, 320);
}

BicycleFrontMonitorMainWindow::~BicycleFrontMonitorMainWindow()
{
    delete ui;
}
