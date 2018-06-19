#include "mesh.h"


FbxStruct::Mesh::Mesh(const QVector<Df::Vector3<double> > &vertices,
                      const QVector<int> &polygonIndices,
                      const QVector<int> &polygonStartIndices) :
    vertices(vertices),
    polygonIndices(polygonIndices),
    polygonStartIndices(polygonStartIndices)
{

}

QVector<QVector3D> FbxStruct::Mesh::getVertices() const
{
    QVector<QVector3D> res;
    for (const auto v : vertices)
        res << QVector3D(v(0,0), v(1,0), v(2,0));
    return res;
}

QVector<int> FbxStruct::Mesh::getPolygonStartIndices() const
{
    return polygonStartIndices;
}

void FbxStruct::Mesh::applyTransform(const Df::Matrix4<double> &mat)
{
    for (int v = 0; v < vertices.length(); ++v)
        vertices[v] = Df::applyTransform<double>(vertices[v], mat);
}

QVector<int> FbxStruct::Mesh::getPolygonIndices() const
{
    return polygonIndices;
}
