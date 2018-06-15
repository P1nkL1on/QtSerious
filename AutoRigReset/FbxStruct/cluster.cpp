#include "cluster.h"

using namespace FbxStruct;
using namespace Df;

Cluster::Cluster(const QVector<int> &vertIndexes,
                 const QVector<double> &weightes,
                 const Df::Matrix4<double> &transformMatrix,
                 const Df::Matrix4<double> &linkTransformMatrix):
    vertIndexes(vertIndexes),
    weights(weightes),
    transformMatrix(transformMatrix),
    linkTransformMatrix(linkTransformMatrix)
{

}

int Cluster::getMeshIndex() const
{
    return meshIndex;
}

void Cluster::setMeshIndex(const int value)
{
    meshIndex = value;
}
