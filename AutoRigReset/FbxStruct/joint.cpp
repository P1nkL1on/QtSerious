#include "joint.h"

using namespace FbxStruct;
using namespace Df;

Joint::Joint(const Vector3<float> &localTranslation,
             const Vector3<float> &localRotation,
             const Vector3<float> &localScaling, const bool isMeshDepended):
    localTranslation(localTranslation),
    localRotation(localRotation),
    localScaling(localScaling),
    meshDependent(isMeshDepended)
{

}

void Joint::setPaterIndex(const int paterPtrInd)
{
    Q_ASSERT(paterInd == -1);
    paterInd = paterPtrInd;
}

void Joint::addKidIndex(const int kidPtrInd)
{
    if (kidPtrInd != -1)
        kidsInd << kidPtrInd;
}

void Joint::addClusterIndex(const int clusterPtrInd)
{
    clusterInds << clusterPtrInd;
}

void Joint::setBindTransform(const Df::Matrix4<double> &value)
{
    bindTransform = value;
}

bool Joint::isMeshDependent() const
{
    return meshDependent;
}

int Joint::getPaterInd() const
{
    return paterInd;
}

QVector<int> Joint::getKidsInd() const
{
    return kidsInd;
}

Df::Matrix4<double> Joint::getBindTransform() const
{
    return bindTransform;
}
