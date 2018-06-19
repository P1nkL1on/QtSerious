#include "skeleton.h"

using namespace FbxStruct;
using namespace Df;

//Skeleton::Skeleton(const QVector<Joint> &joints):
//    joints(joints)
//{
//    for (int jInd = 0; jInd < joints.length(); ++jInd)
//        if (!joints[jInd].isMeshDependent() && joints[jInd].getPaterInd() < 0)
//            rootIndexes << jInd;
//}

//void Skeleton::calculateMatrixes()
//{
//    jointTranslations.clear();
//    for (int i = 0; i < joints.length(); ++i)
//        jointTranslations << Vector3<double>();
//    for (const int rootInd : rootIndexes)
//        calculateMatrix(rootInd);
//    // info for each bone
//    for (int i = 0; i < jointTranslations.length(); ++i)
//        qDebug() << i << getNameByIndex(i) << jointTranslations[i](0,0) << jointTranslations[i](1,0) << jointTranslations[i](2,0);
//}

//QString Skeleton::getNameByIndex(const int index) const
//{
//    return joints[index].getJointName();
//}

//Matrix4<double> Skeleton::calculateLocalTransformByIndex(const int index)
//{

//    //    localTransform = Matrix4<double>::Identity();
//    //    localTransform = localTransform
//    //            * scalingMatrix<double>(localScaling.cast<double>())
//    //            * rotationMatrix<double>(localRotation.cast<double>())
//    //            * translationMatrix<double>(localTranslation.cast<double>());
//    //    return localTransform;
//    const Matrix4<double> trans =
//            translationMatrix<double>(joints[index].getLocalTranslation().cast<double>())
//            //* scalingMatrix<double>(getInverseScale(joints[index].getPaterInd()))
//            * scalingMatrix<double>(joints[index].getLocalScaling().cast<double>())
//            * rotationMatrix<double>((joints[index].getLocalRotation().cast<double>()))
//            ;
//    return joints[index].setLocalTransform(trans);
//}

//QVector<int> Skeleton::getKidsByIndex(const int index) const
//{
//    if (index < 0)
//        return QVector<int>();
//    return joints[index].getKidsInd();
//}

//int Skeleton::getPaterByIndex(const int index) const
//{
//    if (index < 0)
//        return -1;
//    return joints[index].getPaterInd();
//}

//Matrix4<double> Skeleton::getLocalMatrixByIndex(const int index) const
//{
//    if (index < 0)
//        return Matrix4<double>::Identity();
//    return joints[index].getLocalTransform();
//}

//Matrix4<double> Skeleton::getGlobalMatrixByIndex(const int index) const
//{
//    if (index < 0)
//        return Matrix4<double>::Identity();
//    return joints[index].getGlobalTransform();
//}

//Vector3<double> Skeleton::getInverseScale(const int index) const
//{
//    Vector3<double> res = Df::makeUnitVector3<double>();
//    if (index < 0)
//        return res;
//    res = joints[index].getLocalScaling().cast<double>();

//    for (int i = 0; i < 3; ++i)
//        res(i,0) = 1.0 / res(i,0);
//    return res;
//}

//void Skeleton::calculateMatrix(const int currentJointIndex)
//{
//    //qDebug() << QString("Matrix for bone %1").arg(currentJointIndex);
//    // BIND MATRIX GETTER ONLY
//    //    if (variant == 0)
//    //        jointTranslations[currentJointIndex] =
//    //                applyTransformToZeroVec(
//    //                    joints[currentJointIndex].getBindTransform());
//    Matrix4<double> paterMat = getGlobalMatrixByIndex(joints[currentJointIndex].getPaterInd());
//    Matrix4<double> selfMat = calculateLocalTransformByIndex(currentJointIndex);
//    Matrix4<double> finalMat = joints[currentJointIndex].setGlobalTransform(paterMat * selfMat);
//    // calculate final global point
//    jointTranslations[currentJointIndex] =
//            applyTransformToZeroVec<double>(finalMat);
//    for (const int ind : joints[currentJointIndex].getKidsInd())
//        calculateMatrix(ind);
//}

Skeleton::Skeleton(
        const QVector<Joint> &joints,
        const QVector<JointTransform> &jointTransforms,
        const QVector<Matrix4<double> > &bindMatrices):
    joints(joints),
    jointTransforms(jointTransforms),
    bindMatrices(bindMatrices)
{

}

QVector<Matrix4<double> > Skeleton::computeGlobalMatrices(
        const QVector<Joint> &ierarch,
        const QVector<Matrix4<double>> &localMatrices) const
{
    QVector<int> rootIndexes;
    for (int jInd = 0; jInd < ierarch.length(); ++jInd)
        if (!ierarch[jInd].isMeshDependent() && ierarch[jInd].getPaterInd() < 0)
            rootIndexes << jInd;
    // now for each root go recursive
    QVector<Matrix4<double>> globalMatrices;
    for (const auto m : localMatrices)
        globalMatrices << Matrix4<double>::Identity();
    for (const int ind : rootIndexes)
        ;
}

Matrix4<double> Skeleton::computeLocalMatrix(
        const JointTransform &jointTrans,
        const Vector3<double> &paterInverseScale) const
{
    return translationMatrix<double>(jointTrans.getLocalTranslation())
            //* scalingMatrix<double>(paterInverseScale)
            * scalingMatrix<double>(jointTrans.getLocalScaling())
            * rotationMatrix<double>(jointTrans.getLocalTranslation());
}

bool Skeleton::computeGlobalMatrix(const QVector<Joint> &ierarch,
        const QVector<Matrix4<double>> &localMatrices,
        QVector<Matrix4<double> > &globalMatrices,
        const int currentJointIndex) const
{
    const int currentParentIndex = ierarch[currentJointIndex].getPaterInd();
    Matrix4<double> paterMat = (currentParentIndex < 0) ? Matrix4<double>::Identity() : globalMatrices[currentParentIndex];
    Matrix4<double> selfMat = localMatrices[currentJointIndex];
    Matrix4<double> finalMat = ierarch[currentJointIndex].setGlobalTransform(paterMat * selfMat);
    // calculate final global point
    jointTranslations[currentJointIndex] =
            applyTransformToZeroVec<double>(finalMat);
    for (const int ind : ierarch[currentJointIndex].getKidsInd())
        calculateMatrix(ind);
    return true;
}
