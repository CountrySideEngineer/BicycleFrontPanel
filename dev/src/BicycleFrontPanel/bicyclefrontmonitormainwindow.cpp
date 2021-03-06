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

    this->ui->pageStack->setCurrentIndex(PAGE_INDEX_MAIN_PAGE);
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
 * @brief BicycleFrontMonitorMainWindow::on_menuButton_clicked  Event handler of menuButton clicked.
 *                                                              Every time clicked, the main page view
 *                                                              is changed.
 */
void BicycleFrontMonitorMainWindow::on_menuButton_clicked(bool /* state */)
{
    this->ui->pageStack->setCurrentIndex(PAGE_INDEX_MENU_PAGE);
    this->ui->centralWidget->setFocus();
}

/**
 * @brief BicycleFrontMonitorMainWindow::on_lightConfigButton_clicked
 * @param state
 */
void BicycleFrontMonitorMainWindow::on_lightConfigButton_clicked(bool state)
{
    Q_UNUSED(state);

    this->ui->pageStack->setCurrentIndex(PAGE_INDEX_LIGHT_CONFIG_PAGE);
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
    this->mBrakeItemModel->setModelColWithPin(
                CBrakeItemModel::MODEL_COL_INDEX_FRONT_BRAKE_STATE, GPIO_PIN_FRONT_BRAKE);
    this->mBrakeItemModel->setModelRowWithPin(
                CBrakeItemModel::MODEL_ROW_INDEX_BRAKE_STATE, GPIO_PIN_FRONT_BRAKE);
    this->mFrontBrake = new CBrake(
                this->mBrakeItemModel, GPIO_PIN_FRONT_BRAKE, APart::PART_PIN_DIRECTION_INPUT);
    this->mFrontBrake->SetOptionPin(GPIO_PIN_OPTION_FRONT_BRAKE);

    //Setup light.
    this->mBrakeItemModel->setModelColWithPin(
                CBrakeItemModel::MODEL_COL_INDEX_LIGHT_TURN_ON_REQUEST, GPIO_PIN_LIGHT_INPUT);
    this->mBrakeItemModel->setModelRowWithPin(
                CBrakeItemModel::MODEL_ROW_INDEX_LIGHT_STATE, GPIO_PIN_LIGHT_INPUT);
    this->mLight = new CLight(
                this->mBrakeItemModel, GPIO_PIN_LIGHT_INPUT, GPIO_PIN_LIGHT_OUTPUT);
    this->mBrakeItemModel->setLightPtr(dynamic_cast<CLight*>(this->mLight));

    //Setup rear brake configuration.
    this->mBrakeItemModel->setModelColWithPin(
                CBrakeItemModel::MODEL_COL_INDEX_REAR_BRAKE_STATE, GPIO_PIN_REAR_BRAKE);
    this->mBrakeItemModel->setModelRowWithPin(
                CBrakeItemModel::MODEL_ROW_INDEX_BRAKE_STATE, GPIO_PIN_REAR_BRAKE);
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
    REGIST_ISR(instance, this->mLight, 2);          //Both rising up and falling down.

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
    this->mLight->Initialize();

    this->initializeViews();
}

/**
 * @brief BicycleFrontMonitorMainWindow::on_lightAutoManualSwitch_toggled   Event handler of
 *                                                                          auto/manual switch
 *                                                                          toggled.
 * @param state State of auto/manual switch, on or off.
 */
void BicycleFrontMonitorMainWindow::on_lightAutoManualSwitch_toggled(bool state)
{
    int mode = 0;
    bool isManualEnable = false;
    QString titleToShow;
    if (false == state) {
        mode = CBrakeItemModel::LIGHT_AUTO_MANUAL_MODE_AUTO;
        isManualEnable = false;
        titleToShow = QString::fromLocal8Bit("自動");
    } else {
        mode = CBrakeItemModel::LIGHT_AUTO_MANUAL_MODE_MANUAL;
        titleToShow = QString::fromLocal8Bit("手動");
        isManualEnable = true;
    }
    this->mBrakeItemModel->changeLightAutoManMode(mode);
    this->ui->lightAutoManualSwitch->setText(titleToShow);
    this->ui->lightManualOnOffSwitch->setEnabled(isManualEnable);
}

/**
 * @brief BicycleFrontMonitorMainWindow::on_lightManualOnOffSwitch_toggled  Event handler of
 *                                                                          turn on or off light
 *                                                                          in manual mode.
 * @param state State of manual on/off switch button.
 */
void BicycleFrontMonitorMainWindow::on_lightManualOnOffSwitch_toggled(bool state)
{
    QString titleToShow;
    int swithOnOff = 0;
    if (false == state) {
        swithOnOff = CBrakeItemModel::LIGHT_MANUAL_SWITCH_STATE_OFF;
        titleToShow = QString::fromLocal8Bit("消灯");
    } else {
        swithOnOff = CBrakeItemModel::LIGHT_MANUAL_SWITCH_STATE_ON;
        titleToShow = QString::fromLocal8Bit("点灯");
    }
    this->mBrakeItemModel->changeLightManOnOffState(swithOnOff);
    this->ui->lightManualOnOffSwitch->setText(titleToShow);
}

/**
 * @brief BicycleFrontMonitorMainWindow::initializeViews    Initialize views on main window
 *                                                          depending on its default states.
 */
void BicycleFrontMonitorMainWindow::initializeViews()
{
    this->on_lightManualOnOffSwitch_toggled(
                this->ui->lightManualOnOffSwitch->isChecked());
    this->on_lightAutoManualSwitch_toggled(
                this->ui->lightAutoManualSwitch->isChecked());
}
