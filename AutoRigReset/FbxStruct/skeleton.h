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
    void calculateMatrixes(const int variant);
    QVector<Df::Vector3<double>> jointTranslations;
    QVector<int> getKidsByIndex (const int index) const;
    int getPaterByIndex (const int index) const;
    QString getNameByIndex (const int index) const;
private:
    QVector<Joint> joints;
    QVector<int> rootIndexes;
    Df::Matrix4<double> getLocalMatrixByIndex (const int index);
    void calculateMatrix(const int currentJointIndex, const int variant);
};
}

#endif // FBXSTRUCT_SKELETON_H
