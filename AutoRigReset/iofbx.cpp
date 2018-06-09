#include "iofbx.h"

#include "QFile"
#include "QTextStream"
#include "QDebug"
#include "QRegExp"

IOfbx::FbxGeometryMesh::FbxGeometryMesh()
{
    name = id = "";
    vertices = QVector<double>();
    polygonVertexIndex = QVector<int>();
}

void IOfbx::FbxModelJoint::addSubName(QString subName)
{
    this->subName = subName;
}

QString IOfbx::FbxGeometryMesh::parse(QStringList S, const int param)
{
    QString error = QString();

    S.removeFirst(); // -Vertices: *7122 {

    switch (param) {
    // 0 == vertices
    // 1 == PolygonVertexIndex
    case 0:
        vertices = parseFbxArray<double>(S, error);
        break;
    case 1:
        polygonVertexIndex = parseFbxArray<int>(S,error);
    default:
        break;
    }
    if (!error.isEmpty())
        return error;
    qDebug() << QString("v   Success parsed geometry mesh values;");
    return error;
}
QString IOfbx::FbxModelJoint::parse(QStringList S, const int param)
{
    QString error = QString();
    for (int lineInd = 0; lineInd < S.length(); lineInd++){
        QString line = S[lineInd];
        if (line.indexOf("\"Lcl") > 0){
            QVector<double> values = parseLastXValues<double>(line, 3, error);
            if (!error.isEmpty())
                return "Error in parsing 3 last values, error in line: " + line + "; " + error;
            if (line.indexOf("Lcl Translation") >= 0)
                this->localTranslation = values;
            if (line.indexOf("Lcl Rotation") >= 0)
                this->localRotation = values;
            if (line.indexOf("Lcl Scaling") >= 0)
                this->localScaling = values;
        }
    }
    qDebug() << QString("v   Success parsed joint " + this->name);
    return QString();
}

QString IOfbx::FbxPoseNode::parse(QStringList S, const int param)
{
//        PoseNode:  {
//			Node: 1848447504
//			Matrix: *16 {
//				a: 0.393700778484344,0,0,0,0,0.393700778484344,1.7209208280633e-008,0,0,-1.7209208280633e-008,0.393700778484344,0,-0.000393700785934925,45.167106628418,-0.192725494503975,1
//			}
    QStringList arrayContainer = QStringList();
    for (int i = 0; i < S.length(); i++)
        if (S[i].indexOf("Matrix:") >= 0)
        {
            QString error = "";
            transformMatrixArray = parseFbxArray<double>(QStringList(S[i + 1]), error);
            if (!error.isEmpty())
                return "Error in parsing 16 values in matrix of posenode, error in line: " + S[i] + "; " + error;
            break;
        }

    qDebug() << QString("v   Success parsed posenode;");
    return QString();
}



QString IOfbx::loadFromPath(const QString path)
{
    // get a file name to debug a process more shortly
    QString fileName = path;
    fileName = fileName.remove(0,path.lastIndexOf('/') + 1);
    // open stream, check for readable
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        // instant return if incorrect path
        return QString("The path of readable file is incorrect : " + path);

    QTextStream textStream(&file);

    // if filepath is empty, then do nothing
    if (textStream.atEnd())
        return "The file is empty : " + path;

    // initialise a struct for save parsed data
    FbxGeometryMesh fbxMesh = FbxGeometryMesh();   // a readen date of mesh
    QVector<FbxModelJoint> fbxJoints = QVector<FbxModelJoint>();    // a joints readen
    fbxJoints << FbxModelJoint();
    QVector<FbxPoseNode> fbxPoseNodes = QVector<FbxPoseNode>();
    fbxPoseNodes << FbxPoseNode();

    // read a file line by line
    QString line = "";
    QString nodeId = "noneID";
    QString nodeName = "unknown";
    QString nodeSubName = "unknown";
    QStringList nodeBuffer = QStringList();
    ParseType lastParsingType = ParseType::None;

    while (!textStream.atEnd()){
        line = textStream.readLine();
        ParseType detectedTypeHeader = pushHeader(line);

        // there is sometimes a need to remember an ID of node, you are currently parsing
        findIdAndNameInLine(line, nodeId, nodeName, nodeSubName);

        if (detectedTypeHeader != ParseType::None){
            // if it is first detected, then clear buffer and start copying
            if (lastParsingType == ParseType::None){
                nodeBuffer.clear();
            }
            // start to copy to buffer
            nodeBuffer.append(line);
        }else{
            // trace an info of name of parsed block
            int lpt = (int)lastParsingType;
            if (lpt >= 0)
                qDebug() << "v   Success: "
                            + ((lpt < parseBlockNames.length())? parseBlockNames[lpt] : "???")
                            + " buffer ( " + QString::number(nodeBuffer.length()) + " lines ) read;";

            // then just finished to
            QString err = QString();
                switch (lastParsingType)
                {
                    case ParseType::FbxGeometryMeshVertices:
                        if (!fbxMesh.hasNameAndID())
                            fbxMesh.setNameAndID(nodeName, nodeId);
                        // call a parser of mesh
                        err = fbxMesh.parse(nodeBuffer, 0);
                        if (!err.isEmpty())
                            return "x   Error in parsing a mesh vertices: " + err;
                        break;
                    case ParseType::FbxGeometryMeshPolygonIndexes:
                        err = fbxMesh.parse(nodeBuffer,1);
                        if (!err.isEmpty())
                            return "x   Error in parsing a mesh polygon idnexes: " + err;
                        break;
                    case ParseType::FbxObjectModelLimbNodeProperty:
                        if (!fbxJoints.last().hasNameAndID())
                            fbxJoints.last().setNameAndID(nodeName, nodeId);
                        err = fbxJoints.last().parse(nodeBuffer, 0);
                        fbxJoints.last().addSubName(nodeSubName);

                        if (!err.isEmpty())
                            return "x   Error in parsing a joint: " + err;
                        fbxJoints << FbxModelJoint();
                        break;
                    case ParseType::FbxObjectPoseNodeID:
                        err = fbxPoseNodes.last().parse(nodeBuffer, 0);
                        if (!err.isEmpty())
                            return "x   Error in parsing a posenode: " + err;
                        fbxPoseNodes << FbxPoseNode();
                        break;
                    default:
                        break;
                }
            if (!nodeBuffer.isEmpty())
                nodeBuffer.clear();
        }

        lastParsingType = detectedTypeHeader;
    }
    // post makes
    fbxJoints.removeAt(fbxJoints.length() - 1);

    // v   Success and exit
    qDebug() << "v   File was succesfully loaded : " + fileName;
    fbxMesh.traceInfo();
    return QString();
}


void TracePath (const QVector<QString> pathSt){
    return;
    QString res = "";
    for (int i = 0; i < pathSt.length(); i++)
        res += pathSt[i] + " : ";
    qDebug() << res;
}
IOfbx::ParseType IOfbx::pushHeader(const QString fromLine)
{
    int stackLengthWas = stackHeaders.length();

    QString lineTrimmed= fromLine.trimmed(),
            lineName = lineTrimmed.mid(0, lineTrimmed.indexOf(':'));
    if (lineTrimmed.indexOf(':') < lineTrimmed.indexOf(' ') && lineName.length() > 0 && lineTrimmed[lineTrimmed.length() - 1] == "{")
    //              space is later then : (not "asdasd aass:"), name is not null,           line ends with {
            stackHeaders << lineName;
    if (lineTrimmed == "}")
        stackHeaders.removeAt(stackHeaders.length() - 1);

    if (stackLengthWas != stackHeaders.length())
        TracePath(stackHeaders);
    //
    // _-_-_- Check -_-_-_-_
    //
    if (indexOfHeaders({"Geometry", "Vertices"}) > 0)
         return ParseType::FbxGeometryMeshVertices;
    if (indexOfHeaders({"Geometry", "PolygonVertexIndex"}) > 0)
        return ParseType::FbxGeometryMeshPolygonIndexes;

    if (indexOfHeaders({"Model","Properties70"}) > 0)
        return ParseType::FbxObjectModelLimbNodeProperty;
    if (indexOfHeaders({"Pose","PoseNode"}) > 0)
        return ParseType::FbxObjectPoseNodeID;
    if (indexOfHeaders({"Pose","PoseNode","Matrix"}) > 0)
        return ParseType::FbxObjectPoseNodeMatrix;
    return ParseType::  None;
}

int IOfbx::indexOfHeaders(const QVector<QString> tags)
{
    if (stackHeaders.length() < tags.length())
        return -1;

    // check for last tags are completely equals
    for (int i = stackHeaders.length() - tags.length(), j = 0; i < stackHeaders.length(); i++, j++)
        if (stackHeaders[i] != tags[j])
            return -1;

    return stackHeaders.length() - tags.length();
}

template<typename T>
QVector<T> IOfbx::FbxNode::parseFbxArray(QStringList S, QString &error)
{
    qDebug() << QString("o   Message: Called array parser");
    error = QString();
    QVector<T> readenNumbers = QVector<T>();

    if (S[0].indexOf("a: ") < 0)
        qDebug() << "+   Warning: array parser is using for non-array value.";

    for (int lineIndex = 0; lineIndex < S.length(); lineIndex++){
        S[lineIndex] = S[lineIndex].trimmed();                                  // remove tabs
        if (S[lineIndex].indexOf("a: ") == 0) S[lineIndex] = S[lineIndex].remove(0,3);  // remove a:
        if (S[lineIndex][S[lineIndex].length() - 1] == ',') S[lineIndex].remove(S[lineIndex].length() - 2, 1) ;

        QVector<T> fromLine = parseLastXValues<T>(S[lineIndex], -1, error);
        if (!error.isEmpty())
        {return readenNumbers;}
        for (int valInd = 0; valInd < fromLine.length(); valInd++)
            readenNumbers << fromLine[valInd];
    }
    qDebug() << "v   Success: array parser read "
                +QString::number( readenNumbers.length()) + " values;";
    return readenNumbers;
}

template<typename T>
QVector<T> IOfbx::FbxNode::parseLastXValues(const QString S, const int X, QString &error)
{
    QVector<T> readenNumbers = QVector<T>();
    error = "";
    if (S.indexOf(',') < 0)
    {error = "Line does not have any \",\""; return readenNumbers;}

    QStringList splitedNumbers = S.split(',');
    // case of incorrect format
    if (splitedNumbers.length() <= 0)
    {error = "Line " + S + " does not contain any T values!"; return readenNumbers;}
    if (splitedNumbers.length() < X)
    {error = "Line " + S + " does not contain at least "+X+" values! ( it only contains " + splitedNumbers.length() + " )"; return readenNumbers;}

    // okay, the line is splited, so it is time to parse
    bool numberParsedCorrectly = false;
    int count = X;
    if (count < 0) count = splitedNumbers.length();
    for (int numIndex = splitedNumbers.length() - count;
         numIndex < splitedNumbers.length();
         numIndex++)
    {
        // remove different tabs and spaces
        splitedNumbers[numIndex] = splitedNumbers[numIndex].trimmed();
        if (splitedNumbers[numIndex] != "")
        {
            const auto parsedValue = T(splitedNumbers[numIndex].toDouble(&numberParsedCorrectly));
            if (!numberParsedCorrectly)
            {error = "Can not parse a value from  \""
                     +splitedNumbers[numIndex] + "\"!" ; return readenNumbers;}
            readenNumbers << parsedValue;
        }
    }
    return readenNumbers;
}

void IOfbx::FbxNode::setNameAndID(const QString name, const QString ID)
{
    this->name = name;
    this->id = ID;
}

bool IOfbx::FbxNode::hasNameAndID() const
{
    return (!name.isEmpty() && !id.isEmpty());
}

void IOfbx::FbxNode::traceInfo() const
{
    qDebug() << "Name: " + name + ";   ID: " + id + ";";
}

void IOfbx::findIdAndNameInLine(const QString line, QString &id, QString &name, QString &subName)
{
    QString ableIdCharacters = "1234567890";
    if (line.indexOf("{") > line.indexOf(",")
            && line.indexOf(",") > line.indexOf(":")
            && line.indexOf(":") >= 0){
        QString canBeNodeId =
                line.mid(line.indexOf(":") + 2, line.indexOf(",") - line.indexOf(":") - 2);
        bool isTrueId = true;
        for (int i = 0; i < canBeNodeId.length(); i++)
            if (ableIdCharacters.indexOf( canBeNodeId[i] ) < 0)
                isTrueId = false;
        if (isTrueId){
            // set a true, beautiful ID
            id = canBeNodeId;
            //  Model: 1852460320, "Model::RightToe_End", "LimbNode" {
            QStringList s = line.split(',');
            if (s.length() >= 3){
                for (int i = 1; i < 3; i++){
                    s[s.length() - i] = s[s.length() - i].trimmed();
                }
                subName = s[s.length() - 1];
                subName.remove(0,1);
                subName.remove(subName.length() - 3, 3);
                name = s[s.length() - 2];
                name.remove(0,1);
                name.remove(name.length() - 1, 1);
                int X = 10;
            }
        }
    }
    //
}

IOfbx::FbxModelJoint::FbxModelJoint()
{
    localTranslation = QVector<double>();
    localRotation = QVector<double>();
    localScaling = QVector<double>();
}




IOfbx::FbxPoseNode::FbxPoseNode()
{
    transformMatrixArray = QVector<double>();
}

