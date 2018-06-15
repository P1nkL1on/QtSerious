#ifndef FBXSTRUCT_CLUSTER_H
#define FBXSTRUCT_CLUSTER_H

#include "DerStruct/defines.h"

namespace FbxStruct{

class Cluster
{
public:
    Cluster() = default;
    Cluster(const QVector<int> &vertIndexes,
            const QVector<double> &weightes,
            const Df::Matrix4<double> &transformMatrix,
            const Df::Matrix4<double> &linkTransformMatrix);
    int getMeshIndex() const;
    void setMeshIndex(const int value);
    int getJointIndex() const;
    void setJointIndex(const int value);

private:
    int meshIndex = -1;
    int jointIndex = -1;
    QVector<int> vertIndexes;
    QVector<double> weightes;
    Df::Matrix4<double> transformMatrix = Df::Matrix4<double>::Identity();
    Df::Matrix4<double> linkTransformMatrix = Df::Matrix4<double>::Identity();

};
}
#endif // FBXSTRUCT_CLUSTER_H
