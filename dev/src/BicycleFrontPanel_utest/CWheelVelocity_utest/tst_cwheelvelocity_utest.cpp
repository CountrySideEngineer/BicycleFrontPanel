#include <QtTest>
#include <QtDebug>

// add necessary includes here
#include "cwheelvelocity.h"

class CWheelVelocity_utest : public QObject
{
    Q_OBJECT

public:
    CWheelVelocity_utest();
    ~CWheelVelocity_utest();

protected:
    CWheelVelocity* WheelVelocity;

private slots:
    void initTestCase();
    void test_case001();
    void test_case002();
    void cleanupTestCase();

};

CWheelVelocity_utest::CWheelVelocity_utest()
{
    this->WheelVelocity = NULL;
}
CWheelVelocity_utest::~CWheelVelocity_utest() {}
void CWheelVelocity_utest::initTestCase() {}
void CWheelVelocity_utest::cleanupTestCase()
{
    delete this->WheelVelocity;
    this->WheelVelocity = NULL;
}
void CWheelVelocity_utest::test_case001()
{
    this->initTestCase();
    this->WheelVelocity = new CWheelVelocity();

    for (int index = 0; index < 83; index++) {
        this->WheelVelocity->InterruptCallback(0);
    }
    this->WheelVelocity->TimerCallback(0);

    QVERIFY(0 == this->WheelVelocity->GetVelocity());

    this->cleanupTestCase();
}
void CWheelVelocity_utest::test_case002()
{
    this->initTestCase();
    this->WheelVelocity = new CWheelVelocity();

    for (int index = 0; index < 84; index++) {
        this->WheelVelocity->InterruptCallback(0);
    }
    this->WheelVelocity->TimerCallback(0);

    QVERIFY(1243 == this->WheelVelocity->GetVelocity());

    this->cleanupTestCase();
}

QTEST_APPLESS_MAIN(CWheelVelocity_utest)

#include "tst_cwheelvelocity_utest.moc"
