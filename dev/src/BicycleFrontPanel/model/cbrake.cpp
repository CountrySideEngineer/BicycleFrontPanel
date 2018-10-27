#include <QObject>
#include <QLabel>
#include "cbrake.h"
#include "pigpio/pigpio.h"
#include "QtDebug"

/**
 * @brief CBrake::CBrake    Default constructor.
 */
CBrake::CBrake()
    : CParts ()
    , mIsHold(false)
{}
CBrake::CBrake(uint Pin, QFrame* delegateWidget)
    : CParts(Pin, delegateWidget)
    , mIsHold(false)
{
    CParts::Setup(this->mPin, PARTS_PIN_DIRECTION_INPUT);
}

/**
 * @brief CBrake::~CBrake   Destructor.
 */
CBrake::~CBrake()
{}

/**
 * @brief CBrake::Callback  Callback function called when the pin state,
 *                          HIGH or LOW has been changed.
 */
void CBrake::Callback(int state)
{
    this->Update(state);
}

/**
 * @brief CBrake::UpdateView    Update view showing state.
 */
void CBrake::UpdateView()
{
    if (nullptr == this->mDelegateWidget) { return; }

    QString styleSheet = QString("");
    if (this->mIsHold) {
        styleSheet = QString("background-color:red");
    }
    qDebug() << "CBrake::UpdateView() - StyleSheet = " << styleSheet;

    this->mDelegateWidget->setStyleSheet(styleSheet);
}

/**
 * @brief CBrake::Update    Update "state" value.
 */
void CBrake::Update()
{
    int ReadData = gpioRead(this->mPin);
    if (PI_HIGH == ReadData) {
        this->mIsHold = true;
    } else {
        this->mIsHold = false;
    }
}

/**
 * @brief CBrake::Update    Update "state" value from argument "Level".
 *                          "Level = 1" means pin level is high, "Level = 0"
 *                          means low.
 * @param Level     Pin level, 1 means high , 0 means low, others are unknown.
 */
void CBrake::Update(int Level)
{
    qDebug() << "CBrake::Update : Level = " << Level;
    if (1 == Level) {
        this->mIsHold = true;
    } else if (0 == Level) {
        this->mIsHold = false;
    } else {
        //Error:Nothing to do!
        /*
         * @ToDo:Change to throw exception, meaning "argument is invalid."
         */
    }
}
