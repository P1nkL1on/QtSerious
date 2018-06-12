#include "autorigtests.h"

//#include "autorig.h"

int square(const int x)
{
    return x * x;
}

void AutoRig::AutoRigTests::testForwardTransform01()
{
//    QSKIP("Some test skipped");
    const int res = square(4);
    QCOMPARE(res, 16);
    QVERIFY(res == 16);

    long i = 10;
    QCOMPARE(i, static_cast<long>(10));
}
