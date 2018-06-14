#ifndef FBXSTRUCT_H
#define FBXSTRUCT_H
#include "Eigen/core"
#include "QVector"
#include "DerStruct/derivable.h"

using Eigen::Matrix;

namespace FbxStruct
{

template <typename Scalar>
using Vector3 = Matrix<Scalar,3,1>;

template <typename Scalar>
using Vector4 = Matrix<Scalar,3,1>;

template <typename Scalar>
using Matrix4 = Matrix<Scalar,4,4>;

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
    Q_ASSERT(from.length() == 3);
    for (int i = 0; i < 3; i++)
        v3(i,0) = Scalar(from[i]);
    return v3;
}

}

#endif // FBXSTRUCT_H
