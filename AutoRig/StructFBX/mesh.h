#ifndef MESH_H
#define MESH_H
#include "QVector"
#include "Eigen/Core"
#include "derivable.h"


class Mesh
{
public:
    Mesh();
    QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> vertexes;
    QVector<int> polygonIndexes;
    QVector<int> polygonStartIndexes;

    DerOperations::Derivable CompareWithAnotherMesh (const Mesh* anotherMesh) const ;
    QVector<DerOperations::Derivable> CompareWithAnotherMeshCoords(const Mesh *anotherMesh) const;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif // MESH_H
