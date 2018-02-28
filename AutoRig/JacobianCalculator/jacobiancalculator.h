#ifndef JACOBIANCALCULATOR_H
#define JACOBIANCALCULATOR_H

#include "Eigen/Core"
#include "QVector"
#include "Derivable/derivable.h"
#include "qdebug.h"

using namespace DerOperations;
using Eigen::Matrix;

namespace JacobianCalculator {
    inline Eigen::VectorXd toValue(const QVector<Derivable> &vec)
    {
        Eigen::VectorXd res(vec.size());
        for(int i = 0; i < vec.size(); ++i)
            res[i] = vec[i].getValue();
        return res;
    }

    template <typename Function>
    bool CalculateMixed(QVector<float> currentParams, Matrix<float, -1, -1>& resJacobian, Matrix<float,-1,-1>& residual, Function &loss){
        QVector<Derivable> derCurParams;
        int jointCount = (currentParams.length() - 3) / 4;
        //= currentParams;
        for (int i = 0; i < currentParams.length(); i++)
            derCurParams << Derivable(currentParams[i]);

        QVector<Derivable> jacobColumn = loss(derCurParams);

        const unsigned int angCount = currentParams.length(),
                           vertCount = jacobColumn.length();

        resJacobian = Eigen::MatrixXf(vertCount, angCount);
        Eigen::MatrixXd F = Eigen::MatrixXd(vertCount, 1);

        F = toValue(loss(derCurParams));

        double dx = 1e-6;
        for (int curParam = 0; curParam < derCurParams.length(); curParam ++)
            if (curParam >= jointCount * 3 + 3)
            {
                derCurParams[curParam] = derCurParams[curParam] + Derivable(dx);
                Eigen::VectorXd curF = toValue(loss(derCurParams));
                derCurParams[curParam] = derCurParams[curParam] - Derivable(dx);
                resJacobian.col(curParam) = ((curF - F) / dx).cast<float>();
            }
            else
            {
                derCurParams[curParam].setPrValue(1);
                jacobColumn = loss(derCurParams);
                for (int i = 0; i < jacobColumn.length(); i++)
                    resJacobian(i, curParam) = jacobColumn[i].getProiz();
                derCurParams[curParam].setPrValue(0);
            }
        F *= 0.5;
        residual = F.cast<float>();
    }

    template <typename Function>
    bool CalculateNumerical(QVector<float> currentParams, Matrix<float, -1, -1>& resJacobian, Matrix<float,-1,-1>& residual, Function &loss){
        QVector<Derivable> derCurParams;
        //= currentParams;
        for (int i = 0; i < currentParams.length(); i++)
            derCurParams << Derivable(currentParams[i]);

        QVector<Derivable> jacobColumn = loss(derCurParams);

        const unsigned int angCount = currentParams.length(),
                           vertCount = jacobColumn.length();

        resJacobian = Eigen::MatrixXf(vertCount, angCount);
        Eigen::MatrixXd F = Eigen::MatrixXd(vertCount, 1);

        F = toValue(loss(derCurParams));

        double dx = 1e-6;
        for (int curParam = 0; curParam < derCurParams.length(); curParam ++){
            derCurParams[curParam] = derCurParams[curParam] + Derivable(dx);
            Eigen::VectorXd curF = toValue(loss(derCurParams));
            derCurParams[curParam] = derCurParams[curParam] - Derivable(dx);
            resJacobian.col(curParam) = ((curF - F) / dx).cast<float>();
        }
        F *= 0.5;
        residual = F.cast<float>();
    }

    template <typename Function>
    bool CalculateForFunction (QVector<float> currentParams, Matrix<float, -1, -1>& resJacobian, Matrix<float,-1,-1>& F, Function &loss, int times){
        //return CalculateNumerical(currentParams, resJacobian, F, loss);
        QVector<Derivable> derCurParams; derCurParams.clear();
        //= currentParams;
        for (int i = 0; i < currentParams.length(); i++)
            derCurParams << Derivable(currentParams[i]);

        QVector<Derivable> jacobColumn = loss(derCurParams);

        const unsigned int angCount = currentParams.length(),
                           vertCount = jacobColumn.length();

        resJacobian = Matrix<float,-1,-1>(vertCount, angCount);
        F = Matrix<float,-1,-1>(vertCount, 1);
        //Eigen::MatrixXd F = Eigen::MatrixXd(vertCount, 1);
        jacobColumn = loss(derCurParams);

        for (int i = 0; i < jacobColumn.length(); i++)
            F(i,0) = .5 * jacobColumn[i].getValue();

        for (int curParam = 0; curParam < ((times > 0)? times : derCurParams.length()); curParam ++)
        {
            qDebug() << curParam << "/" << derCurParams.length();
            //if (curParam < ((derCurParams.length() - 3) / 4 * 3 + 3)){
                derCurParams[curParam].setPrValue(1);
                jacobColumn = loss(derCurParams);
                for (int i = 0; i < jacobColumn.length(); i++)
                    resJacobian(i, curParam) = jacobColumn[i].getProiz();
                derCurParams[curParam].setPrValue(0);
            //}
        }
//        int scales = (derCurParams.length() - 3) / 4;
//        Eigen::VectorXf lastCol = resJacobian.col(resJacobian.cols() - 1);
//        for (int i = scales - 2; i >= 0; i--){
//            int Ind = 3 + scales * 3 + i;
//            resJacobian.col(Ind) -= lastCol * 2;
//            lastCol += resJacobian.col(Ind);
//        }
        return 0;
    }

}

#endif // JACOBIANCALCULATOR_H
