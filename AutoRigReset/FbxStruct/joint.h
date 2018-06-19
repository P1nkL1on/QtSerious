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
    QVector<int> getClusterInds() const;
private:
    QString jointName;
    bool meshDependent = false;
    int paterInd = -1;
    QVector<int> kidsInd;
    QVector<int> clusterInds;
};

class JointTransform
{
public:
    JointTransform() = default;
    JointTransform(const Df::Vector3<double> &localTranslation,
                   const Df::Vector3<double> &localRotation,
                   const Df::Vector3<double> &localScaling);

    Df::Vector3<double> getLocalTranslation() const;
    Df::Vector3<double> getLocalRotation() const;
    Df::Vector3<double> getLocalScaling() const;
    Df::Vector3<double> getInverseScaling() const;
private:
    Df::Vector3<double> localTranslation;
    Df::Vector3<double> localRotation;
    Df::Vector3<double> localScaling = Df::Vector3<double>::Constant(double(1));
};

}
#endif // JOINT_H
