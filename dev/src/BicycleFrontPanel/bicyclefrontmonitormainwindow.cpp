#include "bicyclefrontmonitormainwindow.h"
#include "ui_bicyclefrontmonitormainwindow.h"

/**
 * @brief Constructor.
 * @param parent
 */
BicycleFrontMonitorMainWindow::BicycleFrontMonitorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BicycleFrontMonitorMainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(480, 320);

    this->mTimer = new QTimer(this);
    this->mTimer->setInterval(100);
    this->mTimer->setSingleShot(false);

    connect(this->mTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    this->mTimer->start();

    this->mDateTimerBuilder = new CDateTimeBuilder();
    this->updateDateTime();
}

/**
 * @brief Destructor.
 */
BicycleFrontMonitorMainWindow::~BicycleFrontMonitorMainWindow()
{
    delete ui;

    this->mTimer->stop();
    delete this->mTimer;
    delete this->mDateTimerBuilder;
}

/**
 * @brief Timer dispatch event handler.
 */
void BicycleFrontMonitorMainWindow::onTimeout()
{
    this->updateDateTime();
}

/**
 * @brief Update date time view, labels.
 */
void BicycleFrontMonitorMainWindow::updateDateTime()
{
    this->ui->dateLabel->setText(this->mDateTimerBuilder->createDate()->getTime());
    this->ui->timeLabel->setText(this->mDateTimerBuilder->createTime()->getTime());

}
