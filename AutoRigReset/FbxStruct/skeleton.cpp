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
    for (int i = 0; i < joints.length(); ++i)
        jointTranslations << Vector3<double>();
    for (const int rootInd : rootIndexes)
        calculateMatrix(rootInd);
    for (int i = 0; i < jointTranslations.length(); ++i)
        qDebug() << i << joints[i].getJointName() <<
                    jointTranslations[i](0,0)<<
                    jointTranslations[i](1,0)<<
                    jointTranslations[i](2,0);
}

QVector<int> Skeleton::getKidsByIndex(const int index)
{
    if (index < 0)
        return {};
    return joints[index].getKidsInd();
}

int Skeleton::getPaterByIndex(const int index)
{
    if (index < 0)
        return -1;
    return joints[index].getPaterInd();
}

Matrix4<double> Skeleton::getLocalMatrixByIndex(const int index)
{
    if (index < 0)
        return Matrix4<double>::Identity();
    return joints[index].getLocalTransform();
}

void Skeleton::calculateMatrix(const int currentJointIndex)
{
    //qDebug() << QString("Matrix for bone %1").arg(currentJointIndex);
    jointTranslations[currentJointIndex] =
            kostilBoneDrawer<double>(
                joints[currentJointIndex].getBindTransform());
    for (const int ind : joints[currentJointIndex].getKidsInd())
        calculateMatrix(ind);
}
