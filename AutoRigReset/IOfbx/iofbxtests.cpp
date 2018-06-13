#include "iofbxtests.h"

using namespace IOfbx;

QString IOfbxTests::loadAndReturnError(const QString &fileName)
{
    QString err = "";
    const FbxParsedContainer *res = loadFromPath(makePathFromName(fileName), err);
    if (res != nullptr){
        delete res;
    }
    return err;
}

QString IOfbxTests::makePathFromName(const QString fileName) const
{
    return testFolder + fileName + ".FBX";
}

void IOfbxTests::testIncorrectFilePath01()
{    
    QVERIFY(loadAndReturnError("test-1").indexOf("The path of readable file is incorrect : ") == 0);
}

void IOfbx::IOfbxTests::testEmptyFile02()
{
    QVERIFY(loadAndReturnError("test0").indexOf("The file is empty : ") == 0);
}

void IOfbxTests::testStrangeFileContaned03()
{
    QVERIFY(loadAndReturnError("test1").indexOf("File does not contain any rigs") >= 0);
}

void IOfbxTests::testStrangeFileContaned04()
{
    QVERIFY(loadAndReturnError("test2").indexOf("File does not contain any rigs") >= 0);
}

void IOfbxTests::testOnlyMeshContaints05()
{
    QVERIFY(loadAndReturnError("test4").indexOf("File contains only mesh!") >= 0);
}

void IOfbxTests::testTooMuchTabs07()
{
    QString err;
    res = loadFromPath(makePathFromName("test5"), err);

    QVERIFY(err.isEmpty());
    QVERIFY( res->getClusters()->length() == 2);
    QVERIFY( res->getMesh() != nullptr);
    QVERIFY( res->getJoints()->length() == 3);
    QVERIFY( res->getPosenodes()->length() == 3);
    QVERIFY( res->getConnections()->length() == 3);
}

void IOfbxTests::testCorruptedNumbers08()
{
    QVERIFY(!loadAndReturnError("test6").isEmpty());
}

void IOfbxTests::testIncorrectMatrixCount09()
{
    QVERIFY(!loadAndReturnError("test7").isEmpty());
}

void IOfbxTests::testTheMostEasyExample06()
{
    QString err;
    res = loadFromPath(makePathFromName("test3"), err);

    QVERIFY(err.isEmpty());
    QVERIFY( res->getClusters()->length() == 2);
    QVERIFY( res->getMesh() != nullptr);
    QVERIFY( res->getJoints()->length() == 3);
    QVERIFY( res->getPosenodes()->length() == 3);
    QVERIFY( res->getConnections()->length() == 3);
}

