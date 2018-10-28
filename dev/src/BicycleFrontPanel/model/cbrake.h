#ifndef CBRAKE_H
#define CBRAKE_H
#include <QObject>
#include <QFrame>
#include "model/cparts.h"

class CBrake : public CParts
{
public:
    CBrake();
    CBrake(uint Pin, QFrame* delegateWidget);
    ~CBrake();

    void SetIsHold(bool isHold) { this->mIsHold = isHold; }
    bool GetIsHold() const { return this->mIsHold; }
    virtual void Callback(int state);

public:
    void UpdateView();
    void Update();
    void Update(int Level);

protected:
    bool mIsHold;
};

#endif // CBRAKE_H
