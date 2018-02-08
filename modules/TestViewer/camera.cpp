#include "camera.h"
#include "QMatrix4x4"
#include "QVector4D"
#include "transforms.h"
#include "QVector2D"
#include "qdebug.h"
#include "stereometry.h"
#include "qmath.h"

Camera::Camera(double fAngle, double fDist, double focusDist)
{
    // make a props
    cameraProps = { QVector3D(-10, 0, 0), QVector3D(0,0,0), QVector3D(-10, 1, 0), QVector3D(-10, 0, -1) };
    // make a focus props
    fieldAngle = fAngle / 180.0 * M_PI;
    fieldDistance = fDist;
    focusDistance = focusDist;
    calculateMatrixes(true, true);
    prevMousePlace = {0,0};
}

void Camera::calculateMatrixes(bool perspective, bool viewing)
{
    QVector3D up(cameraProps[2].x() - cameraProps[0].x(), cameraProps[2].y() - cameraProps[0].y(), cameraProps[2].z() - cameraProps[0].z());
    QVector3D left(cameraProps[3].x() - cameraProps[0].x(), cameraProps[3].y() - cameraProps[0].y(), cameraProps[3].z() - cameraProps[0].z());
    QVector3D forward( cameraProps[0].x() - cameraProps[1].x(), cameraProps[0].y() - cameraProps[1].y(), cameraProps[0].z() - cameraProps[1].z() );
    // creating a view matrix

    if (viewing)

        viewingMatrix = QMatrix4x4 ( left.x(), up.x(), forward.x(), 0,
                                    left.y(), up.y(), forward.y(), 0,
                                    left.z(), up.z(), forward.z(), 0,
                                    -left.x() * cameraProps[0].x() - left.y() * cameraProps[0].y() - left.z() * cameraProps[0].z(),
                                    -up.x() * cameraProps[0].x() - up.y() * cameraProps[0].y() - up.z() * cameraProps[0].z(),
                                    -forward.x() * cameraProps[0].x() - forward.y() * cameraProps[0].y() - forward.z() * cameraProps[0].z(), 1);
    //

    if (!perspective) return;

    perspectiveMatrix = QMatrix4x4();
    //perspectiveMatrix.perspective(5, 1, .5, 1);
    //perspectiveMatrix.frustum(-10, 10, -10, 10, 20, 200);
    //perspectiveMatrix.frustum(-10, 10, -10, 10, 2 * focusDistance / cos (fieldAngle), 2 * fieldDistance / cos (fieldAngle));


}


QMatrix4x4 Camera::getPerspectiveMatrix() const
{
    return perspectiveMatrix;
}

QMatrix4x4 Camera::getViewingMatrix() const
{
    return viewingMatrix;
}

void Camera::move(const QVector3D addCoordinates)
{
    cameraProps = Transforms::moveVertexes(cameraProps, addCoordinates);
    calculateMatrixes(true, true);
}

void Camera::rotate(const float angle, const QVector3D os)
{
    cameraProps = Transforms::rotateVertexes(cameraProps, angle, os);
    calculateMatrixes(true, true);
}

void Camera::scale(const double scale)
{
    //cameraProps = Transforms::scaleVertexes(cameraProps, scale);
    //cameraProps = Transforms::scaleVertexesByCenter(cameraProps, scale, cameraProps[1]);
    QVector<QVector3D> camsGab = {cameraProps[2], cameraProps[3]};
    camsGab = Transforms::scaleVertexesByCenter(camsGab, scale, cameraProps[0]);

    cameraProps = {cameraProps[0], cameraProps[1], camsGab[0], camsGab[1]};

    calculateMatrixes(true, true);
}


float Dist2D (const QVector2D a, const QVector2D b){
    return std::sqrt(std::pow(a.x() - b.x(), 2) + std::pow(a.y() - b.y(), 2));
}

void Camera::transformByMouseMoving(const QVector2D currentMousePlace,
                                    const unsigned short actionType)
{

    // save a previous state
    if (prevMousePlace.x() == 0 && prevMousePlace.y() == 0){
        prevMousePlace = currentMousePlace;
        return;
    }

    if (prevMousePlace != currentMousePlace){
        // rotation
        if (actionType == 1){
                float sensetive = 50.0,
                      angleHorizont = (std::atan(std::abs(prevMousePlace.x() - currentMousePlace.x()) / sensetive)),
                      angleVertical = (std::atan(std::abs(prevMousePlace.y() - currentMousePlace.y()) / sensetive));
                rotate (-angleHorizont * (currentMousePlace.x() - prevMousePlace.x()),
                        QVector3D(0, 1, 0)/*Stereometry::Resid(cameraProps[2], cameraProps[0])*/);
                rotate (angleVertical * (currentMousePlace.y() - prevMousePlace.y()),
                        /*QVector3D(0, 0, 1)*/Stereometry::Resid(cameraProps[0], cameraProps[3]));
        }
        // scaling
        if (actionType == 2){
                float   sensetive = 100.0,
                        distance = Dist2D( currentMousePlace, prevMousePlace ) / sensetive;
                //(Dist2D( currentMousePlace, transformStartPoint ) - Dist2D( prevMousePlace, transformStartPoint ));
                float zoom = 1 - distance * ((currentMousePlace.y() > prevMousePlace.y() )? 1 : -1);
                scale(zoom);
        }
        // moving
        if (actionType == 0){
                float sensetive = 10.0;
                QVector3D add = (Stereometry::Summ(Stereometry::Mult(Stereometry::Resid(cameraProps[2], cameraProps[0]),
                                                        (currentMousePlace.y() - prevMousePlace.y())
                                                        * 1.0 / (sensetive * Stereometry::Dist(cameraProps[2], cameraProps[0]))),
                                       Stereometry::Mult(Stereometry::Resid(cameraProps[3], cameraProps[0]),
                                                        -(currentMousePlace.x() - prevMousePlace.x())
                                                        * 1.0 / (sensetive * Stereometry::Dist(cameraProps[3], cameraProps[0])))));
                //qDebug () << cameraProps[0].x() << cameraProps[0].y() << cameraProps[0].z();

                for (int i = 0; i < cameraProps.length(); i++)
                    cameraProps[i] = Stereometry::Summ (cameraProps[i], add);
                calculateMatrixes(true, true);
        }


        prevMousePlace = currentMousePlace;
    }
    return;
}

QVector<QVector3D> Camera::GetCamInfo() const
{
    return cameraProps;
}
