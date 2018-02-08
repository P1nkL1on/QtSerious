#ifndef MODEL_H
#define MODEL_H
#include "QVector";
#include "graphicsobject.h"
#include "QImage"
#include "qvector2d.h"

namespace ModelStructs {

    class Model : public GraphicsObjectStruct::GraphicsObject
    {
    private:

        QString DrawItSelf(QVector<QVector2D> &points,const QVector<QVector3D> vertGiven,
                           const QMatrix4x4 view, const QMatrix4x4 perspective);

    public:
        QColor modelColor;
        Model();
        QVector<QVector3D> vertexes;
        QVector<QVector2D> vertexes_texture;
        QVector<unsigned int> polygon_vertex_indexes;
        QVector<unsigned int> polygon_texture_vertex_indexes;
        QVector<unsigned int> polygon_start;
        QVector<QVector3D> vertex_normals;
        QVector<QVector<double>> parametric;
        QImage textureMain;
        QImage normalMap;
        QImage mirrorMap;

        QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                               const int width, const int hei) override;

        QVector3D* RayIntersection (const QVector3D *rayStart,
                                     const QVector3D *rayFinish, const unsigned int polygonIndex) const override;

        virtual QVector<QVector3D> GetVertexes (unsigned int polygonIndex) const override;

        virtual QVector<QVector3D> GetVertexNormals (unsigned int polygonIndex) const override;

        virtual QColor GetPixelFromTexture(unsigned int textureIndex, QVector2D *interPoint) const override;

        virtual QVector<QVector2D> GetTextureVertexes (unsigned int polygonIndex) const override;
    };
}

#endif // MODEL_H
