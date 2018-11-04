#ifndef CWHEEL_H
#define CWHEEL_H
#include <QObject>
#include <QFrame>
#include <QDateTime>
#include <QTimer>
#include "model/cparts.h"

class CWheel : public CParts
{
    Q_OBJECT

public:
    explicit CWheel();
    explicit CWheel(uint Pin, QFrame* delegateWidget);
    ~CWheel();

    virtual void Callback(int state);

public:
    virtual void UpdateView();
    virtual void Update();
    virtual void Update(int state);

    QTimer* GetWatchDog() { return static_cast<QTimer*>(&this->mWatchDog); }

protected:
    void SetupWatchDog();
    void StartWatchDogTimer();

public slots:
    void onWatchDogTimeout();

protected:
    QTimer mWatchDog;
    QDateTime mBaseTime;
    uint32_t mRpm;
    uint32_t mRotateCount;
    uint64_t mTmpCounter;

    const uint16_t mInterval = 5000;    //Watch dog timer interval.
    const uint16_t mWatchDogTime = mInterval / 1000;    //5 sec
};

#endif // CWHEEL_H
