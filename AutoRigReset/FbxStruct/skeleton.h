#ifndef FBXSTRUCT_SKELETON_H
#define FBXSTRUCT_SKELETON_H


#include "DerStruct/defines.h"
#include "FbxStruct/joint.h"

namespace FbxStruct{

class Skeleton
{
public:
    Skeleton() = default;
    Skeleton(const QVector<Joint> &joints);
    void calculateMatrixes();
    QVector<Df::Vector3<double>> jointTranslations;
    QVector<int> getKidsByIndex (const int index);
    int getPaterByIndex (const int index);
private:
    QVector<Joint> joints;
    QVector<int> rootIndexes;
    Df::Matrix4<double> getLocalMatrixByIndex (const int index);
    void calculateMatrix(const int currentJointIndex);
};
}

#endif // FBXSTRUCT_SKELETON_H
