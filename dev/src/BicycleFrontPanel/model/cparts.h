#ifndef CPARTS_H
#define CPARTS_H
#include <QObject>
#include <QFrame>

class CParts
{
public:
    enum PARTS_PIN_DIRECTION {
        PARTS_PIN_DIRECTION_INPUT = 0,
        PARTS_PIN_DIRECTION_OUTPUT,
        PARTS_PIN_DIRECTION_MAX
    };
public:
    CParts();
    CParts(uint pin, QFrame* delegateWidget);
    virtual ~CParts() {}

    virtual void UpdateView();
    virtual void Update();
    virtual void Callback(int state);
    virtual void Setup(uint8_t pin, PARTS_PIN_DIRECTION direction);

protected:
    uint mPin;
    QFrame* mDelegateWidget;
    QString mStyleSheet;

    bool mPinSet;
};

#endif // CPARTS_H
