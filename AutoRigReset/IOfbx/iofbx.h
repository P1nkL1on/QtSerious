#ifndef IOFBX_IOFBX_H
#define IOFBX_IOFBX_H

#include "fbxconnection.h"
#include "fbxgeometrymesh.h"
#include "fbxmodeljoint.h"
#include "fbxposenode.h"
#include "fbxsubdeformercluster.h"

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

class FbxParsedContainer
{
public:
    FbxParsedContainer();
    FbxParsedContainer(FbxGeometryMesh mesh,
                       QVector<FbxModelJoint> joints,
                       QVector<FbxPoseNode> posenodes,
                       QVector<FbxSubDeformerCluster> clusters,
                       QVector<FbxConnection> connections);
    ~FbxParsedContainer();

    FbxGeometryMesh getMesh() const;
    QVector<FbxModelJoint> getJoints() const;
    QVector<FbxPoseNode> getPosenodes() const;
    QVector<FbxSubDeformerCluster> getClusters() const;
    QVector<FbxConnection> getConnections() const;
    void traceInfo() const;

private:
    FbxGeometryMesh mesh;
    QVector<FbxModelJoint> joints;
    QVector<FbxPoseNode> posenodes;
    QVector<FbxSubDeformerCluster> clusters;
    QVector<FbxConnection> connections;
};

// _-_-_-_-_-_-_-_-_-_-PARSER-_-_-_-_-_-_-_-_-_-_-_-_


static QVector<QString> stackHeaders = QVector<QString>();
ParseType pushHeader(const QString fromLine);
int indexOfHeaders (const QVector<QString> tags);

// the method of loading
FbxParsedContainer *loadFromPath(const QString path, QString &error);
void findIdAndNameInLine (const QString line, QString& id, QString& name, QString &subName);

}

#endif // IOFBX_H
