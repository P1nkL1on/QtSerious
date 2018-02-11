#include "model.h"
#include <QVector2D>
#include "QVector3D"
#include "iostream"
#include "stereometry.h"
#include "cmath"

using namespace Stereometry;

namespace ModelStructs{
Model::Model()
{
    modelColor = QColor(Qt::black);
    polygon_start = {0};
}

QString Model::DrawItSelf(QVector<QVector2D> &resultPoints, const QVector<QVector3D> vertGiven,
                          const QMatrix4x4 view, const QMatrix4x4 perspective){

    for (int i = 0; i < vertGiven.length(); i++){
        QVector4D vertexCoords = QVector4D(vertGiven[i].x(), vertGiven[i].y(), vertGiven[i].z(), 1.0 ) ;

        vertexCoords = vertexCoords * view * perspective;
        resultPoints << QVector2D (vertexCoords[0] / vertexCoords[3], vertexCoords[1] / vertexCoords[3]);
    }
    return QString();
}

QVector2D toScrCoords (const QVector2D point, const int screenWidth, const int screenHeight){
    double size = 1.0;
    return QVector2D (screenWidth/2 + (screenWidth/2 * (point[0])) * size, screenHeight * 3 /4 - (screenHeight/2 * (point[1])) * size);
}

QString Model::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective,
                              const int width, const int height){

    // first get a point array
    QVector<QVector2D> resPoints = {};
    QString err = DrawItSelf(resPoints, vertexes, view, perspective);

    QVector<QVector2D> normalPoints = {};
    QString errNormal = DrawItSelf( normalPoints, vertex_normals, view, perspective);

    if (!err.isEmpty())    // force quit
        return err;

    QPen pen;
    pen.setWidth(2);
    pen.setColor(modelColor);//((currentPolygon == polygonSelectedIndex)? Qt::red : modelColor);
    painter->setPen(pen);

    for (int currentVert = 0; currentVert < vertexes.length(); currentVert ++){
        QVector2D res = toScrCoords(resPoints[currentVert], width, height);
        painter->drawPoint((int)res[0],(int)res[1]);
    }


    for (int currentPolygon = 0; currentPolygon < polygon_start.length() - 1; currentPolygon++){
        QVector2D lastPointInPolygon = toScrCoords(resPoints[polygon_vertex_indexes[polygon_start[currentPolygon + 1] - 1] - 1], width, height);
        for (int currentVertInd = polygon_start[currentPolygon]; currentVertInd < polygon_start[currentPolygon + 1]; currentVertInd++){

            QVector2D res = toScrCoords(resPoints[polygon_vertex_indexes[currentVertInd] - 1], width, height);
            painter->drawLine((int)res[0],(int)res[1], (int)lastPointInPolygon[0], (int) lastPointInPolygon[1]);
            lastPointInPolygon = res;
        }
    }
    // draw normals
    pen.setColor(Qt::red);
    painter->setPen(pen);
    if (false)
    if (errNormal.isEmpty() && normalPoints.length() > 0)
        for (int currentNormal = 0; currentNormal < vertex_normals.length(); currentNormal++){
            QVector2D point = toScrCoords(resPoints[currentNormal], width, height),
                      pointN = toScrCoords(normalPoints[currentNormal], width, height);
            painter->drawLine((int)point[0],(int)point[1], (int)pointN[0], (int) pointN[1]);
        }
    if (!textureMain.isNull())
        painter->drawImage(QRectF(0.0,20.0, 100.0, 100.0), textureMain);
    if (!normalMap.isNull())
        painter->drawImage(QRectF(0.0,120.0, 100.0, 100.0), normalMap);
    if (!mirrorMap.isNull())
        painter->drawImage(QRectF(0.0,220.0, 100.0, 100.0), mirrorMap);

    return QString();
}

double GetValue (const QVector3D* point, const QVector<double> parametric){
    Q_ASSERT(parametric.length() == 4);
    return parametric[0] * point->x() + parametric[1] * point->y() + parametric[2] * point->z() + parametric[3];
}

QVector3D* Model::RayIntersection(const QVector3D *rayStart,
                                   const QVector3D *rayFinish, const unsigned int polygonIndex) const{
    QVector<QVector3D> trianglePoints = {vertexes[-1 + polygon_vertex_indexes[polygonIndex * 3]],
                                        vertexes[-1 + polygon_vertex_indexes[polygonIndex * 3 + 1]],
                                        vertexes[-1 + polygon_vertex_indexes[polygonIndex * 3 + 2]]};
    QVector3D* intersection = NULL;
    //__________
    //qDebug() << polygonIndex << parametric.length();
    double distance = Dist(*rayStart, *rayFinish);
    QVector3D cosPoint = Mult (Resid (*rayFinish, *rayStart), 1.0/distance);

    double
        fromValue = GetValue(rayStart, parametric[polygonIndex]),
        cosValue = GetValue(&cosPoint, parametric[polygonIndex]) - parametric[polygonIndex][3];
    if (cosValue > .00001 || cosValue < -.00001)
    {
        QVector3D fin = Mult(cosPoint, -(fromValue / (double)cosValue));
        intersection = new QVector3D();
        *intersection = Summ(*rayStart, fin);
    }
    //__________
    if (intersection == NULL)
        return NULL;

    // TRUE == ANGLES, FALSE = BALLICENTER
    if (true){
        if (!(Dist(*intersection, *rayStart ) > .0001 && Dist(*rayFinish, *intersection ) < distance + .0001))
            return NULL;
        double angleDoff = M_PI - (
                           Angle(trianglePoints[0], *intersection, trianglePoints[1])
                         + Angle(trianglePoints[1], *intersection, trianglePoints[2])
                         + Angle(trianglePoints[2], *intersection, trianglePoints[0]));
        if (angleDoff > -.001 && angleDoff < .001){
            QVector3D* ballicenter = new QVector3D();
            *ballicenter =
                BallecenterCoordGeron(*intersection, trianglePoints);
            return ballicenter;
        }
            //return intersection;

    }
    return NULL;

}

    QVector<QVector3D> Model::GetVertexes(unsigned int polygonIndex) const
    {
        QVector<QVector3D> verts = { vertexes[polygon_vertex_indexes[polygonIndex * 3 + 0] - 1],
                                     vertexes[polygon_vertex_indexes[polygonIndex * 3 + 1] - 1],
                                     vertexes[polygon_vertex_indexes[polygonIndex * 3 + 2] - 1]};
        return verts;
    }

    QVector<QVector3D> Model::GetVertexNormals(unsigned int polygonIndex) const
    {
        QVector<QVector3D> verts = { vertex_normals[polygon_vertex_indexes[polygonIndex * 3 + 0] - 1],
                                     vertex_normals[polygon_vertex_indexes[polygonIndex * 3 + 1] - 1],
                                     vertex_normals[polygon_vertex_indexes[polygonIndex * 3 + 2] - 1]};
        return verts;
    }



    QColor Model::GetPixelFromTexture(unsigned int textureIndex, QVector2D *interPoint) const
    {
        QImage texture;
        switch (textureIndex){
            case 0:
                if (textureMain.isNull())
                    return Qt::white;
                texture = textureMain; break;
            case 1:
            if (normalMap.isNull())
                return QColor(127, 127, 127);
                texture = normalMap; break;
            case 2:
            if (mirrorMap.isNull())
                return QColor(0,0,0);
                texture = mirrorMap; break;
            default:
                return QColor(Qt::white);
                break;
        }
        return (texture.pixel((int)(interPoint->x() * (texture.width() - 1)),
                              (int)((1 - interPoint->y()) * (texture.height() -1))));
    }

    QVector<QVector2D> Model::GetTextureVertexes(unsigned int polygonIndex) const
    {
        QVector<QVector2D> verts = { vertexes_texture[polygon_texture_vertex_indexes[polygonIndex * 3 + 0] - 1],
                                     vertexes_texture[polygon_texture_vertex_indexes[polygonIndex * 3 + 1] - 1],
                                     vertexes_texture[polygon_texture_vertex_indexes[polygonIndex * 3 + 2] - 1]};
        return verts;
    }

}

