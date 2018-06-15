#ifndef FBXSTRUCT_SKELETON_H
#define FBXSTRUCT_SKELETON_H


#include "DerStruct/defines.h"

namespace FbxStruct{

class Joint
{
public:
    Joint() = default;
//    Joint(const Joint &other) = delete;
    Joint(const Df::Vector3<float> &localTranslation,
          const Df::Vector3<float> &localRotation,
          const Df::Vector3<float> &localScaling);
    template <typename Numerical>
    const Df::Matrix4<Numerical> &calculateLocalTransformMatrix();
    template <typename Numerical>
    const Df::Matrix4<Numerical> &calculateGlobalTransformMatrix();
    void setPaterIndex(const int paterPtrInd);
    void addKidIndex (const int kidPtrInd);
private:
    int paterInd = -1;
    QVector<int> kidsInd;
    Df::Vector3<float> localTranslation;
    Df::Vector3<float> localRotation;
    Df::Vector3<float> localScaling = Df::makeUnitVector3<float>();
    Df::Matrix4<float> localTransform = Df::Matrix4<float>::Identity();
    Df::Matrix4<float> globalTransform = Df::Matrix4<float>::Identity();
    Df::Matrix4<float> bindTransform = Df::Matrix4<float>::Identity();
};

class Skeleton
{
public:
    Skeleton() = default;
    Skeleton(const QVector<Joint> &joints, const QVector<int> &rootIndexes);
    QVector<Joint> joints;
    QVector<int> rootIndexes;
};
}

#endif // FBXSTRUCT_SKELETON_H
