#include "skeleton.h"

using namespace FbxStruct;
using namespace Df;

Joint::Joint(const Df::Vector3<float> &localTranslation,
             const Df::Vector3<float> &localRotation,
             const Df::Vector3<float> &localScaling):
    localTranslation(localTranslation),
    localRotation(localRotation),
    localScaling(localScaling)
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
