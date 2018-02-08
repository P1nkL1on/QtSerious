#ifndef CAMERA_H
#define CAMERA_H
#include "qmatrix4x4"
#include "qvector"
#include "QVector2D"

class Camera
{
private:
    QVector<QVector3D> cameraProps;
    double fieldAngle;
    double fieldDistance;
    double focusDistance;

    QMatrix4x4 perspectiveMatrix;
    QMatrix4x4 viewingMatrix;

    void calculateMatrixes(bool perspective, bool viewing);

    void move ( const QVector3D addCoordinates );
    void rotate (  const float angle, const QVector3D os );
    void scale(  const double scale );

public:
    QVector2D prevMousePlace;
    Camera( double fAngle, double fDist, double focusDist );

    QMatrix4x4 getPerspectiveMatrix() const;
    QMatrix4x4 getViewingMatrix() const;

    void transformByMouseMoving (const QVector2D currentMousePlace,
                                 const unsigned short actionType );
    QVector<QVector3D> GetCamInfo () const;
};

#endif // CAMERA_H
