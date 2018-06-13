#ifndef AUTORIGTESTS_H
#define AUTORIGTESTS_H

#include <QTest>

namespace AutoRig {

class AutoRigTests : public QObject
{
    Q_OBJECT
public:

private slots:
    void testForwardTransform01();
};

}

#endif // AUTORIGTESTS_H
