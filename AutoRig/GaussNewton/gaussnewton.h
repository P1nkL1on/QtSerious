#ifndef GAUSSNEWTON_H
#define GAUSSNEWTON_H

#include "Eigen/Core"
#include "Eigen/Dense"
#include "QTime"
#include "QDebug"

#include "jacobiancalculator.h"

using Eigen::Matrix;

namespace OptimiseMethods {
    template <typename Function>//, typename CallBack>
    QVector<float> GaussNewtonMethod (Function& func, /*CallBack& callback,*/ const QVector<float> params,
                                     const float epsilon, const int maxIterationCount){
        QTime t , ttotal;
        t.start(); ttotal.start();
        QVector<float> res = params;
        Matrix<float,-1,-1>
                jacobMatrix, F;
        JacobianCalculator::CalculateForFunction(res, jacobMatrix, F, func );
        Matrix<float,-1,-1>
                jacobTrans = Matrix<float, -1, -1>(jacobMatrix.rows(), jacobMatrix.cols()),
                step = Matrix<float,-1,-1>( jacobMatrix.cols(), 1);

        int iterationNumber = 0;
        float currentDistance = func(res); // set proto distance here

        do{
            qDebug() << "Iteration " << ++iterationNumber;

            JacobianCalculator::CalculateForFunction(res, jacobMatrix, F, func );
            qDebug() << "Jacobian culculated in: " << t.elapsed() << " ms"; t.restart();
            jacobTrans = jacobMatrix.transpose();
            step = (jacobTrans * jacobMatrix).colPivHouseholderQr().solve(-jacobTrans * F);

            float stepLength = 0;
            for (int i = 0; i < jacobMatrix.cols(); i++){
                if (i < 3){ qDebug() << "ass " << res[i] << " - " << step(i,0);}

                res[i] = res[i] + step(i, 0);
                stepLength += step(i,0) * step(i,0);
            }

            currentDistance = func(res);
            //qDebug() << ">> Callback call !"; callback (res);
            qDebug() << "Current distance is now " << currentDistance << "      Iteration time is: " << t.elapsed() << " ms"; t.restart();
            if (iterationNumber > maxIterationCount){ qDebug() << "@ Finish by too much iteration count!"; break; }
            //if (stepLength < 1){ qDebug() << "@ Finish cause steps become too liitle!"; break; }

        } while (currentDistance > epsilon);
        qDebug() << "@ Done in " << ttotal.elapsed() << " ms";
        return res;
    }
}



#endif // GAUSSNEWTON_H
