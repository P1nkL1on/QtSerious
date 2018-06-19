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
float xOffset = 900;
float yOffset = 900;
float totalScale = 5.0f;
float xScale = 1 * totalScale;
float yScale = -1 * totalScale;

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
bool everFixed = false;
void FixSize (const QVector<QVector3D> &orig){
    if (everFixed)return;
    int margX = 20, margY = 90;
    everFixed = true;
    float minX = 10000, minY = 10000, maxX = -10000, maxY = -10000;
    for (const auto v : orig){
        if (v.x() < minX) minX = v.x();
        if (v.x() > maxX) maxX = v.x();
        if (v.y() < minY) minY = v.y();
        if (v.y() > maxY) maxY = v.y();
    }

    double xK = (1800.0 - 2*margX) / (maxX - minX),
           yK = (1000.0 - 2*margY) / (maxY - minY),
           tK = (xK > yK)? yK : xK;
    totalScale = tK;
    xScale = 1 * totalScale;
    yScale = -1 * totalScale;
    xOffset = -minX * totalScale + margX;
    yOffset = maxY * totalScale + margY;
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
    FbxDrawing::everFixed = false;
}


void FbxStruct::Rig::repaint(QPainter *qp)
{
    //    // ================= CAUSE FUCK DRAWING, thats why =================

    qp->setPen(QPen(FbxDrawing::getNextColor(), 4));
    QVector<QVector3D> allVertexes;
    for (const auto mesh : meshes)
        for (const auto v : mesh.getVertices() )
            allVertexes << (v);
    QVector<Df::Vector3<double> > sks = skeleton.computeJointPositions();
    for (const auto v3 : sks)
        allVertexes << Df::makeQVectorFromVector3<double>(v3);

    FbxDrawing::FixSize(allVertexes);

    for (const auto v : allVertexes)
        qp->drawPoint(FbxDrawing::toPoint(v));

    //    skeleton.calculateMatrixes();
    QVector<QPoint> skeletonPoints;
    for (const auto v3 : sks)
        skeletonPoints << (FbxDrawing::toPoint(Df::makeQVectorFromVector3<double>(v3)));

    for (int jointIndex = 0; jointIndex < skeletonPoints.length(); ++jointIndex)
    {

        qp->setPen(QPen(Qt::blue, 3));
        int paterIndex = skeleton.getPaterByIndex(jointIndex);
        if (paterIndex >= 0)
            qp->drawLine(skeletonPoints[paterIndex], skeletonPoints[jointIndex]);
        qp->setPen(QPen(QColor(255,0,0, 10), 1));
        for (const auto cInd : skeleton.getClustersByJointIndex(jointIndex)){
            int meshInd = clusters[cInd].getMeshIndex();
            QVector<QVector3D> verts = meshes[meshInd].getVertices();

            for (const auto vInd : clusters[cInd].getVertIndices())
                qp->drawLine(skeletonPoints[jointIndex], FbxDrawing::toPoint(verts[vInd]));
        }
    }
    qp->setPen(QPen(Qt::blue, 6));
    for (const auto p : skeletonPoints)
        qp->drawPoint(p);

    qp->setPen(QPen(Qt::black, 1));
    for (int boneIndex =0; boneIndex < skeletonPoints.length(); ++boneIndex){
        QPoint tp(1150, 80 + boneIndex * 12);
        qp->drawText(tp, skeleton.getNameByIndex(boneIndex));
        qp->drawLine(tp, skeletonPoints[boneIndex]);
    }


    //    // =================================================================
    return;
}

int f = 0;
void FbxStruct::Rig::TestRotate()
{
    //skeleton.jointTransforms[4].setRotation(10, f++, 20);
    //return;
    //meshes[0].applyTransform(Df::translationMatrix<double>(Df::makeVector3fromDoubles<double>(10.0, 5.0, 0.0)));
}
