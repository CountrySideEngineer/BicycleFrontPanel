#include <QtTest>
#include <QDebug>

// add necessary includes here
#include "cdatetime.h"

class BicycleFrontPanel_utest : public QObject
{
    Q_OBJECT

public:
    BicycleFrontPanel_utest();
    ~BicycleFrontPanel_utest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case_CDateTime_001();
    void test_case_CDateTime_002();

};

BicycleFrontPanel_utest::BicycleFrontPanel_utest()
{

}

BicycleFrontPanel_utest::~BicycleFrontPanel_utest()
{

}

void BicycleFrontPanel_utest::initTestCase()
{

}

void BicycleFrontPanel_utest::cleanupTestCase()
{

}

void BicycleFrontPanel_utest::test_case_CDateTime_001()
{
    CDateTime dateTime(QString("hh:mm ap"));
    QString curTime = dateTime.getTime();

    qDebug() << curTime;
}

void BicycleFrontPanel_utest::test_case_CDateTime_002()
{
    CDateTime dateTime(QString("hh:mm"));
    QString curTime = dateTime.getTime();

    qDebug() << curTime;
}

QTEST_APPLESS_MAIN(BicycleFrontPanel_utest)

#include "tst_bicyclefrontpanel_utest.moc"
