#ifndef FBXSTRUCT_MESH_H
#define FBXSTRUCT_MESH_H

#include "DerStruct/defines.h"
#include "QVector3D"

namespace FbxStruct{

class Mesh
{
public:
    Mesh () = default;
    Mesh (const QVector<Df::Vector3<double>> &vertices,
          const QVector<int> &polygonIndexes,
          const QVector<int> &polygonStartIndexes);
    QVector<QVector3D> getVertices () const;
    QVector<int> getPolygonIndexes() const;
    QVector<int> getPolygonStartIndexes() const;

private:
    QVector<Df::Vector3<double>> vertices;
    QVector<int> polygonIndexes;
    QVector<int> polygonStartIndexes;
};

}

#endif // FBXSTRUCT_MESH_H
