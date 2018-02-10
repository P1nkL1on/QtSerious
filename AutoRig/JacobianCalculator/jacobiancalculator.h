#ifndef JACOBIANCALCULATOR_H
#define JACOBIANCALCULATOR_H

#include "Eigen/Core"
#include "QVector"
#include "Derivable/derivable.h"
#include "qdebug.h"

using namespace DerOperations;
using Eigen::Matrix;

namespace JacobianCalculator {

    template <typename Function>
    bool CalculateForFunction (QVector<float> currentParams, Matrix<float, -1, -1>& resJacobian, Matrix<float,-1,-1>& F, Function &loss){
        QVector<Derivable> derCurParams;
        //= currentParams;
        for (int i = 0; i < currentParams.length(); i++)
            derCurParams << Derivable(currentParams[i]);

        QVector<Derivable> jacobColumn = loss(derCurParams);

        const unsigned int angCount = currentParams.length(),
                           vertCount = jacobColumn.length();

        resJacobian = Matrix<float,-1,-1>(vertCount, angCount);
        F = Matrix<float,-1,-1>(vertCount, 1);
        for (int curParam = 0; curParam < derCurParams.length(); curParam ++)
        {
            derCurParams[curParam].setPrValue(1);
            jacobColumn = loss(derCurParams);
            for (int i = 0; i < jacobColumn.length(); i++){
                resJacobian(i, curParam) = jacobColumn[i].getProiz();
                F(i,0) = .5 * jacobColumn[i].getValue();
            }
            derCurParams[curParam].setPrValue(0);
        }
    }

}

#endif // JACOBIANCALCULATOR_H
