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
          const Df::Vector3<float> &localScaling,
          const bool isMeshDepended);
    template <typename Numerical>
    const Df::Matrix4<Numerical> &calculateLocalTransformMatrix();
    template <typename Numerical>
    const Df::Matrix4<Numerical> &calculateGlobalTransformMatrix();
    void setPaterIndex(const int paterPtrInd);
    void addKidIndex (const int kidPtrInd);
    void setBindTransform(const Df::Matrix4<double> &value);
    bool isMeshDependent() const;
private:
    bool meshDependent = false;
    int paterInd = -1;
    QVector<int> kidsInd;
    Df::Vector3<float> localTranslation;
    Df::Vector3<float> localRotation;
    Df::Vector3<float> localScaling = Df::makeUnitVector3<float>();
    Df::Matrix4<double> localTransform = Df::Matrix4<double>::Identity();
    Df::Matrix4<double> globalTransform = Df::Matrix4<double>::Identity();
    Df::Matrix4<double> bindTransform = Df::Matrix4<double>::Identity();
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
