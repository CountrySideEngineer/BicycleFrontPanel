#include <QtTest>
#include <QDebug>

// add necessary includes here
#include "cdatetime.h"
#include "apart.h"
#include "cwheel.h"
#include "cwheelvelocity.h"

class BicycleFrontPanel_utest : public QObject
{
    Q_OBJECT

public:
    BicycleFrontPanel_utest();
    ~BicycleFrontPanel_utest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void CWheelTest_001();
    void CWheelTest_002();
    void CWheelTest_003();
    void CWheelTest_004();
    void CWheelTest_005();
    void CWheelTest_006();
    void CWheelTest_007();
    void CWheelTest_008();
    void CWheelTest_009();

private:
    CWheel *wheel;
};

BicycleFrontPanel_utest::BicycleFrontPanel_utest()
{
    this->wheel = NULL;
}

BicycleFrontPanel_utest::~BicycleFrontPanel_utest()
{
    delete this->wheel;

}
void BicycleFrontPanel_utest::initTestCase() {}
void BicycleFrontPanel_utest::cleanupTestCase() {}
void BicycleFrontPanel_utest::CWheelTest_001()
{
    this->wheel = new CWheel();

    QVERIFY(0 == this->wheel->GetGpioPin());
    QVERIFY(false == this->wheel->GetIsFailure());
    QVERIFY(0x0000 == this->wheel->GetFailureCode());
    QVERIFY(0 == this->wheel->GetState());
    QVERIFY(APart::PART_PIN_DIRECTION_MAX == this->wheel->GetPinDirection());
}
void BicycleFrontPanel_utest::CWheelTest_002()
{
    this->wheel = new CWheel(1, APart::PART_PIN_DIRECTION_INPUT, 10, 20);
    QVERIFY(1 == this->wheel->GetGpioPin());
    QVERIFY(false == this->wheel->GetIsFailure());
    QVERIFY(0x000 == this->wheel->GetFailureCode());
    QVERIFY(0 == this->wheel->GetState());
    QVERIFY(APart::PART_PIN_DIRECTION_INPUT == this->wheel->GetPinDirection());
}
void BicycleFrontPanel_utest::CWheelTest_003()
{
    this->wheel = new CWheel(1, APart::PART_PIN_DIRECTION_OUTPUT, 10, 20);
    QVERIFY(APart::PART_PIN_DIRECTION_OUTPUT == this->wheel->GetPinDirection());
}
void BicycleFrontPanel_utest::CWheelTest_004()
{
    this->wheel = new CWheel(1, APart::PART_PIN_DIRECTION_IN_OUT, 10, 20);
    QVERIFY(APart::PART_PIN_DIRECTION_IN_OUT == this->wheel->GetPinDirection());
}
void BicycleFrontPanel_utest::CWheelTest_005()
{
    this->wheel = new CWheel();

    QVERIFY(0 == this->wheel->GetState());

    this->wheel->Update();

    QVERIFY(1 == this->wheel->GetState());
}
void BicycleFrontPanel_utest::CWheelTest_006()
{
    this->wheel = new CWheel();

    QVERIFY(0 == this->wheel->GetState());

    this->wheel->Update(100);

    QVERIFY(0 == this->wheel->GetState());
}
void BicycleFrontPanel_utest::CWheelTest_007()
{
    this->wheel = new CWheel();

    QVERIFY(0 == this->wheel->GetState());

    this->wheel->InterruptCallback(0);

    QVERIFY(1 == this->wheel->GetState());
}
void BicycleFrontPanel_utest::CWheelTest_008()
{
    this->wheel = new CWheel();

    for (int index = 0; index < 83; index++) {
        this->wheel->InterruptCallback(0);
    }
    QVERIFY(83 == this->wheel->GetState());
    QVERIFY(0 == this->wheel->GetRpm());

    this->wheel->TimerCallback(0);
    QVERIFY(0 == this->wheel->GetState());
    QVERIFY(0 == this->wheel->GetRpm());
}
void BicycleFrontPanel_utest::CWheelTest_009()
{
    this->wheel = new CWheel();

    for (int index = 0; index < 84; index++) {
        this->wheel->InterruptCallback(0);
    }
    QVERIFY(84 == this->wheel->GetState());
    QVERIFY(0 == this->wheel->GetRpm());
    this->wheel->TimerCallback(0);
    QVERIFY(0 == this->wheel->GetState());
    QVERIFY(1 == this->wheel->GetRpm());
}
QTEST_APPLESS_MAIN(BicycleFrontPanel_utest)

#include "tst_bicyclefrontpanel_utest.moc"
