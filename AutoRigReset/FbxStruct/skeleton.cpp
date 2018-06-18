#include "skeleton.h"

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
    Matrix4<double> trans = Matrix4<double>::Identity();
    trans = trans
            * scalingMatrix<double>(joints[index].getLocalScaling().cast<double>())
            * scalingMatrix<double>(getInverseScale(index))
            * rotationMatrix<double>((joints[index].getLocalRotation().cast<double>()))
            * translationMatrix<double>(joints[index].getLocalTranslation().cast<double>())
            ;
    return joints[index].setLocalTransform(trans);
}

QVector<int> Skeleton::getKidsByIndex(const int index) const
{
    if (index < 0)
        return {};
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
    Vector3<double> res = Vector3<double>(1,1,1);
    if (index < 0)
        return res;
    res = joints[index].getLocalScaling().cast<double>();
    res = Vector3<double>(1.0 / res(0,0), 1.0 / res(1,0),1.0 / res(2,0));
    return res;
}

void Skeleton::calculateMatrix(const int currentJointIndex, const int variant)
{
    //qDebug() << QString("Matrix for bone %1").arg(currentJointIndex);
    // BIND MATRIX
    if (variant == 0)
        jointTranslations[currentJointIndex] =
                kostilBoneDrawer<double>(
                    joints[currentJointIndex].getBindTransform());
    // SELF CALCULATE
    if (variant == 1){
        Matrix4<double> paterMat = getGlobalMatrixByIndex(joints[currentJointIndex].getPaterInd());
        Matrix4<double> selfMat = calculateLocalTransformByIndex(currentJointIndex);//joints[currentJointIndex].calculateLocalTransformMatrix();
        Matrix4<double> finalMat = joints[currentJointIndex].setGlobalTransform(paterMat * selfMat);
//        traceMatrix(paterMat);
//        qDebug() << "*";
//        traceMatrix(selfMat);
//        qDebug() << "=";
//        traceMatrix(finalMat);
        jointTranslations[currentJointIndex] =
                kostilBoneDrawer<double>(finalMat);
    }

    for (const int ind : joints[currentJointIndex].getKidsInd())
        calculateMatrix(ind, variant);
}
