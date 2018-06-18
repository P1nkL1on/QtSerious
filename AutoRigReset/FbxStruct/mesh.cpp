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

void FbxStruct::Mesh::applyTransform(const Df::Matrix4<double> &mat)
{
    Df::Vector3<double> p1 = vertices[0];
    const int vertCount = vertices.length();
    Eigen::MatrixXd vertArr = Eigen::MatrixXd(4, vertCount);
    int vInd = 0;
    for (const auto v : vertices){
        for (int demens = 0; demens < 3; ++demens)
            vertArr(demens, vInd) = v(demens, 0);
        vertArr(3, vInd) = 1;
        ++vInd;
    }
    auto const transformedDataArray = mat * vertArr;
    for (int vInd = 0; vInd < vertices.length(); ++vInd)
        vertices[vInd] = Df::Vector3<double>(
                    transformedDataArray(0, vInd),
                    transformedDataArray(1, vInd),
                    transformedDataArray(2, vInd));

    Df::Vector3<double> p2 = vertices[0];
    qDebug() << QString("(%1, %2, %3)  -->  (%4, %5, %6)")
                .arg(p1(0,0)).arg(p1(1,0)).arg(p1(2,0))
                .arg(p2(0,0)).arg(p2(1,0)).arg(p2(2,0));
}

QVector<int> FbxStruct::Mesh::getPolygonIndexes() const
{
    return polygonIndexes;
}
