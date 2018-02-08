#include "stereometry.h"
//#include "iostream"
#include "qmath.h"

using namespace std;

    void Stereometry::Normalize (const QVector3D constBegin, QVector3D& movingEnd){
        movingEnd = Summ (constBegin, Mult (Resid(movingEnd, constBegin),
                                            1.0 / (Dist(constBegin, movingEnd) * 1.0)));
        // mE = cB + (mE - cB) / long
    }

    double Stereometry::Dist (const QVector3D vert1, const QVector3D vert2){
        return sqrt( pow(vert1.x() - vert2.x(), 2)+ pow( vert1.y() - vert2.y(), 2)+ pow (vert1.z() - vert2.z(), 2) );
    }

    double Stereometry::Angle (const QVector3D vertLeft, QVector3D vertCenter, QVector3D vertRight){

        if (vertLeft == vertCenter || vertCenter == vertRight || vertRight == vertLeft) return 0.0;

        return acos(Dot( Resid (vertCenter, vertLeft), Resid (vertRight, vertCenter) )
                    /(Dist(vertLeft, vertCenter) * Dist(vertRight, vertCenter)));
        // (a * b) / (|a| * |b|)
    }

    QVector3D Stereometry::Cross (const QVector3D a, const QVector3D b){
        return QVector3D( (a.y() * b.z() - a.z() * b.y()),
                         -(a.x() * b.z() - a.z() * b.x()),
                          (a.x() * b.y() - a.y() * b.x()));
    }

    double Stereometry::Dot (const QVector3D vector1, const QVector3D vector2){
        return vector1.x() * vector2.x() + vector1.y() * vector2.y() + vector1.z() * vector2.z();
    }

    QVector3D Stereometry::Summ (const QVector3D vector1, const QVector3D vector2){
        return QVector3D( vector1.x() + vector2.x(), vector1.y() + vector2.y(), vector1.z() + vector2.z() );
    }

    QVector3D Stereometry::Resid (const QVector3D vector1, const QVector3D vector2){
        return QVector3D( vector1.x() - vector2.x(), vector1.y() - vector2.y(), vector1.z() - vector2.z() );
    }

    QVector3D Stereometry::Mult (const QVector3D vector, const double Koef){
        return QVector3D(vector.x() * Koef, vector.y() * Koef, vector.z() * Koef);
    }   

    double Stereometry::Length(const QVector3D vert)
    {
        return sqrt( pow(vert.x(), 2)+ pow( vert.y(), 2)+ pow (vert.z(), 2) );
    }

    QVector3D Stereometry::BallecenterCoord(const QVector3D pointInPolygon, const QVector<QVector3D> points)
    {
        if (points.length() != 3) return {0,0,0};
        QVector3D squares = {0,0,0};
        float totalSquare = 0;

        for (int i = 0, j = 1, k = 2; i < 3; i++, j = (j+1)%3, k = (k+1)%3){
            squares[i] = Dist( points[j], pointInPolygon ) * Dist (points[k], pointInPolygon)
                        * sin (Angle( points[j], pointInPolygon, points[k] ));
            totalSquare += squares[i];
        }
        if (totalSquare < .0001)
            return {0, 0, 0};
        for (int i = 0; i < 3; i++)
            squares[i] /= totalSquare;
        return squares;
    }
    float Geron (const float a, const float b, const float c){
        float p = .5 * ( a + b + c);
        return sqrt(p * ( p - a ) * ( p - b ) * ( p - c ));
    }

    QVector3D Stereometry::BallecenterCoordGeron(const QVector3D pointInPolygon, const QVector<QVector3D> points)
    {
        if (points.length() != 3) return {0,0,0};
        float   d0 = Dist(points[0], pointInPolygon),
                d1 = Dist(points[1], pointInPolygon),
                d2 = Dist(points[2], pointInPolygon),
                d01 = Dist(points[0], points[1]),
                d12 = Dist(points[1], points[2]) ,
                d20 = Dist(points[2], points[0]);

        float s12 = Geron (d1,d2,d12),
              s20 = Geron (d2,d0,d20),
              s01 = Geron (d0,d1,d01),
              s012 = Geron (d01,d12,d20);
        if (abs (1000.0 * (s12 + s20 + s01 - s012)) > 2) return {-1,-1,-1};

        return QVector3D (s12 / s012, s20 / s012, (s012 - s12 - s20) / s012);
    }



    QVector3D Stereometry::Reflect(const QVector3D *rayStartXYZ, const QVector3D *intersectionPointXYZ, const QVector3D *normalEndXYZ)
    {
        double dd = Dist(*normalEndXYZ, *intersectionPointXYZ),
               scalar = Dist(*rayStartXYZ, *intersectionPointXYZ) * dd *
                        std::cos(M_PI - Angle(*rayStartXYZ, *intersectionPointXYZ, *normalEndXYZ));
        QVector3D res = Summ(*intersectionPointXYZ, Mult(Summ(Resid(*intersectionPointXYZ, *rayStartXYZ),
                                                   Mult(Resid(*normalEndXYZ, *intersectionPointXYZ), 2.0 * scalar)), 2.0));
        Normalize(*intersectionPointXYZ, res, 1 / 60.0);
        return res;
    }

    void Stereometry::Normalize(const QVector3D constBegin, QVector3D &movingEnd, float lengthMultyplier)
    {
            movingEnd = Summ (constBegin, Mult (Resid(movingEnd, constBegin),
                                                1.0 / (Dist(constBegin, movingEnd) * lengthMultyplier)));
            // mE = cB + (mE - cB) / long
    }
