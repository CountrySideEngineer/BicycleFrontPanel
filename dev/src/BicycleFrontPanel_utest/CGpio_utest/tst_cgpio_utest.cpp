#include <QtTest>

// add necessary includes here
#include <map>
#include "pigpio/pigpio.h"
#include "model/cgpio.h"
#include "model/cbrake.h"
#include "cpartmock.h"
using namespace std;

//Stub.
extern void pigpio_stub_init();

//variables.
extern unsigned int gpioInitialise_called_counter;
extern int gpioInitialise_return[];
extern unsigned int gpioTerminate_called_counter;
extern unsigned int gpioSetMode_called_counter;
extern unsigned int gpioSetMode_gpio[];
extern unsigned int gpioSetMode_mode[];
extern int gpioSetMode_return[];
extern unsigned int gpioRead_called_counter;
extern unsigned int gpioRead_gpio[];
extern int gpioRead_return[];
extern unsigned int gpioSetTimerFunc_called_counter;
extern unsigned int gpioSetTimerFunc_timer[];
extern unsigned int gpioSetTimerFunc_millis[];
extern gpioTimerFunc_t gpioSetTimerFunc_gpioTimerFunc[];
extern int gpioSetTimerFunc_return[];
extern unsigned int gpioSetISRFunc_called_counter;
extern unsigned int gpioSetISRFunc_gpio[];
extern unsigned int gpioSetISRFunc_edge[];
extern int gpioSetISRFunc_timeout[];
extern gpioISRFunc_t gpioSetISRFunc_f[];
extern int gpioSetISRFunc_return[];

class CGpio_utest : public QObject
{
    Q_OBJECT

public:
    CGpio_utest();
    ~CGpio_utest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Initialize_case001();
    void test_Initialize_case002();
    void test_Finalize_case001();
    void test_SetMode_case001();
    void test_SetMode_case002();
    void test_SetMode_case003();
    void test_SetMode_case004();
    void test_SetIsr_case001();
    void test_SetIsr_case002();
    void test_IntoCriticalSection_001();
    void test_ExitCriticalSection_001();
    void test_StartChatteringTimer_001();
    void test_ChatteringTimeDispatch_001();
    void test_ChatteringTimeDispatch_002();
    void test_ChatteringTimeDispatch_003();
    void test_ChatteringTimeDispatch_004();
    void test_ChatteringTimeDispatch_005();
    void test_ChatteringTimeDispatch_006();
    void test_Interrupt_001();
    void test_Interrupt_002();
    void test_Interrupt_003();
    void test_Interrupt_004();
    void test_SetTimeIsr_001();
    void test_RemoveTimeIsr_001();
    void test_RemoveTimeIsr_002();
    void test_RemoveTimeIsr_003();
    void test_RemoveTimeIsr_004();
};

CGpio_utest::CGpio_utest() {}
CGpio_utest::~CGpio_utest() {}
void CGpio_utest::initTestCase() {}
void CGpio_utest::cleanupTestCase() {}
void CGpio_utest::test_Initialize_case001()
{
    pigpio_stub_init();

    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    QVERIFY(NULL != instance);
    QVERIFY(false == instance->GetInCritical());
    QVERIFY(0xFF == instance->GetInterruptPin());
    QVERIFY(1 == gpioInitialise_called_counter);
    QVERIFY(2 == gpioSetTimerFunc_called_counter);
    QVERIFY(1 == gpioSetTimerFunc_timer[0]);
    QVERIFY(10 == gpioSetTimerFunc_millis[0]);
    QVERIFY(0 == gpioSetTimerFunc_timer[1]);
    QVERIFY(20 == gpioSetTimerFunc_millis[1]);
    QVERIFY(NULL != gpioSetTimerFunc_gpioTimerFunc[0]);
    QVERIFY(NULL != gpioSetTimerFunc_gpioTimerFunc[1]);

    CGpio::Finalize();
}
void CGpio_utest::test_Initialize_case002()
{
    pigpio_stub_init();

    gpioInitialise_return[0] = -1;

    CGpio::Initialize();

    QVERIFY(1 == gpioInitialise_called_counter);
    QVERIFY(0 == gpioSetTimerFunc_called_counter);

    CGpio::Finalize();
}
void CGpio_utest::test_Finalize_case001()
{

    CGpio::Initialize();

    pigpio_stub_init();

    CGpio::Finalize();

    QVERIFY(2 == gpioSetTimerFunc_called_counter);
    QVERIFY(1 == gpioSetTimerFunc_timer[0]);
    QVERIFY(10 == gpioSetTimerFunc_millis[0]);
    QVERIFY(NULL == gpioSetTimerFunc_gpioTimerFunc[0]);
    QVERIFY(0 == gpioSetTimerFunc_timer[1]);
    QVERIFY(20 == gpioSetTimerFunc_millis[1]);
    QVERIFY(NULL == gpioSetTimerFunc_gpioTimerFunc[1]);
    QVERIFY(1 == gpioTerminate_called_counter);
}
void CGpio_utest::test_SetMode_case001()
{
    pigpio_stub_init();

    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    uint pin = 0;
    CGpio::GPIO_PIN_DIRECTION direction = CGpio::GPIO_PIN_DIRECTION_INPUT;
    instance->SetMode(pin, direction);

    QVERIFY(1 == gpioSetMode_called_counter);
    QVERIFY(0 == gpioSetMode_gpio[0]);
    QVERIFY(PI_INPUT == gpioSetMode_mode[0]);

    CGpio::Finalize();
}
void CGpio_utest::test_SetMode_case002()
{
    pigpio_stub_init();

    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    uint pin = 1;
    CGpio::GPIO_PIN_DIRECTION direction = CGpio::GPIO_PIN_DIRECTION_OUTPUT;

    instance->SetMode(pin ,direction);

    QVERIFY(1 == gpioSetMode_called_counter);
    QVERIFY(1 == gpioSetMode_gpio[0]);
    QVERIFY(PI_OUTPUT == gpioSetMode_mode[0]);

    CGpio::Finalize();
}
void CGpio_utest::test_SetMode_case003()
{
    pigpio_stub_init();

    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    uint pin = 2;
    CGpio::GPIO_PIN_DIRECTION direction = CGpio::GPIO_PIN_DIRECTION_MAX;

    instance->SetMode(pin ,direction);

    QVERIFY(1 == gpioSetMode_called_counter);
    QVERIFY(2 == gpioSetMode_gpio[0]);
    QVERIFY(0xFF == gpioSetMode_mode[0]);

    CGpio::Finalize();
}
void CGpio_utest::test_SetMode_case004()
{
    pigpio_stub_init();

    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    uint pin = 0xFF;
    CGpio::GPIO_PIN_DIRECTION direction = CGpio::GPIO_PIN_DIRECTION_INPUT;

    instance->SetMode(pin ,direction);

    QVERIFY(1 == gpioSetMode_called_counter);
    QVERIFY(0xFF == gpioSetMode_gpio[0]);
    QVERIFY(PI_INPUT == gpioSetMode_mode[0]);

    CGpio::Finalize();
}
void CGpio_utest::test_SetIsr_case001()
{
    pigpio_stub_init();

    uint pin = 1;
    uint edge = 2;

    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    APart* part = new CBrake();
    instance->SetIsr(pin, edge, part);

    QVERIFY(1 == gpioSetISRFunc_called_counter);
    QVERIFY(1 == instance->GetPinMap()->count(pin));
    QVERIFY(false == instance->IsCriticalSection(pin));

    CGpio::Finalize();
}
void CGpio_utest::test_SetIsr_case002()
{
    pigpio_stub_init();

    uint pin = 1;
    uint edge = 2;

    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    APart* part = new CBrake();
    instance->SetIsr(pin + 1, edge, part);

    QVERIFY(1 == gpioSetISRFunc_called_counter);
    QVERIFY(0 == instance->GetPinMap()->count(pin));
    QVERIFY(false == instance->IsCriticalSection(pin));

    CGpio::Finalize();
}
void CGpio_utest::test_IntoCriticalSection_001()
{
    pigpio_stub_init();

    uint pin = 1;

    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    instance->IntoCriticalSection(pin);

    QVERIFY(true == instance->IsCriticalSection(pin));
    QVERIFY(false == instance->IsCriticalSection(pin + 1));

    CGpio::Finalize();
}
void CGpio_utest::test_ExitCriticalSection_001()
{
    pigpio_stub_init();

    uint pin = 1;

    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    instance->IntoCriticalSection(pin);
    instance->ExitCriticalSection(pin);

    QVERIFY(false == instance->IsCriticalSection(pin));
    QVERIFY(false == instance->IsCriticalSection(pin + 1));

    CGpio::Finalize();
}
void CGpio_utest::test_StartChatteringTimer_001()
{
    pigpio_stub_init();

    APart* part = new CBrake(1, APart::PART_PIN_DIRECTION_IN_OUT, 10, 20);
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    QVERIFY(0 == instance->GetWaitChattering()->size());

    instance->StartChatteringTimer(part);

    QVERIFY(1 == instance->GetWaitChattering()->size());

    CGpio::Finalize();
}
void CGpio_utest::test_ChatteringTimeDispatch_001()
{
    pigpio_stub_init();

    APart* part = new CPartMock(1, APart::PART_PIN_DIRECTION_IN_OUT, 10, 20);
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    gpioRead_return[0] = 10;

    instance->SetIsr(part->GetGpioPin(), 0, part);
    instance->ChatteringTimeDispatch();

    CPartMock* Mock = dynamic_cast<CPartMock*>(part);

    QVERIFY(0 == Mock->GetInterruptCallbackCalledCount());
    QVERIFY(0 == instance->GetWaitChattering()->size());

    CGpio::Finalize();
}
void CGpio_utest::test_ChatteringTimeDispatch_002()
{
    pigpio_stub_init();

    APart* part = new CPartMock(2, APart::PART_PIN_DIRECTION_IN_OUT, 0, 0);
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    gpioRead_return[0] = 11;

    instance->SetIsr(part->GetGpioPin(), 0, part);
    instance->StartChatteringTimer(part);
    instance->ChatteringTimeDispatch();

    CPartMock* Mock = dynamic_cast<CPartMock*>(part);

    QVERIFY(1 == Mock->GetInterruptCallbackCalledCount());
    QVERIFY(11 == Mock->GetInterruptCallback1StArg(0));
    QVERIFY(1 == gpioRead_called_counter);
    QVERIFY(2 == gpioRead_gpio[0]);
    QVERIFY(0 == instance->GetWaitChattering()->size());

    CGpio::Finalize();
}
void CGpio_utest::test_ChatteringTimeDispatch_003()
{
    pigpio_stub_init();

    APart* part1 = new CPartMock(2, APart::PART_PIN_DIRECTION_IN_OUT, 0, 0);
    APart* part2 = new CPartMock(3, APart::PART_PIN_DIRECTION_IN_OUT, 0, 0);
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    gpioRead_return[0] = 11;
    gpioRead_return[1] = 12;

    instance->SetIsr(part1->GetGpioPin(), 0, part1);
    instance->StartChatteringTimer(part1);
    instance->StartChatteringTimer(part2);

    QThread::sleep(1);

    instance->ChatteringTimeDispatch();

    CPartMock* Mock1 = dynamic_cast<CPartMock*>(part1);
    CPartMock* Mock2 = dynamic_cast<CPartMock*>(part2);

    QVERIFY(1 == Mock1->GetInterruptCallbackCalledCount());
    QVERIFY(11 == Mock1->GetInterruptCallback1StArg(0));
    QVERIFY(1 == Mock2->GetInterruptCallbackCalledCount());
    QVERIFY(12 == Mock2->GetInterruptCallback1StArg(0));
    QVERIFY(2 == gpioRead_called_counter);
    QVERIFY(2 == gpioRead_gpio[0]);
    QVERIFY(3 == gpioRead_gpio[1]);
    QVERIFY(0 == instance->GetWaitChattering()->size());

    CGpio::Finalize();
}
void CGpio_utest::test_ChatteringTimeDispatch_004()
{
    pigpio_stub_init();

    APart* part1 = new CPartMock(2, APart::PART_PIN_DIRECTION_IN_OUT, 0, 0);
    APart* part2 = new CPartMock(3, APart::PART_PIN_DIRECTION_IN_OUT, 10000, 0);
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    gpioRead_return[0] = 11;
    gpioRead_return[1] = 12;

    instance->SetIsr(part1->GetGpioPin(), 0, part1);
    instance->StartChatteringTimer(part1);
    instance->StartChatteringTimer(part2);

    QThread::sleep(1);

    instance->ChatteringTimeDispatch();

    CPartMock* Mock1 = dynamic_cast<CPartMock*>(part1);
    CPartMock* Mock2 = dynamic_cast<CPartMock*>(part2);

    QVERIFY(1 == Mock1->GetInterruptCallbackCalledCount());
    QVERIFY(11 == Mock1->GetInterruptCallback1StArg(0));
    QVERIFY(0 == Mock2->GetInterruptCallbackCalledCount());
    QVERIFY(1 == gpioRead_called_counter);
    QVERIFY(2 == gpioRead_gpio[0]);
    QVERIFY(1 == instance->GetWaitChattering()->size());

    CGpio::Finalize();
}
void CGpio_utest::test_ChatteringTimeDispatch_005()
{
    pigpio_stub_init();

    APart* part1 = new CPartMock(2, APart::PART_PIN_DIRECTION_IN_OUT, 10000, 0);
    APart* part2 = new CPartMock(3, APart::PART_PIN_DIRECTION_IN_OUT, 0, 0);
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    gpioRead_return[0] = 11;
    gpioRead_return[1] = 12;

    instance->SetIsr(part1->GetGpioPin(), 0, part1);
    instance->StartChatteringTimer(part1);
    instance->StartChatteringTimer(part2);

    QThread::sleep(1);

    instance->ChatteringTimeDispatch();

    CPartMock* Mock1 = dynamic_cast<CPartMock*>(part1);
    CPartMock* Mock2 = dynamic_cast<CPartMock*>(part2);

    QVERIFY(0 == Mock1->GetInterruptCallbackCalledCount());
    QVERIFY(1 == Mock2->GetInterruptCallbackCalledCount());
    QVERIFY(11 == Mock2->GetInterruptCallback1StArg(0));
    QVERIFY(1 == gpioRead_called_counter);
    QVERIFY(3 == gpioRead_gpio[0]);
    QVERIFY(1 == instance->GetWaitChattering()->size());

    CGpio::Finalize();
}
void CGpio_utest::test_ChatteringTimeDispatch_006()
{
    pigpio_stub_init();

    APart* part1 = new CPartMock(2, APart::PART_PIN_DIRECTION_IN_OUT, 10000, 0);
    APart* part2 = new CPartMock(3, APart::PART_PIN_DIRECTION_IN_OUT, 10000, 0);
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    gpioRead_return[0] = 11;
    gpioRead_return[1] = 12;

    instance->SetIsr(part1->GetGpioPin(), 0, part1);
    instance->StartChatteringTimer(part1);
    instance->StartChatteringTimer(part2);

    QThread::sleep(1);

    instance->ChatteringTimeDispatch();

    CPartMock* Mock1 = dynamic_cast<CPartMock*>(part1);
    CPartMock* Mock2 = dynamic_cast<CPartMock*>(part2);

    QVERIFY(0 == Mock1->GetInterruptCallbackCalledCount());
    QVERIFY(0 == Mock2->GetInterruptCallbackCalledCount());
    QVERIFY(0 == gpioRead_called_counter);
    QVERIFY(2 == instance->GetWaitChattering()->size());

    CGpio::Finalize();
}
void CGpio_utest::test_Interrupt_001()
{
    pigpio_stub_init();

    CGpio::Initialize();

    CGpio::Interrupt(1, 2, 3);

    QVERIFY(0 == gpioRead_called_counter);

    CGpio::Finalize();
}
void CGpio_utest::test_Interrupt_002()
{
    pigpio_stub_init();

    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    APart* part1 = new CPartMock(1, APart::PART_PIN_DIRECTION_IN_OUT, 10000, 0);

    instance->SetIsr(part1->GetGpioPin(), 0, part1);
    instance->IntoCriticalSection(part1->GetGpioPin());

    CGpio::Interrupt(part1->GetGpioPin(), 2, 3);

    QVERIFY(0 == gpioRead_called_counter);

    CGpio::Finalize();
}
void CGpio_utest::test_Interrupt_003()
{
    pigpio_stub_init();

    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    APart* part1 = new CPartMock(11, APart::PART_PIN_DIRECTION_IN_OUT, 0, 0);

    instance->SetIsr(part1->GetGpioPin(), 0, part1);

    gpioRead_return[0] = 3;

    CGpio::Interrupt(part1->GetGpioPin(), 2, 3);

    CPartMock* Mock = dynamic_cast<CPartMock*>(part1);

    QVERIFY(1 == gpioRead_called_counter);
    QVERIFY(11 == gpioRead_gpio[0]);
    QVERIFY(1 == Mock->GetInterruptCallbackCalledCount());
    QVERIFY(3 == Mock->GetInterruptCallback1StArg(0));
    QVERIFY(false == instance->IsCriticalSection(part1->GetGpioPin()));

    CGpio::Finalize();
}
void CGpio_utest::test_Interrupt_004()
{
    pigpio_stub_init();

    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    APart* part1 = new CPartMock(11, APart::PART_PIN_DIRECTION_IN_OUT, 10000, 0);

    instance->SetIsr(part1->GetGpioPin(), 0, part1);

    gpioRead_return[0] = 3;

    CGpio::Interrupt(part1->GetGpioPin(), 2, 3);

    CPartMock* Mock = dynamic_cast<CPartMock*>(part1);

    QVERIFY(0 == gpioRead_called_counter);
    QVERIFY(0 == Mock->GetInterruptCallbackCalledCount());
    QVERIFY(true == instance->IsCriticalSection(part1->GetGpioPin()));
    QVERIFY(1 == instance->GetWaitChattering()->size());

    CGpio::Finalize();
}
void CGpio_utest::test_SetTimeIsr_001()
{
    pigpio_stub_init();
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    APart* part = new CBrake(1, APart::PART_PIN_DIRECTION_INPUT, 11, 22);
    instance->SetTimeIsr(part);

    QVERIFY(1 == instance->GetPeriodicTime()->size());
    QVERIFY(part == instance->GetPeriodicTime()->at(0)->GetParts());
    QVERIFY(22 == instance->GetPeriodicTime()->at(0)->GetWaitTime());

    CGpio::Finalize();
}
void CGpio_utest::test_RemoveTimeIsr_001()
{
    pigpio_stub_init();
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    APart* part = new CPartMock(11, APart::PART_PIN_DIRECTION_INPUT, 10, 20);
    instance->SetTimeIsr(part);

    instance->RemoveTimeIsr(part);

    QVERIFY(0 == instance->GetPeriodicTime()->size());

    delete part;
    CGpio::Finalize();
}
void CGpio_utest::test_RemoveTimeIsr_002()
{
    pigpio_stub_init();
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    APart* part1 = new CPartMock(11, APart::PART_PIN_DIRECTION_INPUT, 11, 21);
    instance->SetTimeIsr(part1);
    APart* part2 = new CPartMock(21, APart::PART_PIN_DIRECTION_INPUT, 12, 22);
    instance->SetTimeIsr(part2);

    instance->RemoveTimeIsr(part1);

    QVERIFY(1 == instance->GetPeriodicTime()->size());
    QVERIFY(part2 == instance->GetPeriodicTime()->at(0)->GetParts());

    delete part2;
    CGpio::Finalize();
}
void CGpio_utest::test_RemoveTimeIsr_003()
{
    pigpio_stub_init();
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    APart* part1 = new CPartMock(11, APart::PART_PIN_DIRECTION_INPUT, 11, 21);
    instance->SetTimeIsr(part1);
    APart* part2 = new CPartMock(21, APart::PART_PIN_DIRECTION_INPUT, 12, 22);
    instance->SetTimeIsr(part2);

    instance->RemoveTimeIsr(part2);

    QVERIFY(1 == instance->GetPeriodicTime()->size());
    QVERIFY(part1 == instance->GetPeriodicTime()->at(0)->GetParts());

    delete part1;
    CGpio::Finalize();
}
void CGpio_utest::test_RemoveTimeIsr_004()
{
    pigpio_stub_init();
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

    APart* part1 = new CPartMock(11, APart::PART_PIN_DIRECTION_INPUT, 11, 21);
    instance->SetTimeIsr(part1);
    APart* part2 = new CPartMock(21, APart::PART_PIN_DIRECTION_INPUT, 12, 22);
    instance->SetTimeIsr(part2);
    APart* part3 = new CPartMock(31, APart::PART_PIN_DIRECTION_INPUT, 13, 23);
    instance->SetTimeIsr(part3);

    instance->RemoveTimeIsr(part2);

    QVERIFY(2 == instance->GetPeriodicTime()->size());
    QVERIFY(part1 == instance->GetPeriodicTime()->at(0)->GetParts());
    QVERIFY(part3 == instance->GetPeriodicTime()->at(1)->GetParts());

    delete part1;
    delete part3;
    CGpio::Finalize();
}


QTEST_APPLESS_MAIN(CGpio_utest)

#include "tst_cgpio_utest.moc"
