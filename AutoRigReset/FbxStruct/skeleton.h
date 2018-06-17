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
private:
    QVector<Joint> joints;
    QVector<int> rootIndexes;
    void calculateMatrixe(const int currentJointIndex);
};
}

#endif // FBXSTRUCT_SKELETON_H
