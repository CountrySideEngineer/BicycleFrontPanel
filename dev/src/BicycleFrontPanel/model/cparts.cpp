#include "cparts.h"
#include "pigpio/pigpio.h"

/**
 * @brief CParts::CParts    Constructor of CParts class.
 */
CParts::CParts()
    : mPin(0)
    , mDelegateWidget(nullptr)
    , mPinSet(false)
{}

/**
 * @brief CParts::CParts    Constructor of CParts class setting parameters, pin and qframe.
 * @param pin   Pin number of GPIO.
 * @param delegateWidget    Pointer to widget of user interface as delegate.
 */
CParts::CParts(uint pin, QFrame* delegateWidget)
    : mPin(pin)
    , mDelegateWidget(delegateWidget)
    , mPinSet(true)
{}

/**
 * @brief CParts::UpdateView    Update view.
 */
void CParts::UpdateView()
{
    if (nullptr == this->mDelegateWidget) {
        /*
         * Nothing to do.
         * Some time, in the situation, exception thrown is better.
         */
        return;
    }
    this->mDelegateWidget->setStyleSheet(this->mStyleSheet);
}

/**
 * @brief CParts::Update    Update device state. In this class, nothing to do.
 */
void CParts::Update() {}

/**
 * @brief CParts::Callback  Callback function.
 */
void CParts::Callback(int /* state */) {}

/**
 * @brief CParts::Setup Setup pin mode, INPUT or OUTPUT.
 * @param pin           Pin number to setup.
 * @param direction     Pin access direction, INPUT or OUTPUT.
 */
void CParts::Setup(uint8_t pin, PARTS_PIN_DIRECTION direction)
{
    if (!this->mPinSet) {
        this->mPin = pin;
        this->mPinSet = true;
    } else {
        if (pin != this->mPin) {
            /*
             * The GPIO pin to be configured in this method must match with
             * that of insntace has.
             * So, if it does not match, this function will be exited.
             * In the future version, this function should be updated to throw
             * exception, invalid argument.
             */
            return;
        }
    }
    if ((PARTS_PIN_DIRECTION_INPUT != direction)
     && (PARTS_PIN_DIRECTION_OUTPUT != direction))
    {
        /*
         * In the case that the direction is invalid, this function should throw exceptoin.
         * Change this method to do so will be done in the future version.
         */
        return;
    }
    uint pinDirection = 0;
    switch (static_cast<int>(direction)) {
    case PARTS_PIN_DIRECTION_INPUT:
        pinDirection = PI_INPUT;
        break;

    case PARTS_PIN_DIRECTION_OUTPUT:
        pinDirection = PI_OUTPUT;
        break;

    default:
        //This code in unreachable.
        break;
    }

    gpioSetMode(this->mPin, pinDirection);
}
