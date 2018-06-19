#include "joint.h"

using namespace FbxStruct;
using namespace Df;

Joint::Joint(const bool isMeshDepended,
             const QString name):
    meshDependent(isMeshDepended),
    jointName(name)
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

QString Joint::getJointName() const
{
    return jointName;
}


JointTransform::JointTransform(
        const Df::Vector3<float> &localTranslation,
        const Df::Vector3<float> &localRotation,
        const Df::Vector3<float> &localScaling):
    localTranslation(localTranslation),
    localRotation(localRotation),
    localScaling(localScaling)
{

}

Df::Vector3<float> JointTransform::getLocalTranslation() const
{
    return localTranslation;
}

Df::Vector3<float> JointTransform::getLocalRotation() const
{
    return localRotation;
}

Df::Vector3<float> JointTransform::getLocalScaling() const
{
    return localScaling;
}
