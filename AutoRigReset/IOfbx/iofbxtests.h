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
    QVector<QString> findIdAndName (const QString &S);
    FbxParsedContainer *res = nullptr;

    const QString testFolder = "../Models/FBXtest/";

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

    void testFinderUsuallCase01();
    void testFinderIdOnly02();
    void testFinderIdName03();
    void testFinderEmptyLine04();
    void testFinderIdSkipper05();
    void testFinderErrored06();
};
}

#endif // IOFBXTESTS_H
