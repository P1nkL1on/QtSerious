#ifndef FBXSTRUCT_MESH_H
#define FBXSTRUCT_MESH_H

#include "DerStruct/defines.h"

namespace FbxStruct{

template <typename Scalar>
class Mesh
{
public:
    Mesh ();

private:
    QVector<Df::Vector3<Scalar>> vertices;
    QVector<int> polygonIndexes;
    QVector<int> polygonStartIndexes;
};

}

#endif // FBXSTRUCT_MESH_H
