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

    // read a file line by line
    QString line = "";
    QString nodeId = "noneID";
    QString nodeName = "unknown";
    QStringList nodeBuffer = QStringList();
    ParseType lastParsingType = ParseType::None;

    while (!textStream.atEnd()){
        line = textStream.readLine();
        ParseType detectedTypeHeader = pushHeader(line);

        // there is sometimes a need to remember an ID of node, you are currently parsing
        findIdAndNameInLine(line, nodeId, nodeName);

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
                        err = // [i].Parse()
                                //  << to arr
                                // << some new :3
                                // remember to clean last zero on end
                        break;
                    default:
                        break;
                }
            if (!nodeBuffer.isEmpty())
                nodeBuffer.clear();
        }

        lastParsingType = detectedTypeHeader;
    }

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

    if (indexOfHeaders({"Model","Properties70"}) > 0/* && fromLine.indexOf("LimbNode") > 0*/)
        return ParseType::FbxObjectModelLimbNodeProperty;

    return ParseType::None;
}

int IOfbx::indexOfHeaders(const QVector<QString> tags)
{
    if (stackHeaders.length() < tags.length())
        return -1;

    // check for last tags are completely equals
    for (int i = stackHeaders.length() - tags.length(), j = 0; i < stackHeaders.length(); i++, j++)
        if (stackHeaders[i] != tags[j])
            return -1;
//    QString res = "";
//    for (int i = 0; i < stackHeaders.length(); i++)
//        res += stackHeaders[i] + " _ ";

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
        S[lineIndex] = S[lineIndex].trimmed();                                  // rmove tabs
        if (S[lineIndex].indexOf("a: ") == 0) S[lineIndex] = S[lineIndex].remove(0,3);  // remove a:
        if (S[lineIndex][S[lineIndex].length() - 1] == ',') S[lineIndex].remove(S[lineIndex].length() - 2, 1) ;

        QStringList splitedNumbers = S[lineIndex].split(',');
        // case of incorrect format
        if (splitedNumbers.length() <= 0)
        {error = "Line " + S[lineIndex] + " does not contain any T values!"; return readenNumbers;}
        // okay, the line is splited, so it is time to parse
        bool numberParsedCorrectly = false;
        for (int numIndex = 0; numIndex < splitedNumbers.length(); numIndex++){
            // remove different tabs and spaces
            splitedNumbers[numIndex] = splitedNumbers[numIndex].trimmed();
            if (splitedNumbers[numIndex] != "")
            {
                const auto parsedValue = T(splitedNumbers[numIndex].toDouble(&numberParsedCorrectly));
                if (!numberParsedCorrectly)
                {error = "Can not parse a value from  \"" +splitedNumbers[numIndex] + "\"!" ; return readenNumbers;}
                readenNumbers << parsedValue;
            }
        }
    }
    qDebug() << "v   Success: array parser read " +QString::number( readenNumbers.length()) + " values;";
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

void IOfbx::findIdAndNameInLine(const QString line, QString &id, QString &name)
{
    QString ableIdCharacters = "1234567890";
    if (line.indexOf("{") > line.indexOf(",") && line.indexOf(",") > line.indexOf(":") && line.indexOf(":") >= 0){
        QString canBeNodeId = line.mid(line.indexOf(":") + 2, line.indexOf(",") - line.indexOf(":") - 2);
        bool isTrueId = true;
        for (int i = 0; i < canBeNodeId.length(); i++)
            if (ableIdCharacters.indexOf( canBeNodeId[i] ) < 0)
                isTrueId = false;
        if (isTrueId){
            // set a true, beautiful ID
            id = canBeNodeId;
            // also try to get a name
            int firstBracInd = line.indexOf('\"'), secondBracInd = -1;
            // get a name betw brackets
            if (firstBracInd >= 0){
                secondBracInd = line.indexOf('\"', firstBracInd+1);
                if (secondBracInd > 0)
                    name = line.mid(firstBracInd, secondBracInd-firstBracInd);
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
