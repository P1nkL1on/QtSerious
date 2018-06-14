#include "iofbx.h"

#include <QFile>
#include "QTextStream"
#include "QDebug"


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

const QString errMessageIncorrectFilePath = "The path of readable file is incorrect : ";
const QString errMessageFileEmpty       = "The file is empty : ";
const QString errMessageMeshVert        = "x   Error in parsing a mesh vertices: ";
const QString errMessageMeshPolygons    = "x   Error in parsing a mesh polygon idnexes: ";
const QString errMessageJoint           = "x   Error in parsing a joint: ";
const QString errMessagePoseNode        = "x   Error in parsing a posenode: ";
const QString errMessageCluster         = "x   Error in parsing a subdeformer cluster: ";
const QString errMessageConnection      = "x   Error in parsing connection: ";
const QString errMessageOnlyMesh        = "x   File contains only mesh!";
const QString errMessageNoRigs          = "x   File does not contain any rigs!";


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

int IOfbx::indexOfHeaders(const QStringList &tags, QVector<QString> &stackHeaders)
{
    if (stackHeaders.length() < tags.length())
        return -1;

    // check for last tags are completely equals
    for (int i = stackHeaders.length() - tags.length(), j = 0; i < stackHeaders.length(); i++, j++)
        if (stackHeaders[i] != tags[j])
            return -1;

    return stackHeaders.length() - tags.length();
}

IOfbx::FbxParsedContainer *IOfbx::loadFromPath(const QString &path,  QString &error)
{
    QVector<QString> stackHeaders;
    const QString fileName = path.split("/").last();

#warning create constant strings for errors

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
    FbxGeometryMesh fbxMesh;   // a readen date of mesh
    QVector<FbxModelJoint> fbxJoints;    // a joints readen
    QVector<FbxPoseNode> fbxPoseNodes;
    QVector<FbxSubDeformerCluster> fbxClusters;
    QVector<FbxConnection> fbxConnections;

    fbxJoints.append(FbxModelJoint());
    fbxPoseNodes.append(FbxPoseNode());
    fbxClusters.append(FbxSubDeformerCluster());

    // read a file line by line
    int lineNumber = 0;
    QString conLine;
    QString nodeId;
    QString nodeName;
    QString nodeSubName;
    QStringList nodeBuffer;
    QStringList maybeConnection;
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
            if (lastParsingType == ParseType::None){
                nodeBuffer.clear();
            }
            // start to copy to buffer
            nodeBuffer.append(line);
        }else{
            // trace an info of name of parsed block
            const int lpt = (int)lastParsingType;
            if(lpt >= 0)
                traceMessage(
                            QString("v   Success: %1 buffer is copyed; It containts #2 lines;")
                            .arg((lpt < parseBlockNames.length())? parseBlockNames[lpt] : "???", nodeBuffer.length()));

            // then just finished to
            QString err = QString();
            switch (lastParsingType){
            case ParseType::FbxGeometryMeshVertices:
                if (!fbxMesh.hasNameAndID())
                    fbxMesh.setNameAndID(nodeName, nodeId);
                // call a parser of mesh
                err = fbxMesh.parse(nodeBuffer, 0);
                if (!err.isEmpty()){
                    error += IOfbx::errMessageMeshVert + err;
                    return nullptr;
                }
                break;
            case ParseType::FbxGeometryMeshPolygonIndexes:
                err = fbxMesh.parse(nodeBuffer,1);
                if (!err.isEmpty()){
                    error += IOfbx::errMessageMeshPolygons + err;
                    return nullptr;
                }
                break;
            case ParseType::FbxObjectModelLimbNodeProperty:
                if (!fbxJoints.last().hasNameAndID())
                    fbxJoints.last().setNameAndID(nodeName, nodeId);
                err = fbxJoints.last().parse(nodeBuffer, 0);
                fbxJoints.last().addSubName(nodeSubName);

                if (!err.isEmpty()){
                    error += IOfbx::errMessageJoint + err;
                    return nullptr;
                }
                fbxJoints.append(FbxModelJoint());
                break;
            case ParseType::FbxObjectPoseNodeID:
                err = fbxPoseNodes.last().parse(nodeBuffer, 0);
                if (!err.isEmpty()){
                    error += IOfbx::errMessagePoseNode + err;
                    return nullptr;
                }
                fbxPoseNodes.append(FbxPoseNode());
                break;
            case ParseType::FbxObjectDeformerCluster:
                if (!fbxClusters.last().hasNameAndID())
                    fbxClusters.last().setNameAndID(nodeName, nodeId);
                err = fbxClusters.last().parse(nodeBuffer, 0);

                if (!err.isEmpty()){
                    error += IOfbx::errMessageCluster + err;
                    return nullptr;
                }
                fbxClusters.append(FbxSubDeformerCluster());
                break;
            case ParseType::FbxConnection:
                maybeConnection = QStringList();
                for (int lineInd = 1; lineInd < nodeBuffer.length(); lineInd ++){
                    conLine = nodeBuffer[lineInd].trimmed();
                    if (!conLine.isEmpty())
                        maybeConnection << conLine;
                    if (maybeConnection.length() == 2){
                        fbxConnections.append(FbxConnection());
                        err = fbxConnections.last().parse(maybeConnection, 0);
                        if (!err.isEmpty())
                        {error += IOfbx::errMessageConnection + err; return nullptr;}
                        maybeConnection.clear();
                    }
                }

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
    error = "";
    fbxJoints.removeAt(fbxJoints.length() - 1);
    fbxPoseNodes.removeAt(fbxPoseNodes.length() - 1);
    fbxClusters.removeAt(fbxClusters.length() - 1);
    if (fbxClusters.length() > 0 && fbxClusters[0].isEmpty())
        fbxClusters.remove(0,1);

    if (fbxJoints.length() == 0 && fbxPoseNodes.length() == 0 && fbxClusters.length() == 0 && fbxConnections.length() == 0){
        if (fbxMesh.hasNameAndID()){
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

    return new FbxParsedContainer(fbxMesh, fbxJoints, fbxPoseNodes, fbxClusters, fbxConnections);
}


void TracePath (const QVector<QString> pathSt){
    return;
    QString res = "";
    for (int i = 0; i < pathSt.length(); i++)
        res += pathSt[i] + " : ";
    IOfbx::traceMessage (res);
}
IOfbx::ParseType IOfbx::pushHeader(const QString fromLine, QVector<QString> &stackHeaders)
{
    int stackLengthWas = stackHeaders.length();

    const QString lineTrimmed = fromLine.trimmed();
    const QString lineName = lineTrimmed.mid(0, lineTrimmed.indexOf(':'));

    const bool isNameNotNull = !lineName.isEmpty();
    const bool isLineEndsWithBracket = isNameNotNull && lineTrimmed[lineTrimmed.length() - 1] == "{";
    const bool isSpaceBeforeTwoPoints = lineTrimmed.indexOf(':') < lineTrimmed.indexOf(' ');
    const bool isLineABlockOpener = isSpaceBeforeTwoPoints && isLineEndsWithBracket;
    if(isLineABlockOpener)
        stackHeaders << lineName;
    if (lineTrimmed == "}")
        stackHeaders.removeAt(stackHeaders.length() - 1);

    if (stackLengthWas != stackHeaders.length())
        TracePath(stackHeaders);

    for(const auto iter : IOfbx::fbxBlockSignatures){
        if(indexOfHeaders(iter.first, stackHeaders) >= 0)
            return iter.second;
    }
    bool isStackMiddleIsDeformer
            = (stackHeaders.length() >= 2 && stackHeaders[stackHeaders.length() - 2] == "Deformer");
    bool isStackLastsADeformer = indexOfHeaders({"Deformer"}, stackHeaders) > 0;

    if (isStackLastsADeformer || isStackMiddleIsDeformer)
        return ParseType::FbxObjectDeformerCluster;

    return ParseType::None;
}

#warning make tests
void IOfbx::findIdAndNameInLine(const QString line, QString &id, QString &name, QString &subName)
{
    const QString ableIdCharacters = "1234567890";

    const bool isColonExist = line.indexOf(":") >= 0;
    const bool isCommaEarlierThanColon = isColonExist && (line.indexOf(",") > line.indexOf(":"));
    const bool isBracketEarlierThanComma = isCommaEarlierThanColon && line.indexOf("{") > line.indexOf(",");

    if(!isBracketEarlierThanComma)
        return;

    const QString linePartForId = line.mid(line.indexOf(":") + 2, line.indexOf(",") - line.indexOf(":") - 2);
    bool isIdContainsOnlyAbailableCharacter = true;
    for (int i = 0; i < linePartForId.length(); i++)
        if (ableIdCharacters.indexOf(linePartForId[i]) < 0)
            isIdContainsOnlyAbailableCharacter = false;

    if (!isIdContainsOnlyAbailableCharacter)
        return;

    id = linePartForId;
    //  Model: 1852460320, "Model::RightToe_End", "LimbNode" {
    QStringList s = line.split(',');
    if (s.length() == 2){
        s[1].remove(s[1].length() - 1, 1);  // remove {
        s[1] = s[1].trimmed();
        name = s[1];
        name.remove(0,1);
        name.remove(name.length() - 1, 1);
        return;
    }
    if (s.length() >= 3){
        for (int i = 1; i < 3; i++)
            s[s.length() - i] = s[s.length() - i].trimmed();    // trim all parts

        subName = s[s.length() - 1];
        subName.remove(0,1);
        subName.remove(subName.length() - 1, 1);    // {
        subName = subName.trimmed();                // _ _ _
        subName.remove(subName.length() - 1, 1);    // "
        name = s[s.length() - 2];
        name.remove(0,1);
        name.remove(name.length() - 1, 1);
        return;
    }
}
