#ifndef FBXSTRUCT_H
#define FBXSTRUCT_H
#include "Eigen/core"

using Eigen::Matrix;

namespace FbxStruct
{

template <typename Scalar>
using Vector3 = Matrix<Scalar,3,1>;

template <typename Scalar>
using Vector4 = Matrix<Scalar,3,1>;



}

#endif // FBXSTRUCT_H
