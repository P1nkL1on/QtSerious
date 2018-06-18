#include "definetests.h"
#include "defines.h"

const double eps = .0001;


using namespace Df;
bool Tests::DefineTests::isZero(const double &val)
{
    return (val < eps && val > -eps);
}

bool Tests::DefineTests::isNear(const double &val, const double &near)
{
    return isZero(val - near);
}

void Tests::DefineTests::testDefineVec301()
{
    Vector3<double> f = makeVector3fromDoubles<double>(1, -2, .0003);
    QCOMPARE(f(0,0), 1.0);
    QCOMPARE(f(1,0), -2.0);
    QCOMPARE(f(2,0), .0003);
    //QVERIFY(true);
}

void Tests::DefineTests::testTranslateVec02()
{
    Vector3<double> f = makeVector3fromDoubles<double>(10, 0, -1);
    f = applyTransformTransposed(f, translationMatrix(makeVector3fromDoubles<double>(5,6,7)));
    QCOMPARE(f(0,0), 15.0);
    QCOMPARE(f(1,0), 6.0);
    QCOMPARE(f(2,0), 6.0);
}

void Tests::DefineTests::testRotationX03()
{
    Vector3<double> f = makeVector3fromDoubles<double>(10, 0, -1);
    f = makeVector3fromDoubles<double>(10, 0, -1);
    f = applyTransformTransposed(f, rotationMatrixX<double>(90));
    QVERIFY(isZero(f(2,0)));
    QVERIFY(isNear(f(0,0), 10.0));
    QVERIFY(isNear(f(1,0), 1.0));
}

void Tests::DefineTests::testRotationX04()
{
    Vector3<double> f = makeVector3fromDoubles<double>(10, 0, -1);
    f = makeVector3fromDoubles<double>(10, 0, -1);
    f = applyTransformTransposed(f, rotationMatrix<double>(makeVector3fromDoubles<double>(90,0,0)));
    QVERIFY(isZero(f(2,0)));
    QVERIFY(isNear(f(0,0), 10.0));
    QVERIFY(isNear(f(1,0), 1.0));
}
