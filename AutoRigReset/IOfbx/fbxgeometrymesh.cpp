#include "fbxgeometrymesh.h"



QString IOfbx::FbxGeometryMesh::parse(const QStringList &buffer)
{
    QString error = QString();

    if (buffer.length() < 2)
        return QString ("Incorrect mesh buffer size - line count : " + buffer.length() );

    auto readyBuffer = buffer;
    QString type = readyBuffer.first();
    readyBuffer.removeFirst();  // vertices | polygons
    readyBuffer.removeFirst(); // -Vertices: *7122 {

    if (type == "vertices")
        vertices = parseFbxArray<double>(readyBuffer, error);
    if (type == "polygons")
        polygonVertexIndex = parseFbxArray<int>(readyBuffer,error);

    if (!error.isEmpty())
        return error;
    traceMessage (QString("v   Success parsed geometry mesh values;"));
    return error;
}

bool IOfbx::FbxGeometryMesh::isEmpty() const
{
    return vertices.length() == 0 && polygonVertexIndex.length() == 0;
}

bool IOfbx::FbxGeometryMesh::isComplete() const
{
    return !isEmpty() && hasNameAndID();
}

QVector<int> IOfbx::FbxGeometryMesh::getPolygonVertexIndex() const
{
    return polygonVertexIndex;
}

QVector<double> IOfbx::FbxGeometryMesh::getVertices() const
{
    return vertices;
}
