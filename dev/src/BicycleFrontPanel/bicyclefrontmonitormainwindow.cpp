#include <QtGui>
#include <QIcon>
#include <QPixmap>
#include <QPalette>
#include "bicyclefrontmonitormainwindow.h"
#include "ui_bicyclefrontmonitormainwindow.h"

/**
 * @brief Constructor.
 * @param parent
 */
BicycleFrontMonitorMainWindow::BicycleFrontMonitorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BicycleFrontMonitorMainWindow),
    mIsLightOn(false),
    mIsLightSwManual(false),
    mIsHoldFrontBrake(false),
    mIsHoldRearBrake(false),
    mTimer(new QTimer(this)),
    mDateTimerBuilder(new CDateTimeBuilder()),
    mBrake(new CBrake())
{
    ui->setupUi(this);

    this->setFixedSize(480, 320);

    this->mTimer->setInterval(100);
    this->mTimer->setSingleShot(false);

    connect(this->mTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(this->ui->menuButton, SIGNAL(clicked()), this, SLOT(onLightSw()));
    connect(this->ui->tempButton, SIGNAL(clicked()), this, SLOT(onLightAutoManSw()));
    connect(this->ui->frontBrake, SIGNAL(pressed()), this, SLOT(onFrontBrakeHold()));
    connect(this->ui->frontBrake, SIGNAL(released()), this, SLOT(onFrontBrakeRelease()));
    connect(this->ui->rearBrake, SIGNAL(pressed()), this, SLOT(onRearBrakeHold()));
    connect(this->ui->rearBrake, SIGNAL(released()), this, SLOT(onRearBrakeRelease()));

    this->mTimer->start();

    this->updateDateTime();
    this->updateLightState();
    this->updateLightManualSw();

    this->mBrake->SetDelegate(static_cast<QFrame*>(this->ui->rearBrakeState));
    this->mBrake->SetGpio(17);
    this->mBrake->Initialize();

    QFile styleSheetFile(":resources/qss/stylesheet.qss");
    if (!styleSheetFile.open(QFile::ReadOnly)) {
        qDebug() << "Can not open resources/qss/stylesheet.qss";
    } else {
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheet = QString::fromLatin1(styleSheetFile.readAll());
        qApp->setStyleSheet(styleSheet);
    }
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
    delete this->mBrake;
}

/**
 * @brief Timer dispatch event handler.
 */
void BicycleFrontMonitorMainWindow::onTimeout()
{
    this->updateDateTime();
    this->mBrake->Update();
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
 * @brief Change light state view.
 */
void BicycleFrontMonitorMainWindow::updateLightState()
{
    QPixmap lightImage;
    if (this->mIsLightOn) {
        lightImage.load(QString(":/resources/images/light_on.png"));
    } else {
        lightImage.load(QString(":/resources/images/light_off.png"));
    }
    this->ui->lightState->setPixmap(lightImage);
}

/**
 * @brief Update light sw state, manual or automation.
 */
void BicycleFrontMonitorMainWindow::updateLightManualSw()
{
    QPixmap autoImage;
    QPixmap manualImage;

    if (this->mIsLightSwManual) {
        manualImage.load(QString(":/resources/images/manual.png"));
        autoImage.load(QString(""));
    } else {
        manualImage.load(QString(""));
        autoImage.load(QString(":/resources/images/auto.png"));
    }
    this->ui->lightSwAuto->setPixmap(autoImage);
    this->ui->lightSwManual->setPixmap(manualImage);
}

void BicycleFrontMonitorMainWindow::updateFrontBrakeState()
{
    if (this->mIsHoldFrontBrake) {
        this->ui->frontBrakeState->setStyleSheet(tr("background-color:red"));
    } else {
        this->ui->frontBrakeState->setStyleSheet(tr(""));
    }
}

void BicycleFrontMonitorMainWindow::updateRearBrakeState()
{
    this->mBrake->SetIsHold(!(this->mBrake->GetIsHold()));
    this->mBrake->Update();
#if 0
    if (this->mIsHoldRearBrake) {
        this->ui->rearBrakeState->setStyleSheet(tr("background-color:red"));
    } else {
        this->ui->rearBrakeState->setStyleSheet(tr(""));
    }
#endif
}

//Temporary button event handler.
void BicycleFrontMonitorMainWindow::onLightSw()
{
    this->mIsLightOn = !this->mIsLightOn;
    this->updateLightState();
}

void BicycleFrontMonitorMainWindow::onLightAutoManSw()
{
    this->mIsLightSwManual = !(this->mIsLightSwManual);
    this->updateLightManualSw();
}

void BicycleFrontMonitorMainWindow::onFrontBrakeHold()
{
    this->mIsHoldFrontBrake = true;
    this->updateFrontBrakeState();
}

void BicycleFrontMonitorMainWindow::onFrontBrakeRelease()
{
    this->mIsHoldFrontBrake = false;
    this->updateFrontBrakeState();
}

void BicycleFrontMonitorMainWindow::onRearBrakeHold()
{
    this->mIsHoldRearBrake = true;
    this->updateRearBrakeState();
}

void BicycleFrontMonitorMainWindow::onRearBrakeRelease()
{
    this->mIsHoldRearBrake = false;
    this->updateRearBrakeState();
}
