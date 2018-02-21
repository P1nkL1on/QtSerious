#ifndef SKIN_H
#define SKIN_H
#include "QVector"
#include "structfbx.h"


class Skin
{
    QVector<int> jointIndexes;
    QVector<QVector<int>> vertIndexes;
    QVector<QVector<float>> vertWeightes;

public:
    Skin();
    bool getInfoByJointIndex (int jointID, QVector<int> &outIndexes, QVector<float> &outWeightes) const;
    void addInfo (int jointID, QVector<int> Indexes, QVector<float> Weightes );
    void GenerateAttends (const QVector<Eigen::Matrix<DerOperations::Derivable,1,3>>& vertexes, QVector<Eigen::Matrix<DerOperations::Derivable,4,4>> globalJointCoords);
    // ... final view ...
    QVector<AttendedCluster> clusterAttends;
};

#endif // SKIN_H
