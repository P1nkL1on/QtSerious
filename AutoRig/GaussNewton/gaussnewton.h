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
            JacobianCalculator::CalculateForFunction(res, jacobMatrix, F, func );
            jacobTrans = jacobMatrix.transpose();
            step = (jacobTrans * jacobMatrix).colPivHouseholderQr().solve(-jacobTrans * F);

            float stepMult = 2, prevDistance = currentDistance;
            int trys = 0;
            QVector<float> appRes;
            do{
                appRes = res;
                for (int i = 0; i < jacobMatrix.cols(); i++){
                    if (i < (jacobMatrix.cols() - 3) / 4 * 3 + 3)   // do not apply scale
                        appRes[i] = res[i] + step(i, 0) * stepMult;
                }
                currentDistance = func(appRes);
                if (currentDistance > prevDistance){
                    stepMult -= .1;qDebug() << "Step is now " << stepMult << "   casue fail in distance of " << currentDistance << " > " << prevDistance;
                }
            }while ((currentDistance > prevDistance && trys < 10));
            res = appRes;   // apply a success step

            //qDebug() << ">> Callback call !"; callback (res);
            qDebug() << "Iteration " << ++iterationNumber << "Current distance is now " << currentDistance << "      Iteration time is: " << t.elapsed() << " ms"; t.restart();
            if (iterationNumber > maxIterationCount){ qDebug() << "@ Finish by too much iteration count!"; break; }
            //if (stepLength < 1){ qDebug() << "@ Finish cause steps become too liitle!"; break; }

        } while (currentDistance > epsilon);
        qDebug() << "@ Done in " << ttotal.elapsed() << " ms";
        return res;
    }
}

//                if (i >= (jacobMatrix.cols() - 3) / 4 * 3 + 6 || i < (jacobMatrix.cols() - 3) / 4 * 3 + 3)
//                    //qDebug() << "scale " << res[i] << " - " << step(i,0);
//                    res[i] = res[i] + step(i, 0) * .0;
//                else


#endif // GAUSSNEWTON_H
