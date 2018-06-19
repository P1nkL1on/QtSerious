#ifndef JOINT_H
#define JOINT_H

#include "DerStruct/defines.h"

namespace FbxStruct{

class Joint
{
public:
    Joint() = default;
    Joint(const bool isMeshDepended,
          const QString name);

    void setPaterIndex(const int paterPtrInd);
    void addKidIndex (const int kidPtrInd);
    void addClusterIndex (const int clusterPtrInd);
    bool isMeshDependent() const;
    int getPaterInd() const;
    QVector<int> getKidsInd() const;
    QString getJointName() const;


private:
    QString jointName;
    bool meshDependent = false;
    int paterInd = -1;
    QVector<int> kidsInd;
    QVector<int> clusterInds;
//    Df::Matrix4<double> localTransform = Df::Matrix4<double>::Identity();
//    Df::Matrix4<double> globalTransform = Df::Matrix4<double>::Identity();
//    Df::Matrix4<double> bindTransform = Df::Matrix4<double>::Identity();
};

class JointTransform
{
public:
    JointTransform() = default;
    JointTransform(const Df::Vector3<float> &localTranslation,
                   const Df::Vector3<float> &localRotation,
                   const Df::Vector3<float> &localScaling);

    Df::Vector3<float> getLocalTranslation() const;
    Df::Vector3<float> getLocalRotation() const;
    Df::Vector3<float> getLocalScaling() const;
private:
    Df::Vector3<float> localTranslation;
    Df::Vector3<float> localRotation;
    Df::Vector3<float> localScaling = Df::Vector3<float>::Constant(1);
};

}
#endif // JOINT_H
