#include "skeleton.h"
//#include <math.h>

using namespace FbxStruct;
using namespace Df;

Skeleton::Skeleton(const QVector<Joint> &joints):
    joints(joints)
{
    for (int jInd = 0; jInd < joints.length(); ++jInd)
        if (joints[jInd].getPaterInd() < 0)
            rootIndexes << jInd;
}

void Skeleton::calculateMatrixes(const int variant)
{
    jointTranslations.clear();
    for (int i = 0; i < joints.length(); ++i)
        jointTranslations << Vector3<double>();
    for (const int rootInd : rootIndexes)
        calculateMatrix(rootInd, variant);

    for (int i = 0; i < jointTranslations.length(); ++i)
        qDebug() << i << getNameByIndex(i) << jointTranslations[i](0,0) << jointTranslations[i](1,0) << jointTranslations[i](2,0);
}

QString Skeleton::getNameByIndex(const int index) const
{
    return joints[index].getJointName();
}

Matrix4<double> Skeleton::calculateLocalTransformByIndex(const int index)
{

    //    localTransform = Matrix4<double>::Identity();
    //    localTransform = localTransform
    //            * scalingMatrix<double>(localScaling.cast<double>())
    //            * rotationMatrix<double>(localRotation.cast<double>())
    //            * translationMatrix<double>(localTranslation.cast<double>());
    //    return localTransform;
    const Matrix4<double> trans =
            translationMatrix<double>(joints[index].getLocalTranslation().cast<double>())
            //* scalingMatrix<double>(getInverseScale(joints[index].getPaterInd()))
            * scalingMatrix<double>(joints[index].getLocalScaling().cast<double>())
            * rotationMatrix<double>((joints[index].getLocalRotation().cast<double>()))
            ;
    return joints[index].setLocalTransform(trans);
}

QVector<int> Skeleton::getKidsByIndex(const int index) const
{
    if (index < 0)
        return QVector<int>();
    return joints[index].getKidsInd();
}

int Skeleton::getPaterByIndex(const int index) const
{
    if (index < 0)
        return -1;
    return joints[index].getPaterInd();
}

Matrix4<double> Skeleton::getLocalMatrixByIndex(const int index) const
{
    if (index < 0)
        return Matrix4<double>::Identity();
    return joints[index].getLocalTransform();
}

Matrix4<double> Skeleton::getGlobalMatrixByIndex(const int index) const
{
    if (index < 0)
        return Matrix4<double>::Identity();
    return joints[index].getGlobalTransform();
}

Vector3<double> Skeleton::getInverseScale(const int index) const
{
    Vector3<double> res = Df::makeUnitVector3<double>();
    if (index < 0)
        return res;
    res = joints[index].getLocalScaling().cast<double>();

    for (int i = 0; i < 3; ++i)
        res(i,0) = 1.0 / res(i,0);
    return res;
}

void Skeleton::calculateMatrix(const int currentJointIndex, const int variant)
{
    //qDebug() << QString("Matrix for bone %1").arg(currentJointIndex);
    // BIND MATRIX
    if (variant == 0)
        jointTranslations[currentJointIndex] =
                applyTransformToZeroVec(
                    joints[currentJointIndex].getBindTransform());
    // SELF CALCULATE
    if (variant == 1){
        Matrix4<double> paterMat = getGlobalMatrixByIndex(joints[currentJointIndex].getPaterInd());
        Matrix4<double> selfMat = calculateLocalTransformByIndex(currentJointIndex);//joints[currentJointIndex].calculateLocalTransformMatrix();
        Matrix4<double> finalMat = joints[currentJointIndex].setGlobalTransform(paterMat * selfMat);
        //qDebug() << getNameByIndex(currentJointIndex);
//        traceMatrix(paterMat);
//        qDebug() << "*";
//        traceMatrix(selfMat);
//        qDebug() << "=";
//        traceMatrix(finalMat);
        jointTranslations[currentJointIndex] =
                applyTransformToZeroVec<double>(finalMat);
    }

    for (const int ind : joints[currentJointIndex].getKidsInd())
        calculateMatrix(ind, variant);
}
