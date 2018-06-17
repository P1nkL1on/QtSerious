#ifndef JOINT_H
#define JOINT_H

#include "DerStruct/defines.h"

namespace FbxStruct{

class Joint
{
public:
    Joint() = default;
    Joint(const Df::Vector3<float> &localTranslation,
          const Df::Vector3<float> &localRotation,
          const Df::Vector3<float> &localScaling,
          const bool isMeshDepended);
    const Df::Matrix4<double> &calculateLocalTransformMatrix();
//    template <typename Numerical>
//    const Df::Matrix4<Numerical> &calculateGlobalTransformMatrix();
    void setPaterIndex(const int paterPtrInd);
    void addKidIndex (const int kidPtrInd);
    void addClusterIndex (const int clusterPtrInd);
    void setBindTransform(const Df::Matrix4<double> &value);
    bool isMeshDependent() const;
    int getPaterInd() const;
    QVector<int> getKidsInd() const;
    Df::Matrix4<double> getBindTransform() const;
private:
    bool meshDependent = false;
    int paterInd = -1;
    QVector<int> kidsInd;
    QVector<int> clusterInds;
    Df::Vector3<float> localTranslation;
    Df::Vector3<float> localRotation;
    Df::Vector3<float> localScaling = Df::Vector3<float>::Constant(1);
    Df::Matrix4<double> localTransform = Df::Matrix4<double>::Identity();
    Df::Matrix4<double> globalTransform = Df::Matrix4<double>::Identity();
    Df::Matrix4<double> bindTransform = Df::Matrix4<double>::Identity();
};

const Df::Matrix4<double> &Joint::calculateLocalTransformMatrix()
{
    localTransform = Df::Matrix4<double>::Identity();
    localTransform = localTransform
            * Df::scalingMatrix<double>(localScaling)
            * Df::rotationMatrix<double>(localRotation)
            * Df::translationMatrix<double>(localTranslation);
    return localTransform;
}
}
#endif // JOINT_H
