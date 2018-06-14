#include "fbxgeometrymesh.h"



QString IOfbx::FbxGeometryMesh::parse(const QStringList &S, const int param)
{
    QString error = QString();

    auto readyBuffer = S;
    readyBuffer.removeFirst(); // -Vertices: *7122 {

    switch (param) {
    // 0 == vertices
    // 1 == PolygonVertexIndex
    case 0:
        vertices = parseFbxArray<double>(readyBuffer, error);
        break;
    case 1:
        polygonVertexIndex = parseFbxArray<int>(readyBuffer,error);
    default:
        break;
    }
    if (!error.isEmpty())
        return error;
    traceMessage (QString("v   Success parsed geometry mesh values;"));
    return error;
}

bool IOfbx::FbxGeometryMesh::isEmpty() const
{
    return vertices.length() == 0 && polygonVertexIndex.length() == 0;
}
