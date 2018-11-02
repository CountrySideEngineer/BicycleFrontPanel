#include <iostream>
#include <list>
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
    , mInterruptPin(0xFF)
{}

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

            gpioSetTimerFunc(0, 10, TimerDispatch);
        }
    }
}

/**
 * @brief CGpio::Finalize   Finalize, in other word terminte, the GPIO library
 *                          and delete CGpio instance.
 */
void CGpio::Finalize()
{
    gpioSetTimerFunc(0, 10, NULL);
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

/**
 * @brief CGpio::Interrupt  Interrupt callback function.
 * @param pin   Pin of GPIO.
 */
void CGpio::Interrupt(int pin, int /* level */, uint32_t /* tick */)
{
    qDebug() << "Interrupt" << pin;
    CGpio* instance = CGpio::GetInstance();
    uint interruptPin = static_cast<uint>(pin);
    if (0 != instance->GetMap()->count(interruptPin)) {
        qDebug() << "In critical section = " << instance->IsCriticalSection(interruptPin);
        if (false == instance->IsCriticalSection(interruptPin)) {
            instance->IntoCriticalSection(interruptPin);
            CParts* parts = instance->GetMap()->at(pin);
            CTimeDispatch* timeDispatch = new CTimeDispatch(parts);
            instance->GetTimeDispatch()->push_back(timeDispatch);
        }
    }
}

/**
 * @brief CGpio::TimerDispatch  Callback function called when timet dispatched.
 */
void CGpio::TimerDispatch()
{
    CGpio* instance = CGpio::GetInstance();
    list<CTimeDispatch*>* timeDispatchList = instance->GetTimeDispatch();
    auto it = timeDispatchList->begin();
    while (it != timeDispatchList->end()) {
        CTimeDispatch* timeDispatch = *it;
        if (timeDispatch->ExpiresTimer()) {
            CParts* parts = timeDispatch->GetParts();
            int level = gpioRead(parts->GetGpio());
            parts->Callback(level);

            uint pin = parts->GetGpio();
            instance->ExitCriticalSection(pin);

            it = instance->GetTimeDispatch()->erase(it);
            /*
             * There is no need to delete parts, CParts object, because
             * the memory is released when the erase method is called at
             * the same time.
             */
        } else {
            timeDispatchList->push_back(*it);
            it++;
        }
    }
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
            this->mCriticalSectionMap[pin] = false;
            gpioSetISRFunc(pin, edge, 0, CGpio::Interrupt);
        }
    }
}

/**
 * @brief CGpio::IntoCriticalSection    Set a pin into critical section.
 * @param pin   GPIO pin number.
 */
void CGpio::IntoCriticalSection(uint pin) { this->CriticalSection(pin, true); }

/**
 * @brief CGpio::ExitCriticalSection    Exit a pin from critical section.
 * @param pin   GPIO pin number.
 */
void CGpio::ExitCriticalSection(uint pin) { this->CriticalSection(pin, false); }

/**
 * @brief CGpio::CriticalSection    Main method to manage critical section.
 * @param pin   GPIO pin number.
 * @param isIn  Critical section state, true means "being in critical section",
 *              otherwise not in critical section.
 */
void CGpio::CriticalSection(uint pin, bool isIn)
{
    try {
        this->mCriticalSectionMap[pin] = isIn;
    } catch (out_of_range& ex) {
        cout << ex.what() << endl;
    }
}

/**
 * @brief CGpio::IsCriticalSection  Returns critical section state of GPIO pin
 * @param pin   GPIO pin number.
 * @return Returns TRUE if in critical section, otherwise false.
 */
bool CGpio::IsCriticalSection(uint pin)
{
    try {
        return this->mCriticalSectionMap[pin];
    } catch (out_of_range& ex) {
        cout << ex.what() << endl;

        return false;
    }
}

/**
 * @brief CGpio::CTimeDispatch::CTimeDispatch   Constructor of CTimeDispatch, inner class,
 *                                              default constructor.
 */
CGpio::CTimeDispatch::CTimeDispatch()
    : mParts(nullptr)
    , mWaitTime(0)
{
    this->mBaseTime = QTime::currentTime();
}

/**
 * @brief CGpio::CTimeDispatch::CTimeDispatch   Destructor of CTimeDispatch class.
 * @param parts
 */
CGpio::CTimeDispatch::CTimeDispatch(CParts* parts)
    : mParts(parts)
    , mWaitTime(parts->GetChatteringTime())
{
    this->mBaseTime = QTime::currentTime();
}

/**
 * @brief CGpio::CTimeDispatch::ExpiresTimer    Returns whether the time has been expired or not.
 * @return  Returns true if the time has been expired, otherwise returns false.
 */
bool CGpio::CTimeDispatch::ExpiresTimer()
{
    QTime currentTime = QTime::currentTime();

    if (this->mWaitTime < abs(currentTime.msecsTo(this->mBaseTime))) {
        return true;
    } else {
        return false;
    }
}
