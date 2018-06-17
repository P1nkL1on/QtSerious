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
    qDebug() << "Gooo";
    for (const int rootInd : rootIndexes)
        calculateMatrixe(rootInd);
}

void Skeleton::calculateMatrixe(const int currentJointIndex)
{
    qDebug() << QString("Matrix for bone %1").arg(currentJointIndex);

    joints[currentJointIndex].calculateLocalTransformMatrix();
    for (const int ind : joints[currentJointIndex].getKidsInd())
        calculateMatrixe(ind);
}
