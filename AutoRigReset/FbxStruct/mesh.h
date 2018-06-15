#ifndef FBXSTRUCT_MESH_H
#define FBXSTRUCT_MESH_H

#include "DerStruct/defines.h"

namespace FbxStruct{

class Mesh
{
public:
    Mesh () = default;
    Mesh (const QVector<Df::Vector3<double>> &vertices,
          const QVector<int> &polygonIndexes,
          const QVector<int> &polygonStartIndexes);
private:
    QVector<Df::Vector3<double>> vertices;
    QVector<int> polygonIndexes;
    QVector<int> polygonStartIndexes;
};

}

#endif // FBXSTRUCT_MESH_H
