#ifndef CWHEELVELOCITY_H
#define CWHEELVELOCITY_H
#include <QObject>
#include "model/cwheel.h"

class CWheelVelocity : public CWheel
{
    Q_OBJECT

public:
    CWheelVelocity();
    CWheelVelocity(uint Pin, QFrame* delegateWidget);
    ~CWheelVelocity();

public:
    virtual void UpdateView();
    virtual void Update();

protected:
    uint64_t mVelocity; //LSB = 0.1 [m]
};

#endif // CWHEELVELOCITY_H
