#include "skin.h"

using namespace DerOperations;
using Eigen::Matrix;


Skin::Skin()
{
    jointIndexes = QVector<int>();
    vertIndexes = QVector<QVector<int>>();
    vertWeightes = QVector<QVector<float>>();
}

void Skin::addInfo(int jointID, QVector<int> Indexes, QVector<float> Weightes)
{
    jointIndexes << jointID;
    vertIndexes << Indexes;
    vertWeightes << Weightes;
}

void Skin::GenerateAttends(const QVector<Eigen::Matrix<Derivable, 1, 3>> &vertexes, QVector<Matrix<Derivable,4,4>> globalJointCoords)
{
    clusterAttends.clear();
    for (int curJoint = 0; curJoint < jointIndexes.length(); curJoint++){
        AttendedCluster c = AttendedCluster(globalJointCoords[jointIndexes[curJoint]], jointIndexes[curJoint]);
        c.vertexIndex = vertIndexes[curJoint];
        c.weights = vertWeightes[curJoint];
        clusterAttends << c;
    }
}
