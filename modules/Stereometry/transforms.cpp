#include "transforms.h"
#include "stereometry.h"

QVector3D Transforms::apllyVertexMatrix(const QVector3D original, const QMatrix4x4 originalMatrix)
{
    QVector4D coords = QVector4D( original.x(), original.y(), original.z(), 1.0 );
    coords = coords * originalMatrix;
    return QVector3D(coords[0] / coords[3], coords[1] / coords[3], coords[2] / coords[3]);
}

QVector<QVector3D> Transforms::rotateVertexes(const QVector<QVector3D> original, const float angle, const QVector3D os)
{
    QVector<QVector3D> res = {};
    QMatrix4x4 rot;
    rot.rotate(angle, os);
    for (int currentPoint = 0; currentPoint < original.length(); currentPoint++)
        res << Transforms::apllyVertexMatrix(original[currentPoint], rot);
    return res;
}


QVector<QVector3D> Transforms::moveVertexes(const QVector<QVector3D> original, const QVector3D addCoordinates)
{
    QVector<QVector3D> res = {};
    QMatrix4x4 mov;
    mov.translate(addCoordinates);
    for (int currentPoint = 0; currentPoint < original.length(); currentPoint++)
        res << Transforms::apllyVertexMatrix(original[currentPoint], mov);
    return res;
}

QVector<QVector3D> Transforms::scaleVertexes(const QVector<QVector3D> original, const double scale)
{
    QVector<QVector3D> res = {};
    QMatrix4x4 mov;
    mov.scale((float)scale);
    for (int currentPoint = 0; currentPoint < original.length(); currentPoint++)
        res << Transforms::apllyVertexMatrix(original[currentPoint], mov);
    return res;
}

QVector<QVector3D> Transforms::scaleVertexesByCenter(const QVector<QVector3D> original, const double scale, const QVector3D center)
{
    QVector<QVector3D> previousCords = original;

    for (int currentPoint = 0; currentPoint < original.length(); currentPoint++)
        previousCords[currentPoint] = Stereometry::Resid(previousCords[currentPoint] , center);

    QMatrix4x4 mov;
    mov.scale((float)scale);

    QVector<QVector3D> res = {};
    for (int currentPoint = 0; currentPoint < original.length(); currentPoint++)
        res << Stereometry::Summ ( center, Transforms::apllyVertexMatrix(previousCords[currentPoint], mov));

    return res;
}
