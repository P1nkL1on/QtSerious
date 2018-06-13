#include "iofbxtests.h"

using namespace IOfbx;

void IOfbx::IOfbxTests::clearContainer()
{
    if (res != nullptr){
        delete res;
    }
    res = nullptr;
}


QString IOfbxTests::testLoading(const QString fileName, FbxParsedContainer *&res)
{
    clearContainer();
    QString err = "";

    res = loadFromPath(testFolder + fileName + ".FBX", err);

    return err;
}

void IOfbxTests::testIncorrectFilePath01()
{
    QVERIFY(testLoading("test-1", res).indexOf("The path of readable file is incorrect : ") == 0);
}

void IOfbx::IOfbxTests::testEmptyFile02()
{
    QVERIFY(testLoading("test0", res).indexOf("The file is empty : ") == 0);
}

void IOfbxTests::testStrangeFileContaned03()
{
    QVERIFY(testLoading("test1", res).indexOf("File does not contain any rigs") >= 0);
}

void IOfbxTests::testStrangeFileContaned04()
{
    QVERIFY(testLoading("test2", res).indexOf("File does not contain any rigs") >= 0);
}

void IOfbxTests::testOnlyMeshContaints05()
{
    QVERIFY(testLoading("test4", res).indexOf("File contains only mesh!") >= 0);
}

void IOfbxTests::testTooMuchTabs07()
{
    QString err = testLoading("test5", res);

    QVERIFY(err.isEmpty());
    QVERIFY( res->getClusters()->length() == 2);
    QVERIFY( res->getMesh() != nullptr);
    QVERIFY( res->getJoints()->length() == 3);
    QVERIFY( res->getPosenodes()->length() == 3);
    QVERIFY( res->getConnections()->length() == 3);
}

void IOfbxTests::testCorruptedNumbers08()
{
    QVERIFY(!testLoading("test6", res).isEmpty());
}

void IOfbxTests::testIncorrectMatrixCount09()
{
    QVERIFY(!testLoading("test7", res).isEmpty());
}

void IOfbxTests::testTheMostEasyExample06()
{
    QString err = testLoading("test3", res);

    QVERIFY(err.isEmpty());
    QVERIFY( res->getClusters()->length() == 2);
    QVERIFY( res->getMesh() != nullptr);
    QVERIFY( res->getJoints()->length() == 3);
    QVERIFY( res->getPosenodes()->length() == 3);
    QVERIFY( res->getConnections()->length() == 3);
}

