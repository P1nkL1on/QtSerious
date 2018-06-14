#include "skeleton.h"

using namespace FbxStruct;
using namespace DerStruct;
using namespace IOfbx;


Joint::Joint()
{
    localTransform = makeUnitMatrix<float>();
    globalTransform = makeUnitMatrix<float>();
}

Joint::Joint(const FbxModelJoint &parsedJoint)
{
    localTranslation = makeVector3fromQVector<float>(parsedJoint.getLocalTranslation());
    localRotation = makeVector3fromQVector<float>(parsedJoint.getLocalRotation());
    localScaling = makeVector3fromQVector<float>(parsedJoint.getLocalScaling());

    localTransform = makeUnitMatrix<float>();
    globalTransform = makeUnitMatrix<float>();
}
