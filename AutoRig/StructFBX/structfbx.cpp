#include "structfbx.h"
#include "Derivable/dermatops.h"

using namespace DerivableVectorMatrixes;
using namespace DerOperations;
using Eigen::Matrix;
#include <Eigen/LU>

Joint::Joint()
{
    ID = QString();
    name = QString();
    pater = NULL;
    kids = {};
    localTranslation = Matrix<Derivable,1,3>();
    currentTranslation = Matrix<Derivable,1,3>();
    currentRotation = Matrix<Derivable,1,3>();
    bindMatrix = SetDeriveMatrix();
    bindTransform = Matrix<Derivable,1,3>();

    localScale =  Matrix<Derivable,1,3>(1,1,1);
    AnimCurvesIDs = {"-","-","-"};
}

Joint::Joint(QString ID0, QString name0)
{
    this->ID = ID0;
    this->name = name0;
    pater = NULL;
    kids = {};
    localTranslation = Matrix<Derivable,1,3>();
    currentTranslation = Matrix<Derivable,1,3>();
    currentRotation = Matrix<Derivable,1,3>();
    bindMatrix = SetDeriveMatrix();
    bindTransform = Matrix<Derivable,1,3>();
    localScale =  Matrix<Derivable,1,3>(1,1,1);
    AnimCurvesIDs = {"-","-","-"};
}

Joint::Joint(Eigen::Matrix<Derivable, 1, 3> trans, Eigen::Matrix<Derivable, 1, 3> rotat)
{
    this->ID = "noneID";
    name = "bone";
    pater = NULL;
    kids = {};
    localTranslation = currentTranslation = trans;
    currentRotation = rotat;
    bindMatrix = SetDeriveMatrix();
    bindTransform = localTranslation;
    localScale =  Matrix<Derivable,1,3>(1,1,1);
    AnimCurvesIDs = {"-","-","-"};
}

void Joint::RecaulculateLocalTransformMatrix()
{
    localTransformMatrix = SetDeriveMatrix();

    Matrix<Derivable,1,3> currentRotation2 = (pater != NULL)?pater->currentRotation : Matrix<Derivable,1,3>(0,0,0);

    if (pater != NULL)
         ScaleDeriveMatrix(localTransformMatrix,
                           Matrix<Derivable,1,3>(Derivable(1) / pater->localScale(0,0), Derivable(1) / pater->localScale(0,1), Derivable(1) / pater->localScale(0,2)));
    ScaleDeriveMatrix(localTransformMatrix, localScale);    // scale self
    TranslateDeriveMatrix(localTransformMatrix, localTranslation);
    RotateDeriveMatrix(localTransformMatrix, currentRotation2);

    ResetGlobalTransformMatrix();
}

void Joint::ResetGlobalTransformMatrix()
{
    globalTransformMatrix = SetDeriveMatrix();
}

void Joint::CalculateGlobalTransformMatrix()
{
    ResetGlobalTransformMatrix();
    Joint* now = this;
    do{
        globalTransformMatrix = globalTransformMatrix * now->localTransformMatrix;
        now = now->pater;
    } while ( now != NULL );
}


AttendedCluster::AttendedCluster()
{
    vertexIndex = {};
    weights = {};
    boneBindCoord = SetDeriveMatrix();
    jointIndex = -1;
}

AttendedCluster::AttendedCluster(Matrix<Derivable,4,4> bindMatrix, int JointIndex)
{
    vertexIndex = {};
    weights = {};
    boneBindCoord = bindMatrix.inverse();
    jointIndex = JointIndex;
}

Matrix<Derivable,1,3> CommonFuncs::AddDirect(const Matrix<Derivable,1,3> to, const Matrix<Derivable,1,3> Transform, const Matrix<Derivable,1,3> Rotation)
{
    // (r1 * r2 * r3) * V
    // ротаёшоны применяются в порядке справа налево - 3 2 1

    // QMatrix M; M.Rotate(); M.Transform(); === M * R * T; -- обратынй поряок


    Matrix<Derivable,4,4> rotateMatrix = SetDeriveMatrix();

//    rotateMatrix.rotate(Rotation.x(), 1.0, 0, 0);
//    rotateMatrix.rotate(Rotation.y(), 0, 1.0, 0);
//    rotateMatrix.rotate(Rotation.z(), 0, 0, 1.0);
//    Matrix<Derivable,1,4> temp = rotateMatrix * Matrix<Derivable,1,4>(Transform, 1.0);
    RotateDeriveMatrix(rotateMatrix, Rotation);
    Matrix<Derivable,1,4> temp = MakeVector4From3(Transform, Derivable(1)) * rotateMatrix;

    return Summ3and4( to, temp);
}

Matrix<Derivable,1,3> CommonFuncs::AddDirectWtParent(const Matrix<Derivable,1,3> to, const Matrix<Derivable,1,3> Transform, const Matrix<Derivable,1,3> wasRotation, const Matrix<Derivable,1,3> addRotation)
{
    Matrix<Derivable,4,4> wasRotateMatrix = SetDeriveMatrix(), addRotateMatrix = SetDeriveMatrix();

    RotateDeriveMatrix(wasRotateMatrix, wasRotation);
    RotateDeriveMatrix(addRotateMatrix, addRotation);

    Matrix<Derivable,1,4> temp = MakeVector4From3(Transform, Derivable(1)) * addRotateMatrix * wasRotateMatrix;
    return Summ3and4( to, temp);
}

Matrix<Derivable,1,3> CommonFuncs::AddDirectMatrx(const Matrix<Derivable,1,3> localTransform, const Matrix<Derivable,4,4> transform)
{
    Matrix<Derivable,1,4> temp = MakeVector4From3(localTransform, Derivable(1)) * transform;

    Matrix<Derivable,1,3> fin = Matrix<Derivable,1,3>( temp(0,0), temp(0,1),  temp(0,2));
    return fin;
}

Matrix<Derivable,4,4> CommonFuncs::GetNormalRotateMatrix(const Matrix<Derivable,1,3> rotatXYZ)
{
    Matrix<Derivable,4,4> localTransformMatrix = SetDeriveMatrix();

//    localTransformMatrix.rotate(rotatXYZ.x(), 1.0, 0, 0);
//    localTransformMatrix.rotate(rotatXYZ.y(), 0, 1.0, 0);
//    localTransformMatrix.rotate(rotatXYZ.z(), 0, 0, 1.0);

    RotateDeriveMatrix(localTransformMatrix, rotatXYZ);

    return localTransformMatrix;
}
