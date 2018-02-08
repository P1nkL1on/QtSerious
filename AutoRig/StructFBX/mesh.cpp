#include "mesh.h"
#include "Derivable/dermatops.h"

using namespace DerivableVectorMatrixes;
using Eigen::Matrix;
using namespace DerOperations;

Mesh::Mesh()
{
    vertexes = QVector<Matrix<Derivable,1,3>>();
    polygonIndexes = QVector<int>();
    polygonStartIndexes = QVector<int>();
}

Derivable Mesh::CompareWithAnotherMesh(const Mesh *anotherMesh) const
{
    Q_ASSERT(vertexes.length() == anotherMesh->vertexes.length());
    Derivable summ = Derivable(0);
    for (int vInd = 0; vInd < vertexes.length(); vInd++)
        summ = summ + SquaredLength(vertexes[vInd] - anotherMesh->vertexes[vInd]) / 100000.0;

    return summ;
}
QVector<Derivable> Mesh::CompareWithAnotherMeshCoords(const Mesh *anotherMesh) const
{
    Q_ASSERT(vertexes.length() == anotherMesh->vertexes.length());
    QVector<Derivable> res = QVector<Derivable>();
    for (int vInd = 0; vInd < vertexes.length(); vInd++){
        //summ = summ + SquaredLength(vertexes[vInd] - anotherMesh->vertexes[vInd]) / 100000.0;
        Matrix<Derivable,1,3> dist = vertexes[vInd] - anotherMesh->vertexes[vInd];
        res << dist(0,0) << dist(0,1) << dist(0,2);
    }
    return res;
}
