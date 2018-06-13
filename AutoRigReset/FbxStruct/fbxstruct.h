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

//template <typename Scalar>


}

#endif // FBXSTRUCT_H
