#include "skeleton.h"

using namespace FbxStruct;
using namespace DerStruct;
using namespace IOfbx;


Joint::Joint()
{
    localTransform = makeUnitMatrix<Derivable>();
    globalTransform = makeUnitMatrix<Derivable>();
}

Joint::Joint(const FbxModelJoint &parsedJoint)
{
    localTranslation = makeVector3fromQVector<Derivable>(parsedJoint.getLocalTranslation());
    localRotation = makeVector3fromQVector<Derivable>(parsedJoint.getLocalRotation());
    localScaling = makeVector3fromQVector<Derivable>(parsedJoint.getLocalScaling());

    localTransform = makeUnitMatrix<Derivable>();
    globalTransform = makeUnitMatrix<Derivable>();
}

const Matrix4<Derivable> &Joint::getGlobalTransform() const
{
    return globalTransform;
}
