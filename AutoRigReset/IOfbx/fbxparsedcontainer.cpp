#include "fbxparsedcontainer.h"

IOfbx::FbxParsedContainer::FbxParsedContainer(const FbxGeometryMesh &mesh,
                                              const QVector<FbxModelJoint> &joints,
                                              const QVector<FbxPoseNode> &posenodes,
                                              const QVector<FbxSubDeformerCluster> &clusters,
                                              const QVector<FbxConnection> &connections)
{
    this->mesh = mesh;
    this->joints = joints;
    this->posenodes = posenodes;
    this->clusters = clusters;
    this->connections = connections;
}

IOfbx::FbxParsedContainer::~FbxParsedContainer()
{

}

const QVector<IOfbx::FbxConnection> &IOfbx::FbxParsedContainer::getConnections() const
{
    return connections;
}

void IOfbx::FbxParsedContainer::traceInfo() const
{
    mesh.traceInfo();
    traceMessage ( "!Loaded joints: " + QString::number(joints.length()));
    traceMessage ( "!Loaded posenodes: " +  QString::number(posenodes.length()));
    traceMessage ( "!Loaded clusters: " +  QString::number(clusters.length()));
    traceMessage ( "!Loaded connections: " +  QString::number(connections.length()));
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

const IOfbx::FbxGeometryMesh &IOfbx::FbxParsedContainer::getMesh() const
{
    return mesh;
}
