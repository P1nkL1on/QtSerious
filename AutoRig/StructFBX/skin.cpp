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

void Skin::GenerateAttends(const QVector<Matrix<Derivable,1,3>> vertexes, QVector<Matrix<Derivable,1,3>> globalJointCoords)
{
    vertAttends.clear();
    AttendedVertex vts[vertexes.length()];
    for (int v = 0; v < vertexes.length(); v++)
        vts[v] = AttendedVertex();

    for (int curJoint = 0; curJoint < jointIndexes.length(); curJoint++)
        for (int vertInd = 0; vertInd < vertIndexes[curJoint].length(); vertInd ++){
            vts[vertIndexes[curJoint][vertInd]].jointIndexs << jointIndexes[curJoint];
            vts[vertIndexes[curJoint][vertInd]].weights << vertWeightes[curJoint][vertInd];
            vts[vertIndexes[curJoint][vertInd]].localJointCoords << globalJointCoords[jointIndexes[curJoint]] - vertexes[vertIndexes[curJoint][vertInd]];
        }
    for (int v = 0; v < vertexes.length(); v++)
        vertAttends << vts[v];
}

void Skin::GenerateProizAttends(const QVector<Matrix<Derivable,1,3>> vertexes, QVector<Eigen::Matrix<Derivable, 1, 3> > globalJointCoords)
{
    for (int v = 0; v < vertAttends.length(); v++){
        for (int ljc = 0; ljc < vertAttends[v].localJointCoords.length(); ljc++)
            for (int i = 0; i < 3; i++)
            // only change a diff
                vertAttends[v].localJointCoords[ljc](0,i).setPrValue( globalJointCoords[vertAttends[v].jointIndexs[ljc]](0,i).getProiz());// = globalJointCoords[vertAttends[v].jointIndexs[ljc]] - vertexes[v];

    }
}
