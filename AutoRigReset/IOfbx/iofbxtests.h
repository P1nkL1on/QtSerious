#ifndef IOFBXTESTS_H
#define IOFBXTESTS_H

#include <QTest>
#include "iofbx.h"

namespace IOfbx{
class IOfbxTests : public QObject
{
    Q_OBJECT
public:

private:
    QString loadAndReturnError(const QString &fileName);
    FbxParsedContainer *res = nullptr;
#warning Make it relative
    const QString testFolder = "D:/QT-serious/Models/FBXtest/";
//    const QString testFolder = "../FBXtest/";
    QString makePathFromName(const QString fileName) const;
private slots:
    void testIncorrectFilePath01();
    void testEmptyFile02();
    void testStrangeFileContaned03();
    void testStrangeFileContaned04();
    void testOnlyMeshContaints05();
    void testTheMostEasyExample06();
    void testTooMuchTabs07();
    void testCorruptedNumbers08();
    void testIncorrectMatrixCount09();
};
}

#endif // IOFBXTESTS_H
