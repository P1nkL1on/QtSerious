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
          const bool isMeshDepended,
          const QString name);
//    const Df::Matrix4<double> &calculateLocalTransformMatrix(){
//        localTransform = Df::Matrix4<double>::Identity();
//        localTransform = localTransform
//                * Df::scalingMatrix<double>(localScaling.cast<double>())
//                * Df::rotationMatrix<double>(localRotation.cast<double>())
//                * Df::translationMatrix<double>(localTranslation.cast<double>());
//        return localTransform;
//    }

    void setPaterIndex(const int paterPtrInd);
    void addKidIndex (const int kidPtrInd);
    void addClusterIndex (const int clusterPtrInd);
    bool isMeshDependent() const;
    int getPaterInd() const;
    QVector<int> getKidsInd() const;
    Df::Matrix4<double> getBindTransform() const;
    Df::Matrix4<double> getLocalTransform() const;
    Df::Matrix4<double> getGlobalTransform() const;
    Df::Matrix4<double> setGlobalTransform(const Df::Matrix4<double> &value);
    Df::Matrix4<double> setLocalTransform(const Df::Matrix4<double> &value);
    void setBindTransform(const Df::Matrix4<double> &value);
    QString getJointName() const;
    Df::Vector3<float> getLocalTranslation() const;
    Df::Vector3<float> getLocalRotation() const;
    Df::Vector3<float> getLocalScaling() const;


private:
    QString jointName;
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


}
#endif // JOINT_H
