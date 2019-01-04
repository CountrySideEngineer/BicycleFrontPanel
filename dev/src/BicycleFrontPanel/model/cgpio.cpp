#include <iostream>
#include <assert.h>
#include <list>
#include <vector>
#include <QObject>
#include <QtDebug>
#include "pigpio/pigpio.h"
#include "model/apart.h"
#include "model/cgpio.h"

/*
 * Timer macro used in time configuration.
 */
//Timer id.
#define TIMER_ID_CHATTERING_TIMER       (0)
#define TIMER_ID_PERIODIC_TIMER         (1)

//Time period in millisecond unit.
#define TIMER_PERIOD_CHATTERING_TIMER   (20)    //20 msec
#define TIMER_PERIOD_PERIODIC_TIMER     (10)    //10 msec

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
{
    this->mPinMap = new map<uint, APart*>();
    this->mCriticalSectionMap = new map<uint, bool>();
    this->mWaitChatteringList = new vector<CTimeDispatch*>();
    this->mTimeDispatchList = new vector<CTimeDispatch*>();
    this->mPeriodicTimeList = new vector<CTimeDispatch*>();
}
#define DELETE_MEMBER_POINTER(mPtr)     \
    {                                   \
        delete mPtr;                    \
        mPtr = nullptr;                 \
    }

#define DELETE_MEMBER_MAP(mVectorPtr)               \
{                                                   \
    auto VectorIt = mVectorPtr->begin();            \
    while (VectorIt != mVectorPtr->end()) {         \
        auto item = (*VectorIt).second;             \
        delete item;                                \
        VectorIt++;                                 \
    }                                               \
    DELETE_MEMBER_POINTER(mVectorPtr);              \
}

#define DELETE_MEMBER_VECTOR(mMapPtr)           \
    {                                           \
        auto MapIt = mMapPtr->begin();          \
        while (MapIt != mMapPtr->end()) {       \
            MapIt = mMapPtr->erase(MapIt);      \
        }                                       \
        DELETE_MEMBER_POINTER(mMapPtr);         \
    }

/**
 * @brief CGpio::~CGpio Destructor.
 */
CGpio::~CGpio()
{
    DELETE_MEMBER_MAP(this->mPinMap);
    DELETE_MEMBER_POINTER(this->mCriticalSectionMap);
    DELETE_MEMBER_VECTOR(this->mTimeDispatchList);
    DELETE_MEMBER_VECTOR(this->mWaitChatteringList);
}

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

            gpioSetTimerFunc(TIMER_ID_PERIODIC_TIMER, TIMER_PERIOD_PERIODIC_TIMER, CGpio::PeriodicTimerDispatch);
            gpioSetTimerFunc(TIMER_ID_CHATTERING_TIMER, TIMER_PERIOD_CHATTERING_TIMER, CGpio::ChatteringTimeDispatch);
        }
    }
}

/**
 * @brief CGpio::Finalize   Finalize, in other word terminte, the GPIO library
 *                          and delete CGpio instance.
 */
void CGpio::Finalize()
{
    gpioSetTimerFunc(TIMER_ID_PERIODIC_TIMER, TIMER_PERIOD_PERIODIC_TIMER, nullptr);
    gpioSetTimerFunc(TIMER_ID_CHATTERING_TIMER, TIMER_PERIOD_CHATTERING_TIMER, nullptr);
    gpioTerminate();

    if (nullptr != CGpio::mInstance) {
        delete CGpio::mInstance;
        CGpio::mInstance = nullptr;
    }
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
    CGpio* instance = CGpio::GetInstance();
    uint interruptPin = static_cast<uint>(pin);
    if (0 == instance->GetPinMap()->count(interruptPin)) {
        return;
    }

    if (false == instance->IsCriticalSection(interruptPin)) {
        instance->IntoCriticalSection(interruptPin);
        APart* part = instance->GetPinMap()->at(interruptPin);
        if (0 == part->GetChatteringTime()) {
            int state = gpioRead(part->GetGpioPin());
            part->InterruptCallback(state);
            instance->ExitCriticalSection(interruptPin);
        } else {
            instance->StartChatteringTimer(part);
        }
    }
}

/**
 * @brief CGpio::StartChatteringTimer   Start chattering timer.
 * @param part
 */
void CGpio::StartChatteringTimer(APart* part)
{
    assert(nullptr != part);

    this->mWaitChatteringList->push_back(new CTimeDispatch(part));
}

/**
 * @brief CGpio::ChatteringTimeDispatch Callback function called when a time
 *                                      set to wait for chattering is expired.
 */
void CGpio::ChatteringTimeDispatch()
{
    CGpio* instance = CGpio::GetInstance();
    vector<CTimeDispatch*>* waitChatteringList = instance->GetWaitChattering();
    auto it = waitChatteringList->begin();
    while (it != waitChatteringList->end()) {
        CTimeDispatch* timeDispatch = *it;
        if (timeDispatch->ExpiresTimer()) {
            APart* part = timeDispatch->GetParts();
            uint8_t gpioPin = part->GetGpioPin();
            int level = gpioRead(gpioPin);
            part->InterruptCallback(level);

            instance->ExitCriticalSection(gpioPin);

            it = instance->GetWaitChattering()->erase(it);
            /*
             * There is no need to delete parts, CParts object, because
             * the memory is released when the erase method is called at
             * the same time.
             */
        } else {
            it++;
        }
    }
}

/**
 * @brief CGpio::PeriodicTimerDispatch  Callback function called when timer dispatched.
 *                                      Attention, this functino is supposed called in
 *                                      periodically.
 */
void CGpio::PeriodicTimerDispatch()
{
    CGpio* instance = CGpio::GetInstance();
    vector<CTimeDispatch*>* periodicTImeList = instance->GetPeriodicTime();
    auto it = periodicTImeList->begin();
    while (it != periodicTImeList->end()) {
        CTimeDispatch* timeDispatch = *it;
        if (timeDispatch->ExpiresTimer()) {
            APart* part = timeDispatch->GetParts();
            uint8_t pin = part->GetGpioPin();
            int level = gpioRead(pin);
            part->TimerCallback(level);

            timeDispatch->UpdateBaseTime();
        }
        it++;
    }
}

/**
 * @brief CGpio::SetIsr Setup GPIO pin interrupt mode.
 * @param pin   Pin number Of GPIO to setup.
 * @param edge  Edge the callback function will be called,
 *              raise up or falling down.
 * @param part  Pointer to CParts object to call.
 */
void CGpio::SetIsr(uint pin, uint edge, APart* part)
{
    assert(nullptr != this->mPinMap);

    if (nullptr == part) {
        /*
         * @ToDo:
         *      Throw exception if the variable "part" is nullprt.
         */
        return;
    }

    /*
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
        if (0 != this->mPinMap->count(pin)) {
            cout << pin << " has already registered as ISR pin." << endl;
        } else {
            (*(this->mPinMap))[pin] = part;
            (*(this->mCriticalSectionMap))[pin] = false;
        }
    }
}

/**
 * @brief CGpio::SetTimeIsr Setup callback function called when a time is dispatched.
 * @param part  Part data to be called when the time is dispatched.
 */
void CGpio::SetTimeIsr(APart *part)
{
    assert(nullptr != this->mPeriodicTimeList);
    assert(NULL != part);

    /*
     *  The periodic timer has been started when the instance of CGpio class, singleton,
     *  was initialized.
     *  So, the function to regist callback, and so on, does not need to be called
     *  in this method.
     */
    this->mPeriodicTimeList->push_back(new CTimeDispatch(part));
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
    assert(nullptr != this->mCriticalSectionMap);

    try {
        (*(this->mCriticalSectionMap))[pin] = isIn;
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
    assert(nullptr != this->mCriticalSectionMap);
    try {
        return (*(this->mCriticalSectionMap))[pin];
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
    : mPart(nullptr)
    , mWaitTime(0)
{
    this->mBaseTime = QTime::currentTime();
}

/**
 * @brief CGpio::CTimeDispatch::CTimeDispatch   Destructor of CTimeDispatch class.
 * @param parts
 */
CGpio::CTimeDispatch::CTimeDispatch(APart* part)
    : mPart(part)
    , mWaitTime(part->GetChatteringTime())
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

    if (this->mWaitTime <= abs(currentTime.msecsTo(this->mBaseTime))) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief CGpio::CTimeDispatch::UpdateBaseTime  Update base time which will be used in
 *                                              calcurating the time is passed.
 */
void CGpio::CTimeDispatch::UpdateBaseTime()
{
    this->mBaseTime = QTime::currentTime();
}
