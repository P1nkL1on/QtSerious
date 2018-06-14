#include "fbxparsedcontainer.h"

IOfbx::FbxParsedContainer::FbxParsedContainer(const QVector<FbxGeometryMesh> &mesh,
                                              const QVector<FbxModelJoint> &joints,
                                              const QVector<FbxPoseNode> &posenodes,
                                              const QVector<FbxSubDeformerCluster> &clusters,
                                              const QVector<FbxConnection> &connections):
    meshes(mesh),
    joints(joints),
    posenodes(posenodes),
    clusters(clusters),
    connections(connections)
{        

}

IOfbx::FbxParsedContainer::~FbxParsedContainer()
{

}

void IOfbx::FbxParsedContainer::traceInfo() const
{
    for (int ind = 0; ind < meshes.length(); ++ind)
        meshes[ind].traceInfo();
    traceMessage( "!Loaded joints: " + QString::number(joints.length()));
    traceMessage( "!Loaded posenodes: " +  QString::number(posenodes.length()));
    traceMessage( "!Loaded clusters: " +  QString::number(clusters.length()));
    traceMessage( "!Loaded connections: " +  QString::number(connections.length()));
}

const QVector<IOfbx::FbxConnection> &IOfbx::FbxParsedContainer::getConnections() const
{
    return connections;
}

const QVector<IOfbx::FbxSubDeformerCluster> &IOfbx::FbxParsedContainer::getClusters() const
{
    return clusters;
}

const QVector<IOfbx::FbxPoseNode> &IOfbx::FbxParsedContainer::getPosenodes() const
{
    return posenodes;
}

const QVector<IOfbx::FbxModelJoint> &IOfbx::FbxParsedContainer::getJoints() const
{
    return joints;
}

const QVector<IOfbx::FbxGeometryMesh> &IOfbx::FbxParsedContainer::getMeshes() const
{
    return meshes;
}
