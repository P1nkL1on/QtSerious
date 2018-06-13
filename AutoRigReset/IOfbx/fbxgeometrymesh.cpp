#include "fbxgeometrymesh.h"

IOfbx::FbxGeometryMesh::FbxGeometryMesh()
{
    name = id = "";
    vertices = QVector<double>();
    polygonVertexIndex = QVector<int>();
}


QString IOfbx::FbxGeometryMesh::parse(QStringList S, const int param)
{
    QString error = QString();

    S.removeFirst(); // -Vertices: *7122 {

    switch (param) {
    // 0 == vertices
    // 1 == PolygonVertexIndex
    case 0:
        vertices = parseFbxArray<double>(S, error);
        break;
    case 1:
        polygonVertexIndex = parseFbxArray<int>(S,error);
    default:
        break;
    }
    if (!error.isEmpty())
        return error;
    traceMessage (QString("v   Success parsed geometry mesh values;"));
    return error;
}
