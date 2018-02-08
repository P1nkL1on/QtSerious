#ifndef TRANSFORMS_H
#define TRANSFORMS_H
#include "QMatrix4x4"
#include "QVector3D"
#include "QVector4D"
#include "QVector"

namespace Transforms{

    QVector3D apllyVertexMatrix (const QVector3D original, const QMatrix4x4 rotateMatrix);

    QVector<QVector3D> moveVertexes ( const QVector<QVector3D> original, const QVector3D addCoordinates );
    QVector<QVector3D> rotateVertexes ( const QVector<QVector3D> original, const float angle, const QVector3D os );
    QVector<QVector3D> scaleVertexes ( const QVector<QVector3D> original, const double scale );
    QVector<QVector3D> scaleVertexesByCenter ( const QVector<QVector3D> original, const double scale, const QVector3D center );
}

#endif // TRANSFORMS_H
