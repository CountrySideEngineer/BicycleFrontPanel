#include <QtTest>

// add necessary includes here
#include "model/apart.h"
#include "model/cbrake.h"

class CBrake_utest : public QObject
{
    Q_OBJECT

public:
    CBrake_utest();
    ~CBrake_utest();

protected:
    CBrake* brake;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case_001();
    void test_case_002();
    void test_case_003();
    void test_case_004();
    void test_case_005();
    void test_case_006();
    void test_case_007();
    void test_case_008();
    void test_case_009();
    void test_case_010();
    void test_case_011();
    void test_case_012();
};

CBrake_utest::CBrake_utest() {}
CBrake_utest::~CBrake_utest() {}
void CBrake_utest::initTestCase() {}
void CBrake_utest::cleanupTestCase() {}
void CBrake_utest::test_case_001()
{
    this->brake = new CBrake();

    QVERIFY(0 == this->brake->GetGpioPin());
    QVERIFY(APart::PART_PIN_DIRECTION_MAX == this->brake->GetPinDirection());
    QVERIFY(0 == this->brake->GetChatteringTime());
    QVERIFY(0 == this->brake->GetPeriodTime());
    QVERIFY(false == this->brake->GetIsHold());

    delete this->brake;
}
void CBrake_utest::test_case_002()
{
    this->brake = new CBrake(1, APart::PART_PIN_DIRECTION_INPUT, 10, 20);

    QVERIFY(1 == this->brake->GetGpioPin());
    QVERIFY(APart::PART_PIN_DIRECTION_INPUT == this->brake->GetPinDirection());
    QVERIFY(10 == this->brake->GetChatteringTime());
    QVERIFY(20 == this->brake->GetPeriodTime());
    QVERIFY(false == this->brake->GetIsHold());

    delete this->brake;
}
void CBrake_utest::test_case_003()
{
    this->brake = new CBrake(1, APart::PART_PIN_DIRECTION_OUTPUT, 10, 20);

    QVERIFY(APart::PART_PIN_DIRECTION_OUTPUT == this->brake->GetPinDirection());

    delete this->brake;
}
void CBrake_utest::test_case_004()
{
    this->brake = new CBrake(1, APart::PART_PIN_DIRECTION_IN_OUT, 10, 20);

    QVERIFY(APart::PART_PIN_DIRECTION_IN_OUT == this->brake->GetPinDirection());

    delete this->brake;
}
void CBrake_utest::test_case_005()
{
    this->brake = new CBrake();
    this->brake->Update(0);

    QVERIFY(false == this->brake->GetIsHold());

    delete this->brake;
}
void CBrake_utest::test_case_006()
{
    this->brake = new CBrake();
    this->brake->Update(1);

    QVERIFY(true == this->brake->GetIsHold());

    delete this->brake;
}
void CBrake_utest::test_case_007()
{
    this->brake = new CBrake();
    this->brake->Update(0x7FFFFFFF);

    QVERIFY(true == this->brake->GetIsHold());

    delete this->brake;
}
void CBrake_utest::test_case_008()
{
    this->brake = new CBrake();
    this->brake->Update(0x80000000);

    QVERIFY(true == this->brake->GetIsHold());

    delete this->brake;
}
void CBrake_utest::test_case_009()
{
    this->brake = new CBrake();
    this->brake->InterruptCallback(0);

    QVERIFY(false == this->brake->GetIsHold());

    delete this->brake;
}
void CBrake_utest::test_case_010()
{
    this->brake = new CBrake();
    this->brake->InterruptCallback(1);

    QVERIFY(true == this->brake->GetIsHold());

    delete this->brake;
}
void CBrake_utest::test_case_011()
{
    this->brake = new CBrake();
    this->brake->InterruptCallback(0x7FFF);

    QVERIFY(true == this->brake->GetIsHold());

    delete this->brake;
}
void CBrake_utest::test_case_012()
{
    this->brake = new CBrake();
    this->brake->InterruptCallback(0x8000);

    QVERIFY(true == this->brake->GetIsHold());

    delete this->brake;
}

QTEST_APPLESS_MAIN(CBrake_utest)

#include "tst_cbrake_utest.moc"
