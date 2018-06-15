#ifndef DERSTRUCT_DEFINES_H
#define DERSTRUCT_DEFINES_H

#include "Eigen/core"
#include "QVector"

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
Vector3<Scalar> makeVector3fromDoubles (const double x,const double y,const double z){
    Vector3<Scalar> v3;
    v3(0,0) = Scalar(x);
    v3(1,0) = Scalar(y);
    v3(2,0) = Scalar(z);
    return v3;
}

template <typename Scalar>
Vector3<Scalar> makeUnitVector3 (){
    return Vector3<Scalar>(Scalar(1),Scalar(1),Scalar(1));
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
Vector3<Scalar> kostilBoneDrawer (const Matrix4<Scalar> &mat){
    Vector4<Scalar> v4(Scalar(0.0), Scalar(0.0), Scalar(0.0), Scalar(1.0));

    v4 = mat * v4;

    return Vector3<Scalar>(v4(0,0), v4(1,0), v4(2,0));
}

}
#endif // DERSTRUCT_DEFINES_H
