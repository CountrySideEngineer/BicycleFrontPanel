#include <QtTest>

// add necessary includes here
#include "apart.h"
#include "alight.h"
#include "clightmanual.h"

class CLightManual_utest : public QObject
{
    Q_OBJECT

public:
    CLightManual_utest();
    ~CLightManual_utest();

private slots:
    void test_case1();
    void test_case2();
    void test_case3();
    void test_case4();
    void test_case5();
    void test_case6();
    void test_case7();

};
CLightManual_utest::CLightManual_utest() {}
CLightManual_utest::~CLightManual_utest() {}
void CLightManual_utest::test_case1()
{
    CLightManual LightManual;

    QVERIFY(0 == LightManual.GetState());
    QVERIFY(0 == LightManual.GetGpioPin());
    QVERIFY(0 == LightManual.GetChatteringTime());
    QVERIFY(0 == LightManual.GetPeriodTime());
    QVERIFY(false == LightManual.GetIsFailure());
    QVERIFY(0 == LightManual.GetFailureCode());
    QVERIFY(APart::PART_PIN_DIRECTION_MAX == LightManual.GetPinDirection());
    QVERIFY(ALight::LIGHT_MODE_MANUAL == LightManual.GetLightMode());
    QVERIFY(ALight::LIGHT_STATE_OFF == LightManual.GetLightState());
}
void CLightManual_utest::test_case2()
{
    CLightManual LightManual(1, APart::PART_PIN_DIRECTION_INPUT, 2, 3, 4);

    QVERIFY(0 == LightManual.GetState());
    QVERIFY(1 == LightManual.GetGpioPin());
    QVERIFY(2 == LightManual.GetChatteringTime());
    QVERIFY(3 == LightManual.GetPeriodTime());
    QVERIFY(false == LightManual.GetIsFailure());
    QVERIFY(4 == LightManual.GetOnOffBorder());
    QVERIFY(0 == LightManual.GetFailureCode());
    QVERIFY(APart::PART_PIN_DIRECTION_INPUT == LightManual.GetPinDirection());
    QVERIFY(ALight::LIGHT_MODE_MANUAL == LightManual.GetLightMode());
    QVERIFY(ALight::LIGHT_STATE_OFF == LightManual.GetLightState());
}
void CLightManual_utest::test_case3()
{
    CLightManual LightManual;

    LightManual.SetOnOffBorder(10);
    LightManual.Update(9);

    QVERIFY(CLightManual::LIGHT_STATE_OFF == LightManual.GetLightState());
}
void CLightManual_utest::test_case4()
{
    CLightManual LightManual;

    LightManual.SetOnOffBorder(10);
    LightManual.Update(10);

    QVERIFY(CLightManual::LIGHT_STATE_OFF == LightManual.GetLightState());
}
void CLightManual_utest::test_case5()
{
    CLightManual LightManual;

    LightManual.SetOnOffBorder(10);
    LightManual.Update(11);

    QVERIFY(CLightManual::LIGHT_STATE_REQUEST == LightManual.GetLightState());
}
void CLightManual_utest::test_case6()
{
    CLightManual LightManual;

    LightManual.Update();
    LightManual.SetOnOffBorder(10);
    LightManual.Update(11);

    QVERIFY(true == LightManual.GetCanTurnOn());
    QVERIFY(CLightManual::LIGHT_STATE_ON == LightManual.GetLightState());
}
void CLightManual_utest::test_case7()
{
    CLightManual LightManual;

    QVERIFY(false == LightManual.GetCanTurnOn());

    LightManual.Update();
    QVERIFY(true == LightManual.GetCanTurnOn());

    LightManual.Update();
    QVERIFY(false == LightManual.GetCanTurnOn());
}

QTEST_APPLESS_MAIN(CLightManual_utest)

#include "tst_clightmanual_utest.moc"
