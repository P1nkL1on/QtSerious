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
          const QVector<int> &polygonIndices,
          const QVector<int> &polygonStartIndices);
    QVector<QVector3D> getVertices () const;
    QVector<int> getPolygonIndices() const;
    QVector<int> getPolygonStartIndices() const;
    void applyTransform (const Df::Matrix4<double> &mat);
private:
    QVector<Df::Vector3<double>> vertices;
    QVector<int> polygonIndices;
    QVector<int> polygonStartIndices;
};

}

#endif // FBXSTRUCT_MESH_H
