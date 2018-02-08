#ifndef STEREOMETRY_H
#define STEREOMETRY_H
#include "QVector3D"
#include "QVector"

namespace Stereometry {

    void Normalize (const QVector3D constBegin, QVector3D& movingEnd);
    void Normalize(const QVector3D constBegin, QVector3D &movingEnd, float lengthMultyplier);
    double Dist (const QVector3D vert1, const QVector3D vert2);
    double Length (const QVector3D vert);
    double Angle (const QVector3D vertLeft, QVector3D vertCenter, QVector3D vertRight);
    QVector3D Cross (const QVector3D vector1, const QVector3D vector2);
    double Dot (const QVector3D vector1, const QVector3D vector2);
    QVector3D Summ (const QVector3D vector1, const QVector3D vector2);
    QVector3D Resid (const QVector3D vector1, const QVector3D vector2);
    QVector3D Mult (const QVector3D vector, const double Koef);

    QVector3D BallecenterCoord (const QVector3D pointInPolygon, const QVector<QVector3D> points);
    QVector3D BallecenterCoordGeron(const QVector3D pointInPolygon, const QVector<QVector3D> points);
    QVector3D Reflect(const QVector3D* rayStartXYZ, const QVector3D* intersectionPointXYZ,
                    const QVector3D* normalEndXYZ);
}


#endif // STEREOMETRY_H
