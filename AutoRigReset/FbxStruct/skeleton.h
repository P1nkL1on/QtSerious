#ifndef FBXSTRUCT_SKELETON_H
#define FBXSTRUCT_SKELETON_H


#include "fbxstruct.h"

namespace FbxStruct{

class Joint
{
public:
    Joint();
    Joint(const IOfbx::FbxModelJoint &parsedJoint);
    template <typename Numerical>
    const Matrix4<Numerical> &calculateLocalTransformMatrix();
    template <typename Numerical>
    const Matrix4<Numerical> &calculateGlobalTransformMatrix();
private:
    Vector3<float> localTranslation;
    Vector3<float> localRotation;
    Vector3<float> localScaling;
    Matrix4<float> localTransform;
    Matrix4<float> globalTransform;
};

class Skeleton
{
public:
    Skeleton();
};
}

#endif // FBXSTRUCT_SKELETON_H
