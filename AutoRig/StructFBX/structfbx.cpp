#include "structfbx.h"
#include "Derivable/dermatops.h"

using namespace DerivableVectorMatrixes;
using namespace DerOperations;
using Eigen::Matrix;

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
}

void Joint::RecaulculateLocalTransformMatrix()
{
    bool isChecked = ((pater != NULL) && (pater->currentRotation(0,1).getValue() != 0 || pater->currentRotation(0,2).getValue() != 0 | pater->currentRotation(0,0).getValue() != 0));
    localTransformMatrix = SetDeriveMatrix();

    Matrix<Derivable,1,3> currentRotation2 = (pater != NULL)?pater->currentRotation : Matrix<Derivable,1,3>(0,0,0);

//    localTransformMatrix.rotate(currentRotation2.x(), 1.0, 0, 0);
//    localTransformMatrix.rotate(currentRotation2.y(), 0, 1.0, 0);
//    localTransformMatrix.rotate(currentRotation2.z(), 0, 0, 1.0);
//    localTransformMatrix.translate(localTranslation);
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
    Joint* now = this;
    do{
        globalTransformMatrix = globalTransformMatrix * now->localTransformMatrix;
        now = now->pater;
    } while ( now != NULL );
}


AttendedVertex::AttendedVertex()
{
    jointIndexs = {};
    weights = {};
    localJointCoords = {};
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
//    wasRotateMatrix.rotate(wasRotation.x(), 1.0, 0, 0);
//    wasRotateMatrix.rotate(wasRotation.y(), 0, 1.0, 0);
//    wasRotateMatrix.rotate(wasRotation.z(), 0, 0, 1.0);

//    addRotateMatrix.rotate(addRotation.x(), 1.0, 0, 0);
//    addRotateMatrix.rotate(addRotation.y(), 0, 1.0, 0);
//    addRotateMatrix.rotate(addRotation.z(), 0, 0, 1.0);
    RotateDeriveMatrix(wasRotateMatrix, wasRotation);
    RotateDeriveMatrix(addRotateMatrix, addRotation);

    Matrix<Derivable,1,4> temp = MakeVector4From3(Transform, Derivable(1)) * addRotateMatrix * wasRotateMatrix;
    return Summ3and4( to, temp);
}

Matrix<Derivable,1,3> CommonFuncs::AddDirectMatrx(const Matrix<Derivable,1,3> localTransform, const Matrix<Derivable,4,4> transform)
{
    //TraceVector(MakeVector4From3(localTransform, Derivable(1)));
    //TraceMatrix(transform);
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
