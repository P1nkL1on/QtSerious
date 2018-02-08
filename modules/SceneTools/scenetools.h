#ifndef SCENETOOLS_H
#define SCENETOOLS_H
#include "QVector"

#include "modelloader.h"

namespace SceneTools {
    QVector<unsigned int> triangulateMesh (const QVector<unsigned int> indVert, const QVector<unsigned int> vStart);
    QVector<QVector3D> calculateNormals(const QVector<QVector3D> verts, const QVector<unsigned int> polygonVertexIndexes,
                                        const QVector<unsigned int> polygonStartIndexes);
    QVector<QVector<double>> calculateParametricForAllPolygons (const QVector<QVector3D> verts,
                                             const QVector<unsigned int> polygonVertexIndexes);
//    QVector<QVector3D> rotateVertexes (const QVector<QVector4D> original, const float angle, const QVector3D os );
//    QVector<QVector3D> moveVertexes (const QVector<QVector4D> original, const QVector3D os );
//    QVector<QVector3D> scaleVertexes (const QVector<QVector4D> original, const float scal);
//    QVector<QVector3D> scaleVertexByVertex (const QVector<QVector4D> original, const float scal, const QVector3D os);

}

#endif // SCENETOOLS_H
