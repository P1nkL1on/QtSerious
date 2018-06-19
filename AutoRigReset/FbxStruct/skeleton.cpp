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

QVector<Matrix4<double> > Skeleton::computeLocalMatrices(
        const QVector<JointTransform> &jointTrans,
        const QVector<Vector3<double> > &paterInverseScales)
{
    Q_ASSERT (jointTrans.length() == paterInverseScales.length());
    QVector<Matrix4<double>> res(jointTrans.length());
    for (int ind = 0; ind < jointTrans.length(); ++ind)
        res[ind] = computeLocalMatrix(jointTrans[ind], paterInverseScales[ind]);
    return res;
}

QVector<Matrix4<double> > Skeleton::computeGlobalMatrices(
        const QVector<Joint> &joints,
        const QVector<Matrix4<double>> &localMatrices)
{
    using namespace Eigen;
    QVector<int> rootIndices;
    for (int jInd = 0; jInd < joints.length(); ++jInd)
        if (!joints[jInd].isMeshDependent() && joints[jInd].getPaterInd() < 0)
            rootIndices << jInd;
    // now for each root go recursive
    QVector<Matrix4<double>> globalMatrices(localMatrices.size(), Matrix4d::Identity());
    for (const int ind : rootIndices)
        computeGlobalMatrix(joints, localMatrices, globalMatrices, ind);
    return globalMatrices;
}

Matrix4<double> Skeleton::computeLocalMatrix(
        const JointTransform &jointTrans,
        const Vector3<double> &paterInverseScale)
{
    return translationMatrix<double>(jointTrans.getLocalTranslation())
            //* scalingMatrix<double>(paterInverseScale)
            * scalingMatrix<double>(jointTrans.getLocalScaling())
            * rotationMatrix<double>(jointTrans.getLocalRotation());
}

bool Skeleton::computeGlobalMatrix(const QVector<Joint> &joints,
                                   const QVector<Matrix4<double>> &localMatrices,
                                   QVector<Matrix4<double> > &globalMatrices,
                                   const int currentJointIndex)
{
    using namespace Eigen;
    qDebug() << "Global calculator called for index " + QString::number(currentJointIndex);
    const int currentParentIndex = joints[currentJointIndex].getPaterInd();
    const Matrix4<double> paterMat = (currentParentIndex < 0) ? Matrix4d::Identity() : globalMatrices[currentParentIndex];
    const Matrix4<double> selfMat  = localMatrices[currentJointIndex];
    globalMatrices[currentJointIndex] = paterMat * selfMat;

    for (const int ind : joints[currentJointIndex].getKidsInd())
        computeGlobalMatrix(joints, localMatrices, globalMatrices, ind);
    return true;
}

QVector<Vector3<double> > Skeleton::computeJointPositions() const
{
    QVector<Vector3<double>> inverseParentScales;
    for (const auto j : joints){
        int paterInd = j.getPaterInd();
        inverseParentScales << ((paterInd < 0) ? makeUnitVector3<double>() : jointTransforms[paterInd].getInverseScaling());
    }
    QVector<Matrix4<double>> globalTransforms
            = computeGlobalMatrices(
                joints,
                computeLocalMatrices(jointTransforms, inverseParentScales));
    QVector<Vector3<double> > res;
    for (const auto mat : globalTransforms)
        res << applyTransformToZeroVec<double>(mat);
    return res;
}

int Skeleton::getPaterByIndex(const int index) const
{
    if (index < 0)
        return -1;
    return joints[index].getPaterInd();
}

QString Skeleton::getNameByIndex(const int index) const
{
    return joints[index].getJointName();
}

QVector<int> Skeleton::getClustersByJointIndex(const int index) const
{
    return joints[index].getClusterInds();
}
