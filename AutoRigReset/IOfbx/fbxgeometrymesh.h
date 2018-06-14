#ifndef IOFBX_FBXGEOMETRYMESH_H
#define IOFBX_FBXGEOMETRYMESH_H

#include "fbxnode.h"

namespace IOfbx{

class FbxGeometryMesh : public FbxNode
{
public:
    FbxGeometryMesh();
    QString parse(const QStringList &S, const int param) override;
    bool isEmpty() const override;
private:
    QVector<double> vertices;
    QVector<int> polygonVertexIndex;
};

}

#endif // FBXGEOMETRYMESH_H
