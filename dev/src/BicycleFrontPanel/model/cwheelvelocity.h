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
    uint32_t mVelocity; //LSB = 1.0 * 10^(-3) [km/h]
};

#endif // CWHEELVELOCITY_H
