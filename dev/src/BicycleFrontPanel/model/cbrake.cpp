#include <QObject>
#include <QLabel>
#include "cbrake.h"
#include "pigpio/pigpio.h"
#include "QtDebug"

CBrake::CBrake()
    : mDelegate(nullptr)
    , mIsHold(false)
{
    gpioInitialise();
}
CBrake::CBrake(uint8_t Pin, QFrame* delegate)
    : mDelegate(delegate)
    , mIsHold(false)
    , mPin(Pin)
{
    gpioInitialise();
}

CBrake::~CBrake()
{
    gpioTerminate();
}

/**
 * @brief CBrake::UpdateView    Update view showing state.
 */
void CBrake::UpdateView()
{
    if (nullptr == this->mDelegate) { return; }

    QLabel* destWidget = static_cast<QLabel*>(this->mDelegate);
    QString styleSheet = QString("");
    if (this->mIsHold) {
        styleSheet = QString("background-color:red");
    }
    destWidget->setStyleSheet(styleSheet);
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
    this->UpdateView();
}

void CBrake::Initialize()
{
    gpioSetMode(this->mPin, PI_INPUT);
}
