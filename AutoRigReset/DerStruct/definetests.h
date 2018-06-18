#ifndef DERSTRUCT_TESTS_DEFINETESTS_H
#define DERSTRUCT_TESTS_DEFINETESTS_H

#include <QTest>
#include <QObject>

namespace Df {
namespace Tests{
class DefineTests : public QObject
{
    Q_OBJECT
private:
    bool isZero (const double &val);
    bool isNear (const double &val, const double &near);
private slots:
    void testDefineVec301();
    void testTranslateVec02();
    void testRotationX03();
    void testRotationX04();
};
}
}
#endif // DERSTRUCT_TESTS_DEFINETESTS_H
