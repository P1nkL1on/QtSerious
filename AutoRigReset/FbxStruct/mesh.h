#ifndef MESH_H
#define MESH_H

#include "fbxstruct.h"
#include "iofbx.h"

namespace FbxStruct{

template <typename Scalar>
class Mesh
{
public:
    Mesh ();

private:
    QVector<Vector3<Scalar>> vertices;
    QVector<int> polygonIndexes;
    QVector<int> polygonStartIndexes;
};

}

#endif // MESH_H
