#ifndef IOFBX_FBXSUBDEFORMERCLUSTER_H
#define IOFBX_FBXSUBDEFORMERCLUSTER_H

#include "fbxnode.h"


namespace IOfbx{

class FbxSubDeformerCluster : public FbxNode
{
public:
    FbxSubDeformerCluster() = default;
    QString parse(const QStringList &buffer) override;
    bool isEmpty() const override;
    bool isParentDeformder () const;
    QVector<int> getIndexes() const;
    QVector<double> getWeights() const;
    QVector<double> getTransformMatrix() const;
    QVector<double> getTransformLinkMatrix() const;

private:
    bool isDeformer = false;
    QVector<int> indexes;
    QVector<double> weights;
    QVector<double> transformMatrix;
    QVector<double> transformLinkMatrix;
};

}

#endif // FBXSUBDEFORMERCLUSTER_H
