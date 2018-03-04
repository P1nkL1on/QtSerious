#include "rig.h"
#include "QVector2D"
#include "QVector4D"
#include "Derivable/dermatops.h"

#include "Eigen/LU"

using namespace DerivableVectorMatrixes;
using namespace DerOperations;
using Eigen::Matrix;

Rig::Rig()
{
    bindMesh = NULL;
    skeleton = NULL;
    skin = NULL;

    bendedMesh = NULL;
    modelColor = QColor(220,220,220,150);
    conturColor = QColor(100,100,100,50);
}

Rig::Rig(Mesh *mesh, Skeleton *skel, Skin *sk)
{
    bindMesh = mesh;
    skeleton = skel;
    skin = sk;

    bendedMesh = NULL;
    modelColor = QColor(220,220,220,150);
    conturColor = QColor(100,100,100,50);
}

void Rig::BendSkinToSkeleton()
{
    if (!skeleton->CalculateGlobalCoordForEachJointMatrix())
        return;

    //skin->GenerateProizAttends(bindMesh->vertexes, skeleton->getJointsGlobalTranslationsForSkin());

    Mesh* newMesh = new Mesh();
    newMesh->polygonIndexes = bindMesh->polygonIndexes;
    newMesh->polygonStartIndexes = bindMesh->polygonStartIndexes;
    int vertexesTransformed = 0;
    QVector<int> failedIndexes;

    QVector<Matrix<Derivable,4,4>> jointsTransMatrixes;
    for (int i = 0; i < skeleton->joints.length(); i++)
        jointsTransMatrixes << MakeDeriveRotationMatrix(skeleton->joints[i]->currentRotation) * skeleton->joints[i]->globalTransformMatrix;


    for (int currentVertexInd = 0; currentVertexInd < bindMesh->vertexes.length(); currentVertexInd ++){
        QVector<Matrix<Derivable,1,3>> bendedVariants;
        QVector<float> weightes;

        ///!! add
        Matrix<Derivable,4,4> nowGlobal;

        for (int clusterInd = 0; clusterInd < skin->clusterAttends.length(); clusterInd++){
            int vertexInClusterIndex = skin->clusterAttends[clusterInd].vertexIndex.indexOf(currentVertexInd);
            if (vertexInClusterIndex >= 0)
            {
                //qDebug() << currentVertexInd << "vertex in joint" << skin->clusterAttends[clusterInd].jointIndex << "is number" << currentVertexInd << "with wei" << skin->clusterAttends[clusterInd].weights[vertexInClusterIndex];
                //int jointBendInd = skin->clusterAttends[currentVertexInd].vertexIndex[jointInd];

                //Joint* bone = skeleton->joints[skin->clusterAttends[clusterInd].jointIndex];
                Matrix<Derivable,1,4> tmp =
                        MakeVector4From3(bindMesh->vertexes[currentVertexInd], Derivable(1))
                        * skin->clusterAttends[clusterInd].boneBindCoord
                        * jointsTransMatrixes[skin->clusterAttends[clusterInd].jointIndex];
                        //* (MakeDeriveRotationMatrix(bone->currentRotation) * bone->globalTransformMatrix)
                        //                        *
                        ;

                bendedVariants << Matrix<Derivable,1,3>(tmp(0,0),tmp(0,1),tmp(0,2));
                weightes << skin->clusterAttends[clusterInd].weights[vertexInClusterIndex];
            }
        }

        Matrix<Derivable,1,3> result = Matrix<Derivable,1,3>();
        float bendedSumm = 0;
        for (int curPoint = 0; curPoint < weightes.length(); curPoint++)
            bendedSumm += weightes[curPoint];


        // select the middle, based on wighthres
        for (int curPoint = 0; curPoint < bendedVariants.length(); curPoint++)
            result = result +  Derivable(weightes[curPoint]/ bendedSumm) * bendedVariants[curPoint] ;

        if (bendedVariants.length() > 0)
            vertexesTransformed ++;
        newMesh->vertexes << result;
    }
    if (bendedMesh != NULL)
        delete bendedMesh;
    bendedMesh = newMesh;
    return;
}

// ____________drawing____________
QVector<QVector2D> From3DTo2D (const QVector<Matrix<Derivable,1,3>> Input3D, const QMatrix4x4 view, const QMatrix4x4 perspective){
    QVector<QVector2D> resultPoints = {};
    for (int curPoint = 0; curPoint < Input3D.length(); curPoint++){
        QVector4D temp = QVector4D(Input3D[curPoint](0,0).getValue(), Input3D[curPoint](0,1).getValue(), Input3D[curPoint](0,2).getValue(), 1.0);
                temp = temp * view * perspective;
        resultPoints << QVector2D(temp.x() / temp.w(), temp.y() / temp.w());
    }
    return resultPoints;
}

bool ApplyScreen (int& x, int& y, const QVector2D point, const int screenWidth, const int screenHeight){
    x = screenWidth/2 + screenWidth/2 * (point[0]);
    y = screenHeight * 3 /4 - screenHeight/2 * (point[1]);
    if (x < 0 || y < 0 || x > screenWidth || y > screenHeight)
        return false;
    return true;
}

QPen ChangeQPainter (QColor clr, int width){
    QPen res;
    res.setWidth(width);
    res.setColor(clr);
    return res;
}

QVector<int> GetSortedIndex (const QVector<float> dists){
    QVector<int> res;
    QVector<float> tempDists = dists;
    //for (int i = tempDists.length() - 1; i >= 0 ; i--)
    for (int i = 0; i < tempDists.length(); i++)
        res << i;

    for (int i = 0; i < tempDists.length(); i++){
        int maxInd = -1; float maxValue = 0.0;
        for (int j = i; j < tempDists.length(); j++)
            if (tempDists[j] >= maxValue){maxInd = j; maxValue = tempDists[j];}
        tempDists[maxInd] = tempDists[i];
        tempDists[i] = maxValue;
        // ...
        int tempInd = res[i];
        res[i] = res[maxInd];
        res[maxInd] = tempInd;
    }
    return res;
}

float ang = 0;
QString Rig::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective, const int width, const int hei)
{
    //return QString();
    //    if (skeleton != NULL && skin != NULL && bindMesh != NULL){
    //        BendSkinToSkeleton();
    //    }
    // vertexes
    // ...

    QVector<QVector2D> Vertexes2D = From3DTo2D(bindMesh->vertexes, view, perspective);
    QVector<QVector2D> Vertexes2DBend;
    QVector<QPoint> appliedToScreenCoords;
    QVector<QPoint> appliedToScreenCoordsBended;

    if (bendedMesh != NULL)Vertexes2DBend = From3DTo2D(bendedMesh->vertexes, view, perspective);
    for (int curPoint = 0; curPoint < Vertexes2D.length(); curPoint++)
    {
        int x,y;
        painter->setPen(ChangeQPainter(QColor(0,105, 200,200), 1));
        if (ApplyScreen(x,y, Vertexes2D[curPoint], width, hei))
            //if (false)
            painter->drawPoint(x,y);
        appliedToScreenCoords << QPoint(x,y);
        // ...
        // bended mods
        if (bendedMesh != NULL)
        {
            int xb,yb;
            painter->setPen(ChangeQPainter(QColor(255,150,0,255), 2));
            ApplyScreen(xb,yb, Vertexes2DBend[curPoint], width, hei);
            painter->drawPoint(xb,yb);
            appliedToScreenCoordsBended << QPoint(xb, yb);
            //painter->setPen(ChangeQPainter(QColor(255,0,0,10), 1));
            //painter->drawLine(xb,yb,x,y);
        }
        // ...
    }
    // draw a mf polygons
    // ..................................................................................................................................
    Mesh* drawMesh = (bendedMesh == NULL)? bindMesh : bendedMesh;

    QVector<QPainterPath> polygonDrawArray;
    QVector<QColor> polygonColorArray;
    //QVector<float> distFromPolygonCentersToCamera;

    for (int currentPolygon = 0; currentPolygon < drawMesh->polygonStartIndexes.length() - 1; currentPolygon++){
        QPolygon poly;

        QVector<int> selectedIndexes;
        Matrix<Derivable, 1, 3> VertexInPolygonSumm = Matrix<Derivable, 1, 3>();
        int totalVertexInPolygonCount = 0;
        for (int selecInd = drawMesh->polygonStartIndexes[currentPolygon]; selecInd < drawMesh->polygonStartIndexes[currentPolygon + 1]; selecInd++){
            totalVertexInPolygonCount ++;
            VertexInPolygonSumm = VertexInPolygonSumm + drawMesh->vertexes[drawMesh->polygonIndexes[selecInd]];
            selectedIndexes << drawMesh->polygonIndexes[selecInd];

            poly << ((drawMesh == bindMesh)? appliedToScreenCoords/*Bended*/[drawMesh->polygonIndexes[selecInd]]
                     : appliedToScreenCoordsBended[drawMesh->polygonIndexes[selecInd]]);
        }
        //
        //int colorIntenese = (int)(((double)currentPolygon * 254.0)/(bindMesh->polygonStartIndexes.length() - 1));

        QPainterPath newPolyg; newPolyg.addPolygon(poly);
        polygonDrawArray << newPolyg;
        polygonColorArray << modelColor;//(QColor(colorIntenese,colorIntenese,colorIntenese));

        //distFromPolygonCentersToCamera << (QfromDer3( VertexInPolygonSumm )* (1.0 / totalVertexInPolygonCount)).distanceToPoint(*cameraCenter);
    }
    QBrush brush;
    painter->setPen(QPen(conturColor));

    //QVector<int> needPolygonInds = GetSortedIndex(distFromPolygonCentersToCamera);
    for (int cPath = 0, index = 0; cPath < polygonDrawArray.length(); cPath ++){
        index =  cPath;//needPolygonInds[cPath];
        brush = QBrush(polygonColorArray[index]);
        painter->fillPath(polygonDrawArray[index], brush);
        //painter->drawPath(polygonDrawArray[index]);
    }
    // ..................................................................................................................................

    if (skeleton == NULL)
        return QString("No skeleton detected");
    // joints
    // ...
    QVector<Matrix<Derivable, 1, 3>> Joints3D;
    for (int curJoint = 0; curJoint < skeleton->joints.length(); curJoint++){
        Matrix<Derivable, 1, 3> parentVect = Matrix<Derivable, 1, 3>(0,0,0),
                childVect = skeleton->getJointCoordByIndex(curJoint, parentVect);
        Joints3D << childVect << parentVect;
    }

    Vertexes2D = From3DTo2D(Joints3D, view,perspective);

    for (int curJoint = 0; curJoint < Vertexes2D.length() / 2; curJoint++)
    {

        int startGreen = 0;
        if (curJoint > 0 && curJoint < skeleton->joints.length()){
            Joint* j = skeleton->joints[curJoint];
            //while ( j->pater != NULL ){ startGreen += 30; j = j->pater;}
            if (startGreen > 255)startGreen = 255;
        }
        painter->setPen(ChangeQPainter(QColor(255,startGreen,0), 1));

        int xc,yc,xp,yp;
        ApplyScreen(xc,yc,Vertexes2D[curJoint * 2], width, hei);
        ApplyScreen(xp,yp,Vertexes2D[curJoint * 2 + 1], width, hei);
        painter->drawLine(xc,yc,xp,yp);
        painter->drawText(xc, yc,300,150,0, QString::number(curJoint));//+" " +skeleton->joints[curPoint]->name);// +", "+ QString::number(skeleton->joints[curPoint]->currentRotation.y()) +", "+ QString::number(skeleton->joints[curPoint]->currentRotation.z()));
        //painter->drawText(xc, yc,300,150,0, skeleton->joints[curJoint]->name +"\n"+ QString::number(Joints3D[curJoint * 2](0,0).getValue()) +"\n"+ QString::number(Joints3D[curJoint * 2](0,1).getValue()) +"\n"+ QString::number(Joints3D[curJoint * 2](0,2).getValue()));

        //        if (bendedMesh != NULL){
        //            int jointInd = curJoint, clusterInd = 0;
        //            while (skin->clusterAttends[clusterInd].jointIndex != jointInd && clusterInd < skin->clusterAttends.length())
        //                clusterInd++;
        //            if (clusterInd < skin->clusterAttends.length())
        //                for (int i = 0; i < skin->clusterAttends[clusterInd].vertexIndex.length(); i+= 10)
        //                {
        //                    int xv, yv;
        //                    ApplyScreen(xv,yv,Vertexes2DBend[skin->clusterAttends[clusterInd].vertexIndex[i]], width, hei);
        //                    painter->setPen(ChangeQPainter(QColor(70,220,0, 200), 1));
        //                    painter->drawLine(xc,yc,xv,yv);
        //                }
        //        }
    }


    QString logs = "";
    QVector<Matrix<Derivable,1,3>> jointLocalRots = skeleton->getJointsLocalRotationsForDrawing();
    for (int curJointInd = 0; curJointInd < skeleton->joints.length(); curJointInd++)
        logs += ("#" + QString::number(curJointInd) + " : ")
                + ToString(jointLocalRots[curJointInd]) + "      sc: "+ToString(skeleton->joints[curJointInd]->localScale)+"\n";
    painter->drawText(width, 40, 400, hei - 40,0, logs);//+" " +skeleton->joints[curPoint]->name);// +", "+ QString::number(skeleton->joints[curPoint]->currentRotation.y()) +", "+ QString::number(skeleton->joints[curPoint]->currentRotation.z()));

    //painter->end();
    return QString();
}
void Rig::ApplyBending(const Eigen::Matrix<Derivable, 1, 3> rootTrans, const QVector<Eigen::Matrix<Derivable, 1, 3> > newRotations, const QVector<Eigen::Matrix<Derivable, 1, 3> > newScales){
    skeleton->SetRootTranslation(rootTrans);
    skeleton->SetRotations(newRotations);
    skeleton->SetScales(newScales);
    BendSkinToSkeleton();
}

Derivable Rig::CompareWithMeshOnRotates(const Matrix<Derivable,1,3> rootTrans, const QVector<Matrix<Derivable,1,3>> newRotations, const QVector<Matrix<Derivable,1,3>> newScales, const Mesh *with)
{
    ApplyBending(rootTrans, newRotations, newScales);
    return bendedMesh->CompareWithAnotherMesh(with);
}
//CompareWithMeshOnRotatesCoord
QVector<Derivable> Rig::CompareWithMeshOnRotatesCoord(const Matrix<Derivable,1,3> rootTrans, const QVector<Matrix<Derivable,1,3>> newRotations, const QVector<Matrix<Derivable,1,3>> newScales, const Mesh *with)
{
    ApplyBending(rootTrans, newRotations, newScales);
    return bendedMesh->CompareWithAnotherMeshCoords(with);
}
