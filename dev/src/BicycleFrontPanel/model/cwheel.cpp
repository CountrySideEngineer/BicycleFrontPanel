#include <iostream>
#include <QObject>
#include <QLabel>
#include <QtDebug>
#include <QDateTime>
#include <exception>
#include <typeinfo>
#include "model/cwheel.h"
using namespace std;

/**
 * @brief CWheel::CWheel    Default constructor.
 */
CWheel::CWheel()
    : CParts ()
    , mBaseTime(QDateTime::currentDateTime())
    , mRpm(0)
    , mRotateCount(0)
{
    this->SetupWatchDog();
}

/**
 * @brief CWheel::CWheel    Constructor with arguments.
 * @param pin               GPIO Pin number.
 * @param delegateWidget    Pointer to widget to show value the part has.
 */
CWheel::CWheel(uint pin, QFrame* delegateWidget)
    : CParts(pin, delegateWidget)
    , mBaseTime(QDateTime::currentDateTime())
    , mRpm(0)
    , mRotateCount(0)
    , mTmpCounter(0)
{
    CParts::Setup(this->mPin, PARTS_PIN_DIRECTION_INPUT);
    this->SetupWatchDog();
    this->StartWatchDogTimer();
}

/**
 * @brief CWheel::~CWheel   Destructor.
 */
CWheel::~CWheel() {}

/**
 * @brief CWheel::StartWatchDogTimer    Start watch dog timer.
 */
void CWheel::StartWatchDogTimer()
{
    this->mWatchDog.start();
}

/**
 * @brief CWheel::SetupWatchDog Configure "watch dog timer".
 */
void CWheel::SetupWatchDog()
{
    this->mWatchDog.setInterval(mInterval);  //1 sec.
    this->mWatchDog.setSingleShot(false);

    connect(&(this->mWatchDog), SIGNAL(timeout()), this, SLOT(onWatchDogTimeout()));

    this->StartWatchDogTimer();
}

/**
 * @brief CWheel::onWatchDogTimeout Watch dog timer callback.
 */
void CWheel::onWatchDogTimeout()
{
    QDateTime curTime = QDateTime::currentDateTime();
    uint32_t passedSec = static_cast<uint32_t>(curTime.secsTo(this->mBaseTime));
    if (mWatchDogTime < passedSec) {
        this->mRotateCount = 0;
        this->mRpm = 0;
        qDebug() << "mTmpCounter = " << mTmpCounter;
    }
}

/**
 * @brief CWheel::Callback  Callback function of interrupt.
 * @param state GPIO pin state.
 */
void CWheel::Callback(int state)
{
    this->Update(state);
}

/**
 * @brief CWheel::UpdateView    Update RPM value in GUI, if the target view
 *                              has been set while initializing.
 */
void CWheel::UpdateView()
{
    if (nullptr == this->mDelegateWidget) { return; }

    qDebug() << "CWheel::UpdateView()" << this->mRotateCount << "," << this->mRpm;

    try {
        QLabel* label = dynamic_cast<QLabel*>(this->mDelegateWidget);
        label->setText(QString::number(this->mRpm));
    } catch (std::bad_cast& ex) {
        cout << ex.what() << endl;
    }
}

void CWheel::Update() {}

/**
 * @brief CWheel::Update    Update rotation counter, and calcurate rotation count in
 *                          a minitute.
 */
void CWheel::Update(int /* state */)
{
    this->mTmpCounter++;
    this->mRotateCount++;

    QDateTime curTime = QDateTime::currentDateTime();
    uint32_t passedSec = static_cast<uint32_t>(curTime.secsTo(this->mBaseTime));
    if (1 < passedSec) {    //over 1 sec passed.
        this->mRpm = this->mRotateCount * 60;
        this->mBaseTime = curTime;
        this->mRotateCount = 0;
    }
}
