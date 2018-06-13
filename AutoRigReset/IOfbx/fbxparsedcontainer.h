#ifndef FBXPARSEDCONTAINER_H
#define FBXPARSEDCONTAINER_H

#include "fbxconnection.h"
#include "fbxgeometrymesh.h"
#include "fbxmodeljoint.h"
#include "fbxposenode.h"
#include "fbxsubdeformercluster.h"

namespace IOfbx {

class FbxParsedContainer
{
public:
    FbxParsedContainer() = default;
    FbxParsedContainer(const FbxGeometryMesh &mesh,
                       const QVector<FbxModelJoint> &joints,
                       const QVector<FbxPoseNode> &posenodes,
                       const QVector<FbxSubDeformerCluster> &clusters,
                       const QVector<FbxConnection> &connections);
    ~FbxParsedContainer();

    const FbxGeometryMesh &getMesh() const;
    const QVector<FbxModelJoint> &getJoints() const;
    const QVector<FbxPoseNode> &getPosenodes() const;
    const QVector<FbxSubDeformerCluster> &getClusters() const;
    const QVector<FbxConnection> &getConnections() const;
    void traceInfo() const;

private:
    FbxGeometryMesh mesh;
    QVector<FbxModelJoint> joints;
    QVector<FbxPoseNode> posenodes;
    QVector<FbxSubDeformerCluster> clusters;
    QVector<FbxConnection> connections;
};

}

#endif // FBXPARSEDCONTAINER_H
