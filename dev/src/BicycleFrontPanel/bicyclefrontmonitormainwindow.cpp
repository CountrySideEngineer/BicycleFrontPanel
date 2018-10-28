#include <QtGui>
#include <QIcon>
#include <QPixmap>
#include <QPalette>
#include "bicyclefrontmonitormainwindow.h"
#include "ui_bicyclefrontmonitormainwindow.h"
#include "model/cgpio.h"

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
    , mTimer(new QTimer(this))
    , mDateTimerBuilder(new CDateTimeBuilder())
{
    ui->setupUi(this);

    this->setFixedSize(480, 320);

    this->mTimer->setInterval(100);
    this->mTimer->setSingleShot(false);

    connect(this->mTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(this->ui->menuButton, SIGNAL(clicked()), this, SLOT(onLightSw()));

    this->mTimer->start();

    this->updateDateTime();
    this->updateLightState();
    this->updateLightManualSw();

    this->mRearBrake = new CBrake(26, static_cast<QFrame*>(this->ui->rearBrakeState));
    this->mFrontBrake = new CBrake(19, static_cast<QFrame*>(this->ui->frontBrakeState));

    CGpio::Initialize();
    CGpio* instnace = CGpio::GetInstance();
    instnace->SetIsr(this->mFrontBrake->GetGpio(), 2, this->mFrontBrake);
    instnace->SetIsr(this->mRearBrake->GetGpio(), 2, this->mRearBrake);

    QFile styleSheetFile(":resources/qss/stylesheet.qss");
    if (!styleSheetFile.open(QFile::ReadOnly)) {
        qDebug() << "Can not open resources/qss/stylesheet.qss";
    } else {
        QString styleSheet = QString::fromLatin1(styleSheetFile.readAll());
        qApp->setStyleSheet(styleSheet);
        styleSheetFile.close();
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
    delete this->mRearBrake;
    delete this->mFrontBrake;

    CGpio::Finalize();
}

/**
 * @brief Timer dispatch event handler.
 */
void BicycleFrontMonitorMainWindow::onTimeout()
{
    this->updateDateTime();
    this->mRearBrake->UpdateView();
    this->mFrontBrake->UpdateView();
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
