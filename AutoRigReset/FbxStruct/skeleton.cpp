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

void Skeleton::calculateMatrixes()
{
    jointTranslations.clear();
    for (int i = 0; i < joints.length(); i++)
        jointTranslations << Vector3<double>();
    for (const int rootInd : rootIndexes)
        calculateMatrix(rootInd);
}

Matrix4<double> Skeleton::getLocalMatrixByIndex(const int index)
{
    if (index < 0)
        return Matrix4<double>::Identity();
    return joints[index].getLocalTransform();
}

void Skeleton::calculateMatrix(const int currentJointIndex)
{
    qDebug() << QString("Matrix for bone %1").arg(currentJointIndex);

    jointTranslations[currentJointIndex] =
            kostilBoneDrawer(
                joints[currentJointIndex].setGlobalTransform(
                    joints[currentJointIndex].calculateLocalTransformMatrix()
                    * getLocalMatrixByIndex(joints[currentJointIndex].getPaterInd())));
    for (const int ind : joints[currentJointIndex].getKidsInd())
        calculateMatrix(ind);
}
