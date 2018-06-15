#include "mesh.h"


FbxStruct::Mesh::Mesh(const QVector<Df::Vector3<double> > &vertices,
                      const QVector<int> &polygonIndexes,
                      const QVector<int> &polygonStartIndexes) :
    vertices(vertices),
    polygonIndexes(polygonIndexes),
    polygonStartIndexes(polygonStartIndexes)
{

}
