#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H
#include "QVector"
#include "QVector2D"
#include "QMatrix4x4"
#include "QPainter"
namespace GraphicsObjectStruct  {

class GraphicsObject
{
public:
    GraphicsObject();
    virtual QString ApplyDrawToCanvas (QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                                    const int width, const int hei) = 0;
    virtual QVector3D* RayIntersection (const QVector3D *rayStart, const QVector3D *rayFinish,
                                         const unsigned int polygonIndex) const;
    virtual QVector<QVector3D> GetVertexes (unsigned int polygonIndex) const;
    virtual QVector<QVector3D> GetVertexNormals (unsigned int polygonIndex) const;
    virtual QVector<QVector2D> GetTextureVertexes (unsigned int polygonIndex) const;
    virtual QColor GetPixelFromTexture (unsigned int textureIndex, QVector2D* interPoint) const;
};
}
#endif // GRAPHICSOBJECT_H
