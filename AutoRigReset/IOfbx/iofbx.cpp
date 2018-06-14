#include "iofbx.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "IOfbx/fbxmessages.h"

namespace IOfbx {

const QVector<QString> parseBlockNames = {
    "Geometry Mesh Vertices",
    "Geometry Mesh Polygon Indexes",
    "Object Model Limb Node info",
    "Object Pose Node Id",
    "Object Pose Node Bind Matrix",
    "Connection",
    "???"


};
const QVector<QPair<QStringList, ParseType>> fbxBlockSignatures = {
    {{"Connections"}, ParseType::FbxConnection},
    {{"Geometry", "Vertices"}, ParseType::FbxGeometryMeshVertices},
    {{"Geometry", "PolygonVertexIndex"}, ParseType::FbxGeometryMeshPolygonIndexes},
    {{"Model", "Properties70"}, ParseType::FbxObjectModelLimbNodeProperty},
    {{"Pose","PoseNode"}, ParseType::FbxObjectPoseNodeID},
    {{"Pose", "PoseNode", "Matrix"}, ParseType::FbxObjectPoseNodeMatrix}
};



}

//  "Cluster info",
//  "Cluster Indexes",
//  "Cluster Weightes",
//  "Cluster Transform",
//  "Cluster Link Transform",

//    FbxObjectDeformerClusterIndexes = 6,
//    FbxObjectDeformerClusterWeightes = 7,
//    FbxObjectDeformerClusterTransform = 8,
//    FbxObjectDeformerClusterLinkTransform = 9

bool IOfbx::isStackContainsHeaders(const QStringList &tags, QVector<QString> &stackHeaders)
{
    if (stackHeaders.length() < tags.length())
        return false;

    // check for last tags are completely equals
    for (int i = stackHeaders.length() - tags.length(), j = 0; i < stackHeaders.length(); ++i, ++j)
        if (stackHeaders[i] != tags[j])
            return false;
    return true;
}

IOfbx::FbxParsedContainer *IOfbx::loadFromPath(const QString &path,  QString &error)
{
    QVector<QString> stackHeaders;
    const QString fileName = path.split("/").last();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)){
        error = QString(IOfbx::errMessageIncorrectFilePath + path);
        return nullptr;
    }

    QTextStream textStream(&file);

    if (textStream.atEnd()){
        error = IOfbx::errMessageFileEmpty + path;
        return nullptr;
    }

    // initialise a struct for save parsed data
    QVector<FbxGeometryMesh> fbxMeshes;   // a readen date of mesh
    QVector<FbxModelJoint> fbxJoints;    // a joints readen
    QVector<FbxPoseNode> fbxPoseNodes;
    QVector<FbxSubDeformerCluster> fbxClusters;
    QVector<FbxConnection> fbxConnections;

    // read a file line by line
    int lineNumber = 0;
    QString nodeId;
    QString nodeName;
    QString nodeSubName;
    QStringList nodeBuffer;
    ParseType lastParsingType = ParseType::None;

    while (!textStream.atEnd()){
        ++lineNumber;
        // adress of exception in case of fire
        error = QString("(line #%1)").arg(lineNumber);

        const QString line = textStream.readLine();
        const ParseType detectedTypeHeader = pushHeader(line, stackHeaders);

        // there is sometimes a need to remember an ID of node, you are currently parsing
        findIdAndNameInLine(line, nodeId, nodeName, nodeSubName);

        if (detectedTypeHeader != ParseType::None){
            // if it is first detected, then clear buffer and start copying
            if(lastParsingType == ParseType::None){
                nodeBuffer.clear();
            }
            // start to copy to buffer
            nodeBuffer.append(line);
        } else {
            // trace an info of name of parsed block
            const int lpt = (int)lastParsingType;
            if(lpt >= 0)
                traceMessage(
                            QString("v   Success: %1 buffer is copyed; It containts %2 lines;")
                            .arg((lpt < parseBlockNames.length()) ? parseBlockNames[lpt] : "???")
                            .arg(nodeBuffer.length()));

            QString err = selectParserForBuffer(nodeId, nodeName, nodeSubName,
                                                lastParsingType, nodeBuffer,
                                                fbxMeshes, fbxJoints, fbxPoseNodes, fbxClusters, fbxConnections);
            if (!err.isEmpty()){
                error += err;
                return nullptr;
            }
            if (!nodeBuffer.isEmpty())
                nodeBuffer.clear();
        }

        lastParsingType = detectedTypeHeader;
    }

    error = QString();
    if (fbxClusters.length() > 0 && fbxClusters[0].isEmpty())
        fbxClusters.remove(0,1);

    if (fbxJoints.isEmpty()
            && fbxPoseNodes.isEmpty()
            && fbxClusters.isEmpty()
            && fbxConnections.isEmpty()){
        if (!fbxMeshes.isEmpty() && fbxMeshes.first().hasNameAndID()){
            error = IOfbx::errMessageOnlyMesh;
            return nullptr;
        }else{
            error = IOfbx::errMessageNoRigs;
            return nullptr;
        }
    }

    // v   Success and exit
    traceMessage ( "!v   File was succesfully loaded : " + fileName);
    error = "";

    return new FbxParsedContainer(fbxMeshes, fbxJoints, fbxPoseNodes, fbxClusters, fbxConnections);
}


QString IOfbx::selectParserForBuffer(const QString &nodeId,
                                     const QString &nodeName,
                                     const QString &nodeSubName,
                                     const ParseType &lastParsingType,
                                     const QStringList &nodeBuffer,
                                     QVector<FbxGeometryMesh> &fbxMeshes,
                                     QVector<FbxModelJoint> &fbxJoints,
                                     QVector<FbxPoseNode> &fbxPoseNodes,
                                     QVector<FbxSubDeformerCluster> &fbxClusters,
                                     QVector<FbxConnection> &fbxConnections)
{
    QString conLine;
    QStringList maybeConnection;
    QStringList extendedBuffer;
    QString err = QString();
    switch (lastParsingType){

    case ParseType::FbxGeometryMeshVertices:
        // first mesh, or making another mesh, when prevous if fully complete
        if (fbxMeshes.isEmpty() || fbxMeshes.last().isComplete())
            fbxMeshes << FbxGeometryMesh();
        if (!fbxMeshes.last().hasNameAndID())
            fbxMeshes.last().setNameAndID(nodeName, nodeId);
        extendedBuffer = nodeBuffer;
        extendedBuffer.insert(0, QString("vertices"));
        err = fbxMeshes.last().parse(extendedBuffer);
        if (!err.isEmpty())
            return  IOfbx::errMessageMeshVert + err;
        break;

    case ParseType::FbxGeometryMeshPolygonIndexes:
        extendedBuffer = nodeBuffer;
        extendedBuffer.insert(0, QString("polygons"));
        err = fbxMeshes.last().parse(extendedBuffer);
        if (!err.isEmpty())
            return IOfbx::errMessageMeshPolygons + err;
        // if mesh is complete than do nothing ^^
        break;

    case ParseType::FbxObjectModelLimbNodeProperty:
        fbxJoints << FbxModelJoint();
        if (!fbxJoints.last().hasNameAndID())
            fbxJoints.last().setNameAndID(nodeName, nodeId);
        err = fbxJoints.last().parse(nodeBuffer);
        fbxJoints.last().addSubName(nodeSubName);
        if (!err.isEmpty())
            return IOfbx::errMessageJoint + err;
        break;

    case ParseType::FbxObjectPoseNodeID:
        fbxPoseNodes << FbxPoseNode();
        err = fbxPoseNodes.last().parse(nodeBuffer);
        if (!err.isEmpty())
            return IOfbx::errMessagePoseNode + err;
        break;

    case ParseType::FbxObjectDeformerCluster:
        fbxClusters << FbxSubDeformerCluster();
        if (!fbxClusters.last().hasNameAndID())
            fbxClusters.last().setNameAndID(nodeName, nodeId);
        err = fbxClusters.last().parse(nodeBuffer);

        if (!err.isEmpty())
            return IOfbx::errMessageCluster + err;
        break;

    case ParseType::FbxConnection:
        maybeConnection = QStringList();
        for(int lineInd = 1; lineInd < nodeBuffer.length(); ++lineInd){
            conLine = nodeBuffer[lineInd].trimmed();
            if (!conLine.isEmpty())
                maybeConnection << conLine;
            if (maybeConnection.length() == 2){
                fbxConnections.append(FbxConnection());
                err = fbxConnections.last().parse(maybeConnection);
                if (!err.isEmpty())
                    return IOfbx::errMessageConnection + err;

                maybeConnection.clear();
            }
        }
        break;
    default:
        break;
    }
    return QString();
}

void tracePath(const QVector<QString> &pathSt)
{
    return;
    QString res = "";
    for (int i = 0; i < pathSt.length(); ++i)
        res += pathSt[i] + " : ";
    IOfbx::traceMessage(res);
}

IOfbx::ParseType IOfbx::pushHeader(const QString fromLine, QVector<QString> &stackHeaders)
{
    const int stackLengthWas = stackHeaders.length();

    const QString lineTrimmed = fromLine.trimmed();
    const QString lineName = lineTrimmed.mid(0, lineTrimmed.indexOf(':'));

    const bool isNameNotNull = !lineName.isEmpty();
    const bool isLineEndsWithBracket = isNameNotNull && lineTrimmed[lineTrimmed.length() - 1] == "{";
    const bool isSpaceBeforeTwoPoints = lineTrimmed.indexOf(':') < lineTrimmed.indexOf(' ');
    const bool isLineABlockOpener = isSpaceBeforeTwoPoints && isLineEndsWithBracket;
    if(isLineABlockOpener)
        stackHeaders << lineName;
    if(lineTrimmed == "}")
        stackHeaders.removeAt(stackHeaders.length() - 1);

    if(stackLengthWas != stackHeaders.length())
        tracePath(stackHeaders);

    for(const auto iter : IOfbx::fbxBlockSignatures){
        if(isStackContainsHeaders(iter.first, stackHeaders))
            return iter.second;
    }
    bool isStackMiddleIsDeformer =
            (stackHeaders.length() >= 2 && stackHeaders[stackHeaders.length() - 2] == "Deformer");
    bool isStackLastsADeformer = isStackContainsHeaders({"Deformer"}, stackHeaders);

    if (isStackLastsADeformer || isStackMiddleIsDeformer)
        return ParseType::FbxObjectDeformerCluster;

    return ParseType::None;
}


void IOfbx::findIdAndNameInLine(const QString line, QString &id, QString &name, QString &subName)
{
    const QString ableIdCharacters = "0123456789";

    const bool isColonExist = line.indexOf(":") >= 0;
    const bool isCommaEarlierThanColon = isColonExist && (line.indexOf(",") > line.indexOf(":"));
    const bool isBracketEarlierThanComma = isCommaEarlierThanColon && line.indexOf("{") > line.indexOf(",");

    if(!isBracketEarlierThanComma)
        return;

    const QString linePartForId = line.mid(line.indexOf(":") + 2, line.indexOf(",") - line.indexOf(":") - 2);
    bool isIdContainsOnlyAbailableCharacter = true;
    for (int i = 0; i < linePartForId.length(); ++i)
        if (ableIdCharacters.indexOf(linePartForId[i]) < 0)
            isIdContainsOnlyAbailableCharacter = false;

    if (!isIdContainsOnlyAbailableCharacter)
        return;

    id = linePartForId;
    //  Model: 1852460320, "Model::RightToe_End", "LimbNode" {
    QStringList s = line.split(',');
    if(s.length() == 2){
        s[1].remove(s[1].length() - 1, 1);  // remove {
        s[1] = s[1].trimmed();
        name = s[1];
        name.remove(0,1);
        name.remove(name.length() - 1, 1);
        return;
    }

    if (s.length() < 3)
        return;

    for (int i = 1; i < 3; ++i)
        s[s.length() - i] = s[s.length() - i].trimmed();    // trim all parts

    subName = s[s.length() - 1];
    subName.remove(0,1);
    subName.remove(subName.length() - 1, 1);    // {
    subName = subName.trimmed();                // _ _ _
    subName.remove(subName.length() - 1, 1);    // "
    name = s[s.length() - 2];
    name.remove(0,1);
    name.remove(name.length() - 1, 1);
}
