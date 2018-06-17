#ifndef FBXSTRUCT_RIG_H
#define FBXSTRUCT_RIG_H

#include "FbxStruct/cluster.h"
#include "FbxStruct/mesh.h"
#include "FbxStruct/skeleton.h"

#include <QPainter>

namespace FbxStruct{


class Rig
{
public:
    Rig() = default;
    Rig(const Skeleton &skeleton,
        const QVector<Cluster> &clusters,
        const QVector<Mesh> &meshes);
    void repaint (QPainter *qp);
private:
    Skeleton skeleton;
    QVector<Cluster> clusters;
    QVector<Mesh> meshes;
};

}

#endif // FBXSTRUCT_RIG_H
