#ifndef CBRAKE_H
#define CBRAKE_H
#include <QObject>
#include <QFrame>

class CBrake
{
public:
    CBrake();
    CBrake(uint8_t Pin, QFrame* delegate);
    ~CBrake();

    void SetDelegate(QFrame* delegate) { this->mDelegate = delegate; }
    QFrame* getDelegate() const { return this->mDelegate; }
    void SetIsHold(bool isHold) { this->mIsHold = isHold; }
    bool GetIsHold() const { return this->mIsHold; }
    void SetGpio(uint8_t Pin) { this->mPin = Pin; }
    uint8_t GetGpio() const { return this->mPin; }

public:
    void UpdateView();
    void Update();
    void Initialize();

protected:
    QFrame* mDelegate;

    bool mIsHold;
    uint8_t mPin;
};

#endif // CBRAKE_H
