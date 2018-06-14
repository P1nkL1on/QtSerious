#ifndef IOFBX_FBXGEOMETRYMESH_H
#define IOFBX_FBXGEOMETRYMESH_H

#include "fbxnode.h"

namespace IOfbx{

class FbxGeometryMesh : public FbxNode
{
public:
    FbxGeometryMesh() = default;
    QString parse(const QStringList &buffer) override;
    bool isEmpty() const override;
    bool isComplete() const;
private:
    QVector<double> vertices;
    QVector<int> polygonVertexIndex;
};

}

#endif // FBXGEOMETRYMESH_H
