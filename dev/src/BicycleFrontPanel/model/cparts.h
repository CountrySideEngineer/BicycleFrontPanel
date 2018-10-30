#ifndef CPARTS_H
#define CPARTS_H
#include <QObject>
#include <QFrame>

class CParts : public QObject
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
    virtual void Update(int level);
    virtual void Callback(int state);
    virtual void Setup(uint pin, PARTS_PIN_DIRECTION direction);

    //Getter and setter.
    virtual void SetDelegateWidget(QFrame* delegateWidget) {
        this->mDelegateWidget = delegateWidget;
    }
    virtual QFrame* GetDelegateWidget() const { return this->mDelegateWidget; }
    void SetGpio(uint Pin) { this->mPin = Pin; }
    uint GetGpio() const { return this->mPin; }
    void SetPinLevel(int level) { this->mPinLevel = level; }
    int GetPinLevel() { return this->mPinLevel; }

protected:
    uint mPin;
    QFrame* mDelegateWidget;
    QString mStyleSheet;

    bool mPinSet;
    int mPinLevel;
};

#endif // CPARTS_H
