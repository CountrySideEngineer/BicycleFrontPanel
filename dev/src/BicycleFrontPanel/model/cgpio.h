#ifndef CGPIO_H
#define CGPIO_H
#include <iostream>
#include <map>
#include <QObject>
#include "model/cparts.h"
using namespace std;

class CGpio
{
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
    bool GetInCritical() const { return this->mInCritical; }
    uint8_t GetInterruptPin() const { return this->mInterruptPin; }
    void SetInterruptPin(uint Pin) { this->mInterruptPin = (uint8_t)Pin; }
    map<uint, CParts*>* GetMap() { return &this->mPinMap; }

protected:
    static CGpio* mInstance;

    bool mInCritical;
    uint8_t mInterruptPin;
    map<uint, CParts*> mPinMap;

};

#endif // CGPIO_H
