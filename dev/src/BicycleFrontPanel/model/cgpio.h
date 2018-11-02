#ifndef CGPIO_H
#define CGPIO_H
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <QObject>
#include <QTime>
#include "model/cparts.h"
using namespace std;

class CGpio
{

protected:
    /**
     * @brief The CTimeDispatch class   Inner class to manager timer dispatch.
     */
    class CTimeDispatch
    {
    public:
        CTimeDispatch();
        CTimeDispatch(CParts* parts);
        ~CTimeDispatch() {}

        bool ExpiresTimer();
        CParts* GetParts() const { return this->mParts; }

    protected:
        CParts* mParts;
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
    ~CGpio() {}

public:
    static void Initialize();
    static void Finalize();
    static CGpio* GetInstance();
    static void Interrupt(int pin, int level, uint32_t tick);
    static void TimerDispatch();

    void SetMode(uint pin, GPIO_PIN_DIRECTION mode);
    void SetIsr(uint pin, uint edge, CParts* part);
    void IntoCriticalSection() { this->mInCritical = true; }
    void ExitCriticalSection() { this->mInCritical = false; }
    void IntoCriticalSection(uint pin);
    void ExitCriticalSection(uint pin);
    bool IsCriticalSection(uint pin);

    bool GetInCritical() const { return this->mInCritical; }
    uint8_t GetInterruptPin() const { return this->mInterruptPin; }
    void SetInterruptPin(uint Pin) { this->mInterruptPin = (uint8_t)Pin; }
    map<uint, CParts*>* GetMap() { return &this->mPinMap; }
    //list<CTimeDispatch*>* GetTimeDispatch() { return &this->mTimeDispatchList; }
    vector<CTimeDispatch*>* GetTimeDispatch() { return &this->mTimeDispatchList; }

protected:
    void CriticalSection(uint pin, bool isIn);
protected:
    static CGpio* mInstance;

    bool mInCritical;
    uint8_t mInterruptPin;
    map<uint, CParts*> mPinMap;
    map<uint, bool> mCriticalSectionMap;
    //list<CTimeDispatch*> mTimeDispatchList;
    vector<CTimeDispatch*> mTimeDispatchList;
};


#endif // CGPIO_H
