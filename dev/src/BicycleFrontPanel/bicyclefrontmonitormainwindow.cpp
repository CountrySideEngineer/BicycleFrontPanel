#include <QtGui>
#include <QIcon>
#include <QPixmap>
#include <QPalette>
#include "bicyclefrontmonitormainwindow.h"
#include "ui_bicyclefrontmonitormainwindow.h"
#include "model/cgpio.h"
#include "model/cbrake.h"
#include "model/cwheel.h"
#include "model/cwheelvelocity.h"

/**
 * @brief Constructor.
 * @param parent
 */
BicycleFrontMonitorMainWindow::BicycleFrontMonitorMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BicycleFrontMonitorMainWindow)
    , mIsLightOn(false)
    , mIsLightSwManual(false)
    , mIsHoldFrontBrake(false)
    , mIsHoldRearBrake(false)
    , mViewUpdateTimer(new QTimer(this))
    , mDateTimerBuilder(new CDateTimeBuilder())
    , mBicycleState(new CBicycleState())
{
    ui->setupUi(this);

    this->setFixedSize(480, 32);

    //Setup timer.
    //Timer to scan date time.
    this->mViewUpdateTimer = new QTimer();
    this->mViewUpdateTimer->setInterval(100);
    this->mViewUpdateTimer->setSingleShot(false);
    connect(this->mViewUpdateTimer, SIGNAL(timeout()), this, SLOT(onViewUpdateTimerTimeout()));
    this->mViewUpdateTimer->start();

    //Style sheet
    QFile StyleSheetFile(tr(":resources/qss/stylesheet.qss"));
    if (!StyleSheetFile.open(QFile::ReadOnly)) {
        qDebug() << "Can not open resources/qss/stylesheet.qss";
    } else {
        QString StyleSheetContent = QString::fromLatin1(StyleSheetFile.readAll());
        qApp->setStyleSheet(StyleSheetContent);
        StyleSheetFile.close();
    }
}

/**
 * @brief Destructor.
 */
BicycleFrontMonitorMainWindow::~BicycleFrontMonitorMainWindow()
{
    if (nullptr != this->mViewUpdateTimer) {
        delete this->mViewUpdateTimer;
        this->mViewUpdateTimer = nullptr;
    }
    if (nullptr != this->mDateTimerBuilder) {
        delete  this->mDateTimerBuilder;
        this->mDateTimerBuilder = nullptr;
    }
    delete this->mBicycleState;
}

/**
 * @brief Timer dispatch event handler.
 */
void BicycleFrontMonitorMainWindow::onViewUpdateTimerTimeout()
{
    this->updateViews();
}

/**
 * @brief BicycleFrontMonitorMainWindow::updateViews    Update views.
 */
void BicycleFrontMonitorMainWindow::updateViews()
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

/**
 * @brief BicycleFrontMonitorMainWindow::on_menuButton_toggled  Event handler of menuButton toggled.
 * @param state State of Button. The value is true if the button is pushed. and it is false if the button
 *              is released.
 */
void BicycleFrontMonitorMainWindow::on_menuButton_toggled(bool state)
{
    if (true == state) {
        this->ui->pageStack->setCurrentIndex(1);
    } else {
        this->ui->pageStack->setCurrentIndex(0);
    }
}

/**
 * @brief BicycleFrontMonitorMainWindow::on_lightConfigButton_toggled   Event handler of lightConfig toggled.
 * @param state State of Button. The value is true if the button is pushed. and it is false if the button
 *              is released.
 *              If the value is true, the mode of light is set "auto", otherwise set "manual".
 */
void BicycleFrontMonitorMainWindow::on_lightConfigButton_toggled(bool state)
{
    /*
     * @ToDo:
     *      Write code here to change light control configuration, auto or manual.
     */
    if (true == state) {
        //Set to "auto"
    } else {
        //Set to "manual".
    }
}


/**
 * @brief Change light state view.
 */
void BicycleFrontMonitorMainWindow::updateLightState() {}

/**
 * @brief Update light sw state, manual or automation.
 */
void BicycleFrontMonitorMainWindow::updateLightManualSw() {}

//Temporary button event handler.
void BicycleFrontMonitorMainWindow::onLightSw() {}
void BicycleFrontMonitorMainWindow::onLightAutoManSw() {}
