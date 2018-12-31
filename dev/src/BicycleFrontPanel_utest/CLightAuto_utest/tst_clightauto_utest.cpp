#include <QtTest>

// add necessary includes here
#include "apart.h"
#include "alight.h"
#include "clightauto.h"

class CLightAuto_utest : public QObject
{
    Q_OBJECT

public:
    CLightAuto_utest();
    ~CLightAuto_utest();

private slots:
    void test_case1();
    void test_case2();
    void test_case3();
    void test_case4();
    void test_case5();
};

CLightAuto_utest::CLightAuto_utest() {}
CLightAuto_utest::~CLightAuto_utest() {}

/**
 * @brief CLightAuto_utest::test_case1  Default constructor
 */
void CLightAuto_utest::test_case1()
{
    CLightAuto LightAuto;

    QVERIFY(0 == LightAuto.GetState());
    QVERIFY(0 == LightAuto.GetGpioPin());
    QVERIFY(0 == LightAuto.GetChatteringTime());
    QVERIFY(0 == LightAuto.GetPeriodTime());
    QVERIFY(false == LightAuto.GetIsFailure());
    QVERIFY(0 == LightAuto.GetFailureCode());
    QVERIFY(APart::PART_PIN_DIRECTION_MAX == LightAuto.GetPinDirection());
    QVERIFY(ALight::LIGHT_MODE_AUTO == LightAuto.GetLightMode());
    QVERIFY(ALight::LIGHT_STATE_OFF == LightAuto.GetLightState());
}
/**
 * @brief CLightAuto_utest::test_case2  Utest of constructor with arguments.
 */
void CLightAuto_utest::test_case2()
{
    CLightAuto LightAuto(1, APart::PART_PIN_DIRECTION_INPUT, 2, 3);

    QVERIFY(0 == LightAuto.GetState());
    QVERIFY(1 == LightAuto.GetGpioPin());
    QVERIFY(2 == LightAuto.GetChatteringTime());
    QVERIFY(3 == LightAuto.GetPeriodTime());
    QVERIFY(false == LightAuto.GetIsFailure());
    QVERIFY(0 == LightAuto.GetFailureCode());
    QVERIFY(APart::PART_PIN_DIRECTION_INPUT == LightAuto.GetPinDirection());
    QVERIFY(ALight::LIGHT_MODE_AUTO == LightAuto.GetLightMode());
    QVERIFY(ALight::LIGHT_STATE_OFF == LightAuto.GetLightState());
}
/**
 * @brief CLightAuto_utest::test_case3  Utest of update(), Border.
 */
void CLightAuto_utest::test_case3()
{
    CLightAuto LightAuto;

    LightAuto.SetOnOffBorder(10);
    LightAuto.Update(10);

    QVERIFY(CLightAuto::LIGHT_STATE_OFF == LightAuto.GetLightState());
}
/**
 * @brief CLightAuto_utest::test_case3  Utest of update(), Border - 1.
 */
void CLightAuto_utest::test_case4()
{
    CLightAuto LightAuto;

    LightAuto.SetOnOffBorder(10);
    LightAuto.Update(9);

    QVERIFY(CLightAuto::LIGHT_STATE_OFF == LightAuto.GetLightState());
}
/**
 * @brief CLightAuto_utest::test_case3  Utest of update(), Border + 1.
 */
void CLightAuto_utest::test_case5()
{
    CLightAuto LightAuto;

    LightAuto.SetOnOffBorder(10);
    LightAuto.Update(11);

    QVERIFY(CLightAuto::LIGHT_STATE_ON == LightAuto.GetLightState());
}

QTEST_APPLESS_MAIN(CLightAuto_utest)

#include "tst_clightauto_utest.moc"
