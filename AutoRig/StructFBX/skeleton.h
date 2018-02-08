#ifndef SKELETON_H
#define SKELETON_H
#include "structfbx.h"

class Skeleton
{  
    QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> localRotations;
    void RecursiveApplyLocalRotations(Joint* joint, Eigen::Matrix<DerOperations::Derivable,1,3> currentRotation);
    int transformesApplied;
public:
    QVector<Joint*> joints;
    Skeleton();
    Skeleton(QVector<Joint*> j);

    // local
    void SetNullRotations();

    void DebugTree () const;
    bool CalculateGlobalCoordForEachJoint ();
    bool CalculateGlobalCoordForEachJointMatrix();

    QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> getJointsLocalRotations () const;
    QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> getJointsGlobalTranslationsForSkin () const;
    Eigen::Matrix<DerOperations::Derivable,1,3> getJointCoordByIndex(int index, Eigen::Matrix<DerOperations::Derivable,1,3> &paterCoord);
    // local!
    void SetRotation (const Eigen::Matrix<DerOperations::Derivable,1,3> newRotation, int jointInd);
    void SetRotations (const QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> newRotations);

    // return local!
    bool getJointTranslationAndRotation (const int jointIndex, Eigen::Matrix<DerOperations::Derivable,1,3>& translation, Eigen::Matrix<DerOperations::Derivable,1,3>& rotation) const;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif // SKELETON_H
