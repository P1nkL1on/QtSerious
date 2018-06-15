#include "mesh.h"


FbxStruct::Mesh::Mesh(const QVector<Df::Vector3<double> > &vertices,
                      const QVector<int> &polygonIndexes,
                      const QVector<int> &polygonStartIndexes) :
    vertices(vertices),
    polygonIndexes(polygonIndexes),
    polygonStartIndexes(polygonStartIndexes)
{

}

QVector<QVector3D> FbxStruct::Mesh::getVertices() const
{
    QVector<QVector3D> res;
    for (const auto v : vertices)
        res << QVector3D(v(0,0), v(1,0), v(2,0));
    return res;
}

QVector<int> FbxStruct::Mesh::getPolygonStartIndexes() const
{
    return polygonStartIndexes;
}

QVector<int> FbxStruct::Mesh::getPolygonIndexes() const
{
    return polygonIndexes;
}
