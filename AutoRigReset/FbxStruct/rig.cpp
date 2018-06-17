#include "rig.h"
#include <QVector3D>
namespace FbxStruct{
namespace FbxDrawing {


const QVector<int> meshColors = {255,0,0,      0,255,0,    0,0,255,
                                 255,255,0,    0,255,255,  255,0,255,
                                 122,122,0,    122,0,122,  0,122,122,
                                 200,50,0,     50,200,0,   50,0,200,
                                 200,0,50,     0,50,200,   0,200,50 };
int currentColorIndex = 0;
const float xOffset = 700;
const float yOffset = 700;
const float totalScale = 3.0f;
const float xScale = 1 * totalScale;
const float yScale = -1 * totalScale;

QColor getNextColor (){
    currentColorIndex++;
    int index = (currentColorIndex) % (meshColors.length() / 3) ;
    return QColor::fromRgb(
                meshColors[index * 3],
            meshColors[index * 3 + 1],
            meshColors[index * 3 + 2]);
}

QPoint toPoint (const QVector3D &qv3){
    return QPoint(xOffset + qv3.x() * xScale,
                  yOffset + qv3.y() * yScale);
}

}
}

FbxStruct::Rig::Rig(const FbxStruct::Skeleton &skeleton,
                    const QVector<FbxStruct::Cluster> &clusters,
                    const QVector<FbxStruct::Mesh> &meshes):
    skeleton(skeleton),
    clusters(clusters),
    meshes(meshes)
{

}

void FbxStruct::Rig::repaint(QPainter *qp) const
{
    //    // ================= CAUSE FUCK DRAWING, thats why =================
    for (const auto mesh : meshes){
        qp->setPen(QPen(FbxDrawing::getNextColor()));
        for (const auto v : mesh.getVertices() )
            qp->drawPoint(FbxDrawing::toPoint(v));
    }

    skeleton.calculateMatrixes();

    //    qp->setPen(QPen(Qt::blue, 6));
    //    for (int i = 0; i < fbxJoints.length(); ++i){
    //        int curJind = i;
    //        Matrix4<double> now = Matrix4<double>::Identity();
    //        do{
    //            now = now * fbxJoints[curJind].getBindTransform();
    //            curJind = fbxJoints[curJind].getPaterInd();
    //        }while (curJind >= 0);

    //        Vector3<double> fin = Df::kostilBoneDrawer(now);
    //        qp->drawPoint(xOffset + fin(0,0) * xScale, yOffset + fin(1,0) * yScale);
    //    }

    //    // =================================================================
    return;
}
