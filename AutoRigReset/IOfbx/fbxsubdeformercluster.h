#ifndef IOFBX_FBXSUBDEFORMERCLUSTER_H
#define IOFBX_FBXSUBDEFORMERCLUSTER_H

#include "fbxnode.h"


namespace IOfbx{

class FbxSubDeformerCluster : public FbxNode
{
public:
    FbxSubDeformerCluster();
    QString parse(const QStringList &S, const int param) override;
    bool isEmpty() const override;
private:
    QVector<int> indexes;
    QVector<double> weights;
    QVector<double> transformMatrix;
    QVector<double> transformLinkMatrix;
};

}

#endif // FBXSUBDEFORMERCLUSTER_H
