#ifndef IOFBX_IOFBX_H
#define IOFBX_IOFBX_H

#include "fbxparsedcontainer.h"

namespace IOfbx {

enum class ParseType
{
    None = -1,
    FbxGeometryMeshVertices = 0,
    FbxGeometryMeshPolygonIndexes = 1,
    FbxObjectModelLimbNodeProperty = 2,
    FbxObjectPoseNodeID = 3,
    FbxObjectPoseNodeMatrix = 4,
    FbxObjectDeformerCluster = 5,
    FbxConnection = 6,
};

ParseType pushHeader(const QString fromLine, QVector<QString> &stackHeaders);
bool isStackContainsHeaders(const QStringList &tags, QVector<QString> &stackHeaders);

// the method of loading
FbxParsedContainer *loadFromPath(const QString &path, QString &error);
void findIdAndNameInLine(const QString line, QString& id, QString& name, QString &subName);
QString selectParserForBuffer(const QString &nodeId,
                              const QString &nodeName,
                              const QString &nodeSubName,
                              const ParseType &lastParsingType,
                              const QStringList &nodeBuffer,
                              QVector<FbxGeometryMesh> &fbxMeshes,
                              QVector<FbxModelJoint> &fbxJoints,
                              QVector<FbxPoseNode> &fbxPoseNodes,
                              QVector<FbxSubDeformerCluster> &fbxClusters,
                              QVector<FbxConnection> &fbxConnections);

}

#endif // IOFBX_H
