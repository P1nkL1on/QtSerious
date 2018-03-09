#ifndef STRUCTFBX_H
#define STRUCTFBX_H

#include "qstring.h"
#include "qvector.h"
#include "Eigen/Core"
#include "derivable.h"


struct Joint{
public:
    QString ID;
    QString name;
    Joint* pater;
    QVector<Joint*> kids;
    QVector<QString> AnimCurvesIDs;
    Eigen::Matrix<DerOperations::Derivable,1,3> localScale;
    Eigen::Matrix<DerOperations::Derivable,1,3> localTranslation;
    Eigen::Matrix<DerOperations::Derivable,1,3> localTranslationLoaded;
    Eigen::Matrix<DerOperations::Derivable,1,3> currentTranslation;
    Eigen::Matrix<DerOperations::Derivable,1,3> endCurrentTranslation;
    Eigen::Matrix<DerOperations::Derivable,1,3> currentRotation;
    Eigen::Matrix<DerOperations::Derivable,4,4> bindMatrix;
    Eigen::Matrix<DerOperations::Derivable,1,3> bindTransform;

    Eigen::Matrix<DerOperations::Derivable,4,4> localTransformMatrix;
    Eigen::Matrix<DerOperations::Derivable,4,4> globalTransformMatrix;

    Joint();
    Joint(QString ID0, QString name0);
    Joint(Eigen::Matrix<DerOperations::Derivable,1,3> trans,
          Eigen::Matrix<DerOperations::Derivable,1,3> rotat);

    void RecaulculateLocalTransformMatrix ();
    void ResetGlobalTransformMatrix();
    void CalculateGlobalTransformMatrix();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


struct AttendedCluster{
public:
    int jointIndex;
    QVector<int> vertexIndex;
    QVector<float> weights;
    //QVector<Eigen::Matrix<DerOperations::Derivable,1,3>> localJointCoords;
    Eigen::Matrix<DerOperations::Derivable,4,4> boneBindCoord;
    AttendedCluster();
    AttendedCluster(Eigen::Matrix<DerOperations::Derivable,4,4>, int JointIndex);
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

namespace CommonFuncs {
    Eigen::Matrix<DerOperations::Derivable,4,4> GetNormalRotateMatrix (const Eigen::Matrix<DerOperations::Derivable,1,3> rotatXYZ);
    Eigen::Matrix<DerOperations::Derivable,1,3> AddDirectMatrx (const Eigen::Matrix<DerOperations::Derivable,1,3> localTransform, const Eigen::Matrix<DerOperations::Derivable,4,4> transform);
    Eigen::Matrix<DerOperations::Derivable,1,3> AddDirect (const Eigen::Matrix<DerOperations::Derivable,1,3> to, const Eigen::Matrix<DerOperations::Derivable,1,3> Transform, const Eigen::Matrix<DerOperations::Derivable,1,3> Rotation);
    Eigen::Matrix<DerOperations::Derivable,1,3> AddDirectWtParent (const Eigen::Matrix<DerOperations::Derivable,1,3> to, const Eigen::Matrix<DerOperations::Derivable,1,3> Transform, const Eigen::Matrix<DerOperations::Derivable,1,3> wasRotation, const Eigen::Matrix<DerOperations::Derivable,1,3> addRotation );
}



#endif // STRUCTFBX_H
