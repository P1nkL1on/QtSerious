#ifndef DERSTRUCT_DEFINES_H
#define DERSTRUCT_DEFINES_H

#include "Eigen/core"
#include <QVector>
#include <QDebug>
#include <QVector3D>
//using Eigen::Matrix;

namespace Df {

template <typename Scalar>
using Vector3 = Eigen::Matrix<Scalar,3,1>;

template <typename Scalar>
using Vector4 = Eigen::Matrix<Scalar,4,1>;

template <typename Scalar>
using Matrix4 = Eigen::Matrix<Scalar,4,4>;

template <typename Scalar>
Matrix4<Scalar> makeUnitMatrix (){
    Matrix4<Scalar> unitMatrix;
    for (int ind = 0; ind < 4; ind++)
        unitMatrix(ind, ind) = Scalar(0.0f);
    return unitMatrix;
}

template <typename Scalar>
Vector3<Scalar> makeVector3fromQVector (const QVector<double> &from){
    Vector3<Scalar> v3;
    if (!from.length())
        return v3;
    Q_ASSERT(from.length() == 3);
    for (int i = 0; i < 3; i++)
        v3(i,0) = Scalar(from[i]);
    return v3;
}

template <typename Scalar>
QVector3D makeQVectorFromVector3 (const Vector3<double> &from){
    return QVector3D(float(from(0,0)),
                     float(from(1,0)),
                     float(from(2,0)));
}


template <typename Scalar>
Vector3<Scalar> makeVector3fromDoubles (const double x,const double y,const double z){
    Vector3<Scalar> v3;
    v3(0,0) = Scalar(x);
    v3(1,0) = Scalar(y);
    v3(2,0) = Scalar(z);
    return v3;
}

template <typename Scalar>
Vector3<Scalar> makeUnitVector3 (){
    Vector3<Scalar> v3;
    v3(0,0) = Scalar(1);
    v3(1,0) = Scalar(1);
    v3(2,0) = Scalar(1);
    return v3;
}

template <typename Scalar>
Matrix4<Scalar> initialiseMatrix (const QVector<Scalar> &array){
    Matrix4<Scalar> result;
    result << array[0], array[1], array[2], array[3],
            array[4], array[5], array[6], array[7],
            array[8], array[9], array[10], array[11],
            array[12], array[13], array[14], array[15];
    return result;
}

template <typename Scalar>
inline void traceMatrix (const Matrix4<Scalar> &mat){
    qDebug() << "Matrix4x4 Derivable (type:Custom";
    for (int i = 0; i < 4; i++){
        QString lin = "";
        for (int j = 0; j < 4; j++)
            lin +=( (QString::number( mat(j,i)))).rightJustified(16, ' ') + " ";
        qDebug() << lin;
    }
    qDebug() << ")";
}

template <typename Scalar>
Vector3<Scalar> kostilBoneDrawer (const Matrix4<Scalar> &mat){
    Vector4<Scalar> v4(Scalar(0.0), Scalar(0.0), Scalar(0.0), Scalar(1.0));

    v4 = mat.transpose() * v4;

    traceMatrix(mat);
    qDebug() << v4(0,0)<< v4(1,0) << v4(2,0);
    return Vector3<Scalar>(v4(0,0), v4(1,0), v4(2,0));
}

template <typename Scalar>
inline Matrix4<Scalar> translationMatrix (const Vector3<Scalar> &translate){
    Matrix4<Scalar> M = Matrix4<Scalar>::Identity();
    M(3, 0) = translate(0,0);
    M(3, 1) = translate(1,0);
    M(3, 2) = translate(2,0);
    return M;
}

template <typename Scalar>
inline Matrix4<Scalar> rotationMatrixX (const Scalar &val){
    Scalar angle = val / 180.0 * M_PI;
    Matrix4<Scalar> M = Matrix4<Scalar>::Identity();
    M(1,1) = cos(angle); M(2,1) = Scalar(-1)* sin(angle);
    M(1,2) = sin(angle); M(2,2) = cos(angle);
    return M;
}
template <typename Scalar>
inline Matrix4<Scalar> rotationMatrixY (const Scalar &val){
    Scalar angle = val / 180.0 * M_PI;
    Matrix4<Scalar> M = Matrix4<Scalar>::Identity();
    M(0,0) = cos(angle); M(1,0) = Scalar(-1)* sin(angle);
    M(0,1) = sin(angle); M(1,1) = cos(angle);
    return M;
}
template <typename Scalar>
inline Matrix4<Scalar> rotationMatrixZ (const Scalar &val){
    Scalar angle = val / 180.0 * M_PI;
    Matrix4<Scalar> M = Matrix4<Scalar>::Identity();
    M(0,0) = cos(angle); M(2,0) = sin(angle);
    M(0,2) = Scalar(-1)* sin(angle); M(2,2) = cos(angle);
    return M;
}
template <typename Scalar>
inline Matrix4<Scalar> rotationMatrix (const Vector3<Scalar> &rotation){
    return rotationMatrixZ(rotation(2,0))
            * rotationMatrixY(rotation(1,0))
            * rotationMatrixX(rotation(0,0));
}

template <typename Scalar>
inline Matrix4<Scalar> scalingMatrix (const Vector3<Scalar> &scaling){
    Matrix4<Scalar> M = Matrix4<Scalar>::Identity();
    M(0, 0) = scaling(0,0);
    M(1, 1) = scaling(1,0);
    M(2, 2) = scaling(2,0);
    return M;
}

}
#endif // DERSTRUCT_DEFINES_H
