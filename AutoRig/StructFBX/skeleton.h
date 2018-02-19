#ifndef SKELETON_H
#define SKELETON_H
#include "structfbx.h"

class Skeleton
{  

    QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> localScales;
    QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> localRotations;
    void RecursiveApplyLocalRotations(Joint* joint, Eigen::Matrix<DerOperations::Derivable,1,3> currentRotation);
    int transformesApplied;
public:
    Eigen::Matrix<DerOperations::Derivable,1,3> rootTransate;
    QVector<Joint*> joints;
    Skeleton();
    Skeleton(QVector<Joint*> j);

    // local
    void SetNullRotations();

    void DebugTree () const;
    bool CalculateGlobalCoordForEachJoint ();
    bool CalculateGlobalCoordForEachJointMatrix();
    bool SetBonesScaleAsBoneLength ();

    QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> getJointsLocalRotations () const;
    QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> getJointsGlobalTranslationsForSkin () const;
    Eigen::Matrix<DerOperations::Derivable,1,3> getJointCoordByIndex(int index, Eigen::Matrix<DerOperations::Derivable,1,3> &paterCoord);
    // local!
    void SetRotation (const Eigen::Matrix<DerOperations::Derivable,1,3> newRotation, int jointInd);
    void SetRotations (const QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> newRotations);

    void SetRootTranslation (const Eigen::Matrix<DerOperations::Derivable,1,3> assTranslate);
    void SetScales (const QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> newScales);

    // return local!
    bool getJointTranslationAndRotation (const int jointIndex, Eigen::Matrix<DerOperations::Derivable,1,3>& translation, Eigen::Matrix<DerOperations::Derivable,1,3>& rotation) const;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif // SKELETON_H
