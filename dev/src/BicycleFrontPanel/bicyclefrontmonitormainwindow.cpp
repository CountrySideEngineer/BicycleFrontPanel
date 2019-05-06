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
#include "model/cimageresource.h"
#include "model/cimageresourcemanager.h"

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

    //Setup timer.
    //Timer to scan date time.
    this->mViewUpdateTimer = new QTimer();
    this->mViewUpdateTimer->setInterval(100);
    this->mViewUpdateTimer->setSingleShot(false);
    connect(this->mViewUpdateTimer, SIGNAL(timeout()), this, SLOT(onViewUpdateTimerTimeout()));

    //Timer to update parameters.
    this->mParamUpdateTimer = new QTimer();
    this->mParamUpdateTimer->setInterval(1000);
    this->mParamUpdateTimer->setSingleShot(false);
    connect(this->mParamUpdateTimer, SIGNAL(timeout()), this, SLOT(onParamUpdateTiemrTimeout()));

    //Start timers.
    this->mViewUpdateTimer->start();
    this->mParamUpdateTimer->start();

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

void BicycleFrontMonitorMainWindow::onParamUpdateTiemrTimeout()
{
    this->mBicycleState->Update();
}

/**
 * @brief BicycleFrontMonitorMainWindow::updateViews    Update views.
 */
void BicycleFrontMonitorMainWindow::updateViews()
{
    this->updateDateTime();

    CImageResource imageResource(this->mBicycleState);
    CImageResourceManager imageResourceManager;
    QPixmap image = imageResourceManager.getImageResource(imageResource);
    this->ui->mainView->setPixmap(image);

    QString rotate = QString(this->mBicycleState->GetRotateValue().c_str());
    rotate.append(" [RPM]");

    QString velocity = QString(this->mBicycleState->GetVelocityValue().c_str());
    velocity.append(" [m/h]");

    this->ui->rpmLabel->setText(rotate);
    this->ui->velocityLabel->setText(velocity);
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
    if (true == state) {
        //Set to "auto"
        this->mBicycleState->SwitchLightMode(0);
    } else {
        //Set to "manual".
        this->mBicycleState->SwitchLightMode(1);
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
