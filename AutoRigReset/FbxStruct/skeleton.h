#ifndef SKELETON_H
#define SKELETON_H


#include "fbxstruct.h"
#include "iofbx.h"
#include "derivable.h"

namespace FbxStruct{

class Joint
{
public:
    Joint();
    Joint(const IOfbx::FbxModelJoint &parsedJoint);
    const Matrix4<DerStruct::Derivable> &calculateLocalTransformMatrix();
    const Matrix4<DerStruct::Derivable> &calculateGlobalTransformMatrix();
    const Matrix4<DerStruct::Derivable>& getGlobalTransform() const;
private:
    Vector3<DerStruct::Derivable> localTranslation;
    Vector3<DerStruct::Derivable> localRotation;
    Vector3<DerStruct::Derivable> localScaling;
    Matrix4<DerStruct::Derivable> localTransform;
    Matrix4<DerStruct::Derivable> globalTransform;
};

class Skeleton
{
public:
    Skeleton();
};
}

#endif // SKELETON_H
