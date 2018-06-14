#ifndef IOFBX_FBXPOSENODE_H
#define IOFBX_FBXPOSENODE_H

#include "fbxnode.h"

namespace IOfbx{

class FbxPoseNode : public FbxNode
{
public:
    FbxPoseNode() = default;
    QString parse(const QStringList &S, const int param) override;
private:
    QVector<double> transformMatrixArray;
};

}

#endif // FBXPOSENODE_H
