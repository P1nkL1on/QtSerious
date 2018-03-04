#ifndef SKELETON_H
#define SKELETON_H
#include "structfbx.h"

class Skeleton
{  
    QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> localScales;
    QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> localRotations;
    void RecursiveApplyLocalRotations(Joint* joint, Eigen::Matrix<DerOperations::Derivable,1,3> currentRotation);
    int transformesApplied;
    void SetRotation (const Eigen::Matrix<DerOperations::Derivable,1,3> newRotation, int jointInd);
public:
    Eigen::Matrix<DerOperations::Derivable,1,3> rootTransate;
    QVector<Joint*> joints;
    Skeleton();
    Skeleton( const Skeleton & ) = default;
    Skeleton(QVector<Joint*> j);

    // local
    void SetNullRotations();

    void DebugTree () const;
    bool CalculateGlobalCoordForEachJointMatrix();
    QVector<Eigen::Matrix<DerOperations::Derivable,4,4>> SetBonesScaleAsBoneLength ();

    QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> getJointsLocalRotationsForDrawing () const;
    QVector<Eigen::Matrix<DerOperations::Derivable,4,4>> getJointsGlobalTranslationsForSkin () const;
    QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> getJointsGlobalTranslationsForSaveClusters () const;
    Eigen::Matrix<DerOperations::Derivable,1,3> getJointCoordByIndex(int index, Eigen::Matrix<DerOperations::Derivable,1,3> &paterCoord);
    // local!
    void SetRotations (const QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> newRotations);
    void SetRootTranslation (const Eigen::Matrix<DerOperations::Derivable,1,3> assTranslate);
    void SetScales (const QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> newScales);

    // return local!
    //bool getJointTranslationAndRotation (const int jointIndex, Eigen::Matrix<DerOperations::Derivable,1,3>& translation, Eigen::Matrix<DerOperations::Derivable,1,3>& rotation) const;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif // SKELETON_H
