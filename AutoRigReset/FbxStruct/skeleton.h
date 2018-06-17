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
private:
    QVector<Joint> joints;
    QVector<int> rootIndexes;
};
}

#endif // FBXSTRUCT_SKELETON_H
