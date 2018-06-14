#include "iofbxtests.h"

using namespace IOfbx;

QString IOfbxTests::loadAndReturnError(const QString &fileName)
{
    QString err = "";
    const FbxParsedContainer *res = loadFromPath(makePathFromName(fileName), err);
    if (res != nullptr){
        delete res;
    }
    qDebug() << "Message: " << err;
    return err;
}

QVector<QString> IOfbxTests::findIdAndName(const QString &S)
{
    QString id;
    QString name;
    QString subName;
    findIdAndNameInLine(S, id, name, subName);
    return {id, name, subName};
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
    QVERIFY( res->getClusters().length() == 2);
    QVERIFY( !res->getMesh().isEmpty());
    QVERIFY( res->getJoints().length() == 3);
    QVERIFY( res->getPosenodes().length() == 3);
    QVERIFY( res->getConnections().length() == 3);
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
    QVERIFY( res->getClusters().length() == 2);
    QVERIFY( !res->getMesh().isEmpty());
    QVERIFY( res->getJoints().length() == 3);
    QVERIFY( res->getPosenodes().length() == 3);
    QVERIFY( res->getConnections().length() == 3);
}


void IOfbxTests::testFinderUsuallCase01()
{
    auto parsed = findIdAndName("  Model: 1852460320, \"Model::RightToe_End\", \"LimbNode\" {");
    QVERIFY(parsed[0] == "1852460320");
    QVERIFY(parsed[1] == "Model::RightToe_End");
    QVERIFY(parsed[2] == "LimbNode");
}

void IOfbxTests::testFinderIdOnly02()
{
    auto parsed = findIdAndName("          Model: 1852469088 {");
    QVERIFY(parsed[0].isEmpty());
    QVERIFY(parsed[1].isEmpty());
    QVERIFY(parsed[2].isEmpty());
}

void IOfbxTests::testFinderIdName03()
{
    auto parsed = findIdAndName("               Geometry: 1708246768, \"Geometry::\" {");
    QVERIFY(parsed[0] == "1708246768");
    QVERIFY(parsed[1] == "Geometry::");
    QVERIFY(parsed[2].isEmpty());
}

void IOfbxTests::testFinderEmptyLine04()
{
    auto parsed = findIdAndName("");
    QVERIFY(parsed[0].isEmpty());
    QVERIFY(parsed[1].isEmpty());
    QVERIFY(parsed[2].isEmpty());
}

void IOfbxTests::testFinderIdSkipper05()
{
    auto parsed = findIdAndName("  Model: \"Model::RightToe_End\", \"LimbNode\" {");
    QVERIFY(parsed[0].isEmpty());
    QVERIFY(parsed[1].isEmpty());
    QVERIFY(parsed[2].isEmpty());
}

void IOfbxTests::testFinderErrored06()
{
    auto parsed = findIdAndName("			a: 16.2139301300049,191.863830566406,3.98515129089355,18.8588027954102");
    QVERIFY(parsed[0].isEmpty());
    QVERIFY(parsed[1].isEmpty());
    QVERIFY(parsed[2].isEmpty());
}
