#ifndef CGPIO_H
#define CGPIO_H
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <QObject>
#include <QTime>
#include "model/apart.h"
using namespace std;

class CGpio
{

public:
    /**
     * @brief The CTimeDispatch class   Inner class to manager timer dispatch.
     */
    class CTimeDispatch
    {
    public:
        CTimeDispatch();
        CTimeDispatch(APart* part);
        ~CTimeDispatch() {}

        bool ExpiresTimer();
        APart* GetParts() const { return this->mPart; }

    protected:
        APart* mPart;
        QTime mBaseTime;
        int mWaitTime; //unit : millisec
    };

public:
    enum GPIO_PIN_DIRECTION {
        GPIO_PIN_DIRECTION_INPUT = 0,
        GPIO_PIN_DIRECTION_OUTPUT,
        GPIO_PIN_DIRECTION_MAX
    };
protected:
    CGpio();
    ~CGpio();

public:
    static void Initialize();
    static void Finalize();
    static CGpio* GetInstance();
    static void Interrupt(int pin, int level, uint32_t tick);
    static void TimerDispatch();
    static void ChatteringTimeDispatch();

    void SetMode(uint pin, GPIO_PIN_DIRECTION mode);
    void SetIsr(uint pin, uint edge, APart* part);
    void IntoCriticalSection() { this->mInCritical = true; }
    void ExitCriticalSection() { this->mInCritical = false; }
    void IntoCriticalSection(uint pin);
    void ExitCriticalSection(uint pin);
    bool IsCriticalSection(uint pin);
    void StartChatteringTimer(APart* part);

    bool GetInCritical() const { return this->mInCritical; }
    uint8_t GetInterruptPin() const { return this->mInterruptPin; }
    void SetInterruptPin(uint Pin) { this->mInterruptPin = static_cast<uint8_t>(Pin); }
    map<uint, APart*>* GetPinMap() { return this->mPinMap; }
    vector<CTimeDispatch*>* GetTimeDispatch() { return this->mTimeDispatchList; }
    vector<CTimeDispatch*>* GetWaitChattering() { return this->mWaitChatteringList; }

protected:
    void CriticalSection(uint pin, bool isIn);

protected:
    static CGpio* mInstance;

    bool mInCritical;
    uint8_t mInterruptPin;
    map<uint, APart*>* mPinMap;
    map<uint, bool>* mCriticalSectionMap;
    vector<CTimeDispatch*>* mTimeDispatchList;
    vector<CTimeDispatch*>* mWaitChatteringList;
};


#endif // CGPIO_H
