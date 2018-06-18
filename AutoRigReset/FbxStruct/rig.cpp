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

void FbxStruct::Rig::repaint(QPainter *qp)
{
    //    // ================= CAUSE FUCK DRAWING, thats why =================
    for (const auto mesh : meshes){
        qp->setPen(QPen(FbxDrawing::getNextColor(), 2));
        for (const auto v : mesh.getVertices() )
            qp->drawPoint(FbxDrawing::toPoint(v));
    }
    QVector<QColor> vClrs = {Qt::red, Qt::blue};
    for (int v = 0; v < 2; ++v){
        skeleton.calculateMatrixes(v);

        qp->setPen(QPen(vClrs[v], 3));
        QVector<QPoint> skeletonPoints;
        for (const auto v3 : skeleton.jointTranslations)
            skeletonPoints << (FbxDrawing::toPoint(Df::makeQVectorFromVector3<double>(v3)));

        for (int jointIndex = 0; jointIndex < skeletonPoints.length(); ++jointIndex)
        {
            int paterIndex = skeleton.getPaterByIndex(jointIndex);
            if (paterIndex >= 0)
                qp->drawLine(skeletonPoints[paterIndex], skeletonPoints[jointIndex]);
        }
        qp->setPen(QPen(vClrs[v], 6));
        int boneIndex = 0;
        for (const auto p : skeletonPoints)
            qp->drawPoint(p);

        qp->setPen(QPen(Qt::black, 1));
        for (int boneIndex =0; boneIndex < skeletonPoints.length(); ++boneIndex){
            QPoint tp(1150, 80 + boneIndex * 12);
            qp->drawText(tp, skeleton.getNameByIndex(boneIndex));
            qp->drawLine(tp, skeletonPoints[boneIndex]);
        }
    }

    //    // =================================================================
    return;
}

void FbxStruct::Rig::TestRotate()
{
    meshes[0].applyTransform(Df::translationMatrix<double>({1, .5, 0}));
    qDebug() << "moved";
}
