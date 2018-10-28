#include <iostream>
#include <QObject>
#include <QtDebug>
#include "cgpio.h"
#include "pigpio/pigpio.h"
using namespace std;

CGpio* CGpio::mInstance = nullptr;

/**
 * @brief CGpio::CGpio  Constructor of CGpio class.
 *                      In this method, member variable mIsCritical and mInterruptPin
 *                      are initialized.
 */
CGpio::CGpio()
    : mInCritical(false)
    , mInterruptPin(0xFF) {}

/**
 * @brief CGpio::Initialize Initialize GPIO library, and create instance of
 *                          CGpio class if the library initializing finished
 *                          successfully.
 */
void CGpio::Initialize()
{
    if (gpioInitialise() < 0) {
        //GPIO library initialize failed.
        cout << "gpioInitialise() failed" << endl;
    } else {
        if (nullptr == CGpio::mInstance) {
            CGpio::mInstance = new CGpio();
        }
    }
}

/**
 * @brief CGpio::Finalize   Finalize, in other word terminte, the GPIO library
 *                          and delete CGpio instance.
 */
void CGpio::Finalize()
{
    gpioTerminate();

    delete CGpio::mInstance;
}

/**
 * @brief CGpio::GetIntance Returns instande of pointer to CGpio instance.
 * @return
 */
CGpio* CGpio::GetInstance()
{
    if (nullptr == CGpio::mInstance) {
        CGpio::Initialize();
    }
    return CGpio::mInstance;
}

/**
 * @brief CGpio::SetMode    Set mode of Pin, INPUT or OUTPUT.
 * @param pin   Pin number of GPIO pin.
 * @param mode  Pin mode, INPUT or OUTPUT.
 */
void CGpio::SetMode(uint pin, GPIO_PIN_DIRECTION mode)
{
    uint directoin = 0;
    if (GPIO_PIN_DIRECTION_INPUT == mode) {
        directoin = PI_INPUT;
    } else if (GPIO_PIN_DIRECTION_OUTPUT == mode) {
        directoin = PI_OUTPUT;
    } else {
        directoin = 0xFF;
    }
    /*
     * Variable "direction" is not checked because, if it is invalid,
     * the library, "gpioSetMode", returns invalid result, maybe PI_BAD_MODE.
     */

    /**
     * @ToDo    Throw exception if the GPIO library returned
     *          not OK value.
     */
    int result = gpioSetMode(pin, directoin);
    if (PI_BAD_GPIO == result) {
        cout << "gpioSetMode() failed because of PI_BAD_GPIO" << endl;
    } else if (PI_BAD_MODE == result) {
        cout << "gpioSetMode() failed because of PI_BAD_MODE" << endl;
    }
}

void CGpio::Interrupt(int pin, int level, uint32_t tick)
{
    qDebug() << "Interrupt occurred" << tick << "," << pin << "," << level;

    CGpio* instance = CGpio::GetInstance();
    uint interruptPin = static_cast<uint>(pin);
    if (0 != instance->GetMap()->count(interruptPin)) {
        if (false == instance->GetInCritical()) {
            instance->SetInterruptPin(interruptPin);
            instance->IntoCriticalSection();
            gpioSetTimerFunc(0, 20, CGpio::TimerDispatch);
        }
    }
}

void CGpio::TimerDispatch()
{
    CGpio* instance = CGpio::GetInstance();
    uint pin = instance->GetInterruptPin();
    int level  = gpioRead(pin);
    if (PI_BAD_GPIO != level) {
        try {
            qDebug() << "Timer dispatch," << pin << "," << level;

            CParts* Parts = instance->GetMap()->at(pin);
            Parts->Callback(level);
        } catch (out_of_range& ex) {
            cout << ex.what() << endl;
        }
    }
    instance->SetInterruptPin(0xFF);
    instance->ExitCriticalSection();
    gpioSetTimerFunc(0, 20, NULL);
}

/**
 * @brief CGpio::SetIsr Setup GPIO pin interrupt mode.
 * @param pin   Pin number Of GPIO to setup.
 * @param edge  Edge the callback function will be called,
 *              raise up or falling down.
 * @param part  Pointer to CParts object to call.
 */
void CGpio::SetIsr(uint pin, uint edge, CParts* part)
{
    /**
     * @ToDo    Throw exception if the GPIO library returned
     *          not OK value.
     */
    int result = gpioSetISRFunc(pin, edge, 0, CGpio::Interrupt);
    if (PI_BAD_GPIO == result) {
        cout << "gpioSetISRFunc() failed because PI_BAD_GPIO" << endl;
    } else if (PI_BAD_EDGE == result) {
        cout << "gpioSetISRFunc() failed because PI_BAD_EDGE" << endl;
    } else if (PI_BAD_ISR_INIT == result) {
        cout << "gpioSetISRFunc() failed because PI_BAD_ISR_INIT" << endl;
    } else {
        if (this->mPinMap.count(pin)) {
            cout << pin << " has already registered as ISR pin." << endl;
        } else {
            this->mPinMap[pin] = part;
            gpioSetISRFunc(pin, edge, 20, CGpio::Interrupt);
        }
    }
}
