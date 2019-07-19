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
#include "model/gpio_pin_config.h"

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
    , mDateTimerBuilder(new CDateTimeBuilder())
{
    this->ui->setupUi(this);

    this->setup();

    //Setup timer.
    //Timer to scan date time.
    this->mViewUpdateTimer = new QTimer(this);
    this->mViewUpdateTimer->setInterval(10);
    this->mViewUpdateTimer->setSingleShot(false);
    connect(this->mViewUpdateTimer, SIGNAL(timeout()), this, SLOT(onViewUpdateTimerTimeout()));

    //Start timers.
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
    delete this->mDateTimerBuilder;
    if (nullptr != this->mViewUpdateTimer) {
        this->mViewUpdateTimer->stop();
        while (this->mViewUpdateTimer->isActive()) {
            printf("Waiting for timer finishs\r\n");
        }
        delete this->mViewUpdateTimer;
    }
    delete this->mBrakeItemModel;
    delete this->mWheelItemModel;

    delete this->mFrontBrake;
    delete this->mRearBrake;
    delete this->mFrontWheel;
    delete this->mRearWheel;

    CGpio* instance = CGpio::GetInstance();
    instance->Finalize();
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
        this->ui->pageStack->setCurrentIndex(PAGE_INDEX_MENU_PAGE);
    } else {
        this->ui->pageStack->setCurrentIndex(PAGE_INDEX_MAIN_PAGE);
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
 * @brief BicycleFrontMonitorMainWindow::on_appCloseButton_clicked
 * @param state
 */
void BicycleFrontMonitorMainWindow::on_appCloseButton_clicked(bool /* state */)
{
    QApplication::quit();
}

void BicycleFrontMonitorMainWindow::on_moveMainPageButton_clicked(bool /* state */)
{
    this->ui->pageStack->setCurrentIndex(PAGE_INDEX_MAIN_PAGE);
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

/**
 * @brief BicycleFrontMonitorMainWindow::init   Initialize parameters and configure.
 */
void BicycleFrontMonitorMainWindow::setup()
{
    this->setupModelView();
    this->setupDevices();
    this->setupGpio();

    this->initialize();
}

/**
 * @brief BicycleFrontMonitorMainWindow::setupModelView
 * @attention   This method should be called before member functions
 *              setupDevices() and
 */
void BicycleFrontMonitorMainWindow::setupModelView()
{
    //setup model
    this->mBrakeItemModel = new CBrakeItemModel(this);
    this->mWheelItemModel = new CWheelItemModel(this);

    //Set model into view.
    this->ui->brakeWidget->setModel(this->mBrakeItemModel);
    this->ui->rpmWidget->setModel(this->mWheelItemModel);
    this->ui->velocityWidget->setModel(this->mWheelItemModel);
    this->ui->rpmWidget->SetAvailableColumnIndex(2);
    this->ui->velocityWidget->SetAvailableColumnIndex(3);
}

/**
 * @brief BicycleFrontMonitorMainWindow::setupDevices
 * @attention   This method supposed that a member function "setupModelView()" has
 *              been run.
 */
void BicycleFrontMonitorMainWindow::setupDevices()
{
    //Setup front brake configuration.
    this->mBrakeItemModel->setModelRowWithPin(
                CBrakeItemModel::MODEL_ROW_INDEX_FRONT_BRAKE_STATE, GPIO_PIN_FRONT_BRAKE);
    this->mFrontBrake = new CBrake(
                this->mBrakeItemModel, GPIO_PIN_FRONT_BRAKE, APart::PART_PIN_DIRECTION_INPUT);
    this->mFrontBrake->SetOptionPin(GPIO_PIN_OPTION_FRONT_BRAKE);

    //Setup rear brake configuration.
    this->mBrakeItemModel->setModelRowWithPin(
                CBrakeItemModel::MODEL_ROW_INDEX_REAR_BRAKE_STATE, GPIO_PIN_REAR_BRAKE);
    this->mRearBrake = new CBrake(
                this->mBrakeItemModel, GPIO_PIN_REAR_BRAKE, APart::PART_PIN_DIRECTION_INPUT);
    this->mRearBrake->SetOptionPin(GPIO_PIN_OPTION_REAR_BRAKE);

    //Setup rotate and velocity configuration.
    this->mWheelItemModel->setModelRowWithPin(
                CWheelItemModel::MODEL_ROW_INDEX_REAR_WHEEL_MODEL, GPIO_PIN_REAR_WHEEL);
    this->mWheelItemModel->setModelRowWithPin(
                CWheelItemModel::MODEL_ROW_INDEX_FRONT_WHEEL_MODEL, GPIO_PIN_FRONT_WHEEL);
    this->mFrontWheel = new CWheel(
                this->mWheelItemModel, GPIO_PIN_FRONT_WHEEL, APart::PART_PIN_DIRECTION_INPUT, 0, 100);//Update each 100msec.
    this->mRearWheel = new CWheel(
                this->mWheelItemModel, GPIO_PIN_REAR_WHEEL, APart::PART_PIN_DIRECTION_INPUT, 0, 100);
}

/**
 * @brief BicycleFrontMonitorMainWindow::setupGpio
 * @attention   This method supposed that member functions "setupModelView()" and "setupDevices()"
 *              must been run.
 */
void BicycleFrontMonitorMainWindow::setupGpio()
{
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

#define REGIST_ISR(GPIO_instance, part, edge)                       \
    do {                                                            \
        GPIO_instance->SetIsr(part->GetPin(), edge, part);          \
    } while(0)

    REGIST_ISR(instance, this->mFrontBrake, 2);     //Both rising up and falling down.
    REGIST_ISR(instance, this->mRearBrake, 2);      //Both rising up and falling down.

#define REGIST_TIMER_ISR(GPIO_instance, part)                       \
    do {                                                            \
        GPIO_instance->SetTimeIsr(part);                            \
    } while(0)

    REGIST_TIMER_ISR(instance, this->mFrontWheel);
    REGIST_TIMER_ISR(instance, this->mRearWheel);

    CGpio::CSpiMode spiMode(CGpio::CSpiMode::SPI_CHANNEL::SPI_CHANNEL_MAIN,
                            CGpio::CSpiMode::SPI_MODE::SPI_MODE_0,
                            CGpio::CSpiMode::SPI_ACTIVE_MODE::SPI_ACTIVE_MODE_LOW,
                            CGpio::CSpiMode::SPI_ACTIVE_MODE::SPI_ACTIVE_MODE_LOW,
                            CGpio::CSpiMode::SPI_CLOCK::SPI_CLOCK_125K);
    instance->SetSPI(&spiMode);
}

/**
 * @brief BicycleFrontMonitorMainWindow::initialize
 */
void BicycleFrontMonitorMainWindow::initialize()
{
    this->mFrontBrake->Initialize();
    this->mRearBrake->Initialize();
    this->mFrontWheel->Initialize();
    this->mRearWheel->Initialize();
}
