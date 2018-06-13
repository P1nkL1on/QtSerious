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


//  "Cluster info",
//  "Cluster Indexes",
//  "Cluster Weightes",
//  "Cluster Transform",
//  "Cluster Link Transform",
}

//    FbxObjectDeformerClusterIndexes = 6,
//    FbxObjectDeformerClusterWeightes = 7,
//    FbxObjectDeformerClusterTransform = 8,
//    FbxObjectDeformerClusterLinkTransform = 9



int IOfbx::indexOfHeaders(const QVector<QString> tags, QVector<QString> &stackHeaders)
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
        error = QString("The path of readable file is incorrect : " + path);
        return nullptr;
    }

    QTextStream textStream(&file);

    if (textStream.atEnd()){
        error = "The file is empty : " + path;
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
#warning Rewrite with .arg()
            if(lpt >= 0)
                traceMessage( "v   Success: "
                              + ((lpt < parseBlockNames.length())? parseBlockNames[lpt] : "???")
                              + " buffer ( " + QString::number(nodeBuffer.length()) + " lines ) read;");

            // then just finished to
            QString err = QString();
            switch (lastParsingType){
            case ParseType::FbxGeometryMeshVertices:
                if (!fbxMesh.hasNameAndID())
                    fbxMesh.setNameAndID(nodeName, nodeId);
                // call a parser of mesh
                err = fbxMesh.parse(nodeBuffer, 0);
                if (!err.isEmpty()){
                    error += "x   Error in parsing a mesh vertices: " + err;
                    return nullptr;
                }
                break;
            case ParseType::FbxGeometryMeshPolygonIndexes:
                err = fbxMesh.parse(nodeBuffer,1);
                if (!err.isEmpty()){
                    error += "x   Error in parsing a mesh polygon idnexes: " + err;
                    return nullptr;
                }
                break;
            case ParseType::FbxObjectModelLimbNodeProperty:
                if (!fbxJoints.last().hasNameAndID())
                    fbxJoints.last().setNameAndID(nodeName, nodeId);
                err = fbxJoints.last().parse(nodeBuffer, 0);
                fbxJoints.last().addSubName(nodeSubName);

                if (!err.isEmpty()){
                    error += "x   Error in parsing a joint: " + err;
                    return nullptr;
                }
                fbxJoints.append(FbxModelJoint());
                break;
            case ParseType::FbxObjectPoseNodeID:
                err = fbxPoseNodes.last().parse(nodeBuffer, 0);
                if (!err.isEmpty()){
                    error += "x   Error in parsing a posenode: " + err;
                    return nullptr;
                }
                fbxPoseNodes.append(FbxPoseNode());
                break;
            case ParseType::FbxObjectDeformerCluster:
                if (!fbxClusters.last().hasNameAndID())
                    fbxClusters.last().setNameAndID(nodeName, nodeId);
                err = fbxClusters.last().parse(nodeBuffer, 0);

                if (!err.isEmpty()){
                    error += "x   Error in parsing a subdeformer cluster: " + err;
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
                        {error += "x   Error in parsing connection: " + err; return nullptr;}
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
            error = "x   File contains only mesh!";
            return nullptr;
        }else{
            error = "x   File does not contain any rigs!";
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

    if (indexOfHeaders({"Connections"}, stackHeaders) >= 0)
        return ParseType::FbxConnection;
    if (indexOfHeaders({"Geometry", "Vertices"}, stackHeaders) > 0)
        return ParseType::FbxGeometryMeshVertices;
    if (indexOfHeaders({"Geometry", "PolygonVertexIndex"}, stackHeaders) > 0)
        return ParseType::FbxGeometryMeshPolygonIndexes;

    if (indexOfHeaders({"Model", "Properties70"}, stackHeaders) > 0)
        return ParseType::FbxObjectModelLimbNodeProperty;

    if (indexOfHeaders({"Pose","PoseNode"}, stackHeaders) > 0)
        return ParseType::FbxObjectPoseNodeID;
    if (indexOfHeaders({"Pose", "PoseNode", "Matrix"}, stackHeaders) > 0)
        return ParseType::FbxObjectPoseNodeMatrix;

#warning make line shorter
    if (indexOfHeaders({"Deformer"}, stackHeaders) > 0
            || (stackHeaders.length() >= 2 && stackHeaders[stackHeaders.length() - 2] == "Deformer"))
        return ParseType::FbxObjectDeformerCluster;

    return ParseType::None;
}

#warning make tests
void IOfbx::findIdAndNameInLine(const QString line, QString &id, QString &name, QString &subName)
{
    const QString ableIdCharacters = "1234567890";

#warning Rename it
    const bool isSomeBooleanVal = line.indexOf("{") > line.indexOf(",")
            && line.indexOf(",") > line.indexOf(":")
            && line.indexOf(":") >= 0;
    if(!isSomeBooleanVal)
        return;

#warning Rename it
    const QString canBeNodeId = line.mid(line.indexOf(":") + 2, line.indexOf(",") - line.indexOf(":") - 2);
    bool isTrueId = true;
    for (int i = 0; i < canBeNodeId.length(); i++)
        if (ableIdCharacters.indexOf(canBeNodeId[i]) < 0)
            isTrueId = false;
    if (!isTrueId)
        return;

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
    }
}
