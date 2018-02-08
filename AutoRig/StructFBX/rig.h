#ifndef RIG_H
#define RIG_H

#include "graphicsobject.h"
#include "mesh.h"
#include "skeleton.h"
#include "skin.h"

class Rig : public GraphicsObjectStruct::GraphicsObject{
public:
    Mesh* bindMesh;
    Skeleton* skeleton;
    Skin* skin;
    Mesh* bendedMesh;

    Rig();
    Rig(Mesh *mesh, Skeleton *skel, Skin *sk);

    QVector3D* cameraCenter;
    QColor modelColor;
    QColor conturColor;

    // change a mesh w/t a skeleton n skin
    void BendSkinToSkeleton ();
    // drawing to canvas
    QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                           const int width, const int hei) override;
    DerOperations::Derivable CompareWithMeshOnRotates(const Eigen::Matrix<DerOperations::Derivable, 1, 3> rootTrans, const QVector<Eigen::Matrix<DerOperations::Derivable, 1, 3> > newRotations, const QVector<DerOperations::Derivable> newScales, const Mesh* with);
    QVector<DerOperations::Derivable> CompareWithMeshOnRotatesCoord (const Eigen::Matrix<DerOperations::Derivable, 1, 3> rootTrans, const QVector<Eigen::Matrix<DerOperations::Derivable, 1, 3> > newRotations, const QVector<DerOperations::Derivable> newScales, const Mesh* with);
};

#endif // RIG_H
