#ifndef TESTAUTORIG_H
#define TESTAUTORIG_H

#include "StructFBX/rig.h"
#include "QVector"
#include "qpainter.h"

#include "Eigen/Core"
#include "Derivable/dermatops.h"
#include "Eigen/Dense"

using Eigen::Matrix;
using namespace  DerOperations;

class LossFunction {
public:
    LossFunction(Rig* rig, Mesh* mesh): currentRig(rig), targetMesh(mesh)
    {
        qDebug() << "Distance calculkator aka LossFunction contructed!";
        Q_ASSERT(rig->bindMesh->vertexes.length() == targetMesh->vertexes.length());
    }

    QVector<Derivable> operator()(const QVector<Derivable> params/*, float& distance*/) const//QVector<Matrix<Derivable,1,3>> newRotations) const
    {
        Q_ASSERT(params.length() % 3 == 0);
        QVector<Matrix<Derivable,1,3>> newRotations = QVector<Matrix<Derivable,1,3>>();
        for (int i = 0 ; i < params.length() / 3; i++)
            newRotations << Matrix<Derivable,1,3>(params[i * 3], params[i * 3 + 1], params[i * 3 + 2]);
        currentRig->skeleton->SetRotations(newRotations);
        currentRig->BendSkinToSkeleton();
        return currentRig->bendedMesh->CompareWithAnotherMeshCoords(targetMesh);
    }
    float operator()(const QVector<float> params) const
    {
        QVector<Matrix<Derivable,1,3>> newRotations = QVector<Matrix<Derivable,1,3>>();
        for (int i = 0 ; i < params.length() / 3; i++)
            newRotations << Matrix<Derivable,1,3>(params[i * 3], params[i * 3 + 1], params[i * 3 + 2]);
        currentRig->skeleton->SetRotations(newRotations);
        currentRig->BendSkinToSkeleton();
        return currentRig->bendedMesh->CompareWithAnotherMesh(targetMesh).getValue();
    }

private:
    Rig* currentRig;
    Mesh* targetMesh;
};

template <typename Function>
bool CalculateJacobian (QVector<float> currentParams, Matrix<float, -1, -1>& resJacobian, Matrix<float,-1,-1>& F, Function &loss){
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
    qDebug() << "Jacobian << calculated";
}

template <typename Function>
QVector<float> QasiNewtone (Function& func, const QVector<float> params,
                                 const float epsilon, const int maxIterationCount){
    QVector<float> res = params;
    Matrix<float,-1,-1>
            jacobMatrix, F;
    CalculateJacobian(res, jacobMatrix, F, func );
    Matrix<float,-1,-1>
            jacobTrans = Matrix<float, -1, -1>(jacobMatrix.rows(), jacobMatrix.cols()),
            step = Matrix<float,-1,-1>( jacobMatrix.cols(), 1);

    int iterationNumber = 0;
    float currentDistance = func(res); // set proto distance here

    do{
        qDebug() << "Iteration " << ++iterationNumber;

        CalculateJacobian(res, jacobMatrix, F, func );
        jacobTrans = jacobMatrix.transpose();
        step = (jacobTrans * jacobMatrix).colPivHouseholderQr().solve(-jacobTrans * F);

        for (int i = 0; i < jacobMatrix.cols() / 3; i++)
            res[i] = res[i] + step(i, 0);//+ SetDerive3DVector(.5 * QVector3D(step(i * 3, 0),step(i * 3 + 1, 0),step(i * 3 + 2, 0)));

        currentDistance = func(res);
        qDebug() << "Current distance is now " << currentDistance;
        if (iterationNumber > maxIterationCount){ qDebug() << "@ Finish by too mych iteration count!";break;}

    } while (currentDistance > epsilon);
    qDebug() << "@ Done!";
    return res;
}

class TestAutoRig : public GraphicsObjectStruct::GraphicsObject
{
private:
    QVector<Rig*> targetMeshes;
    Rig* bendingRig;
    QVector<Matrix<Derivable,1,3>> nowRotations;
    Matrix<Derivable,1,3> nowRootPose;
    QVector<QVector3D> angleAdds;
    float step, prevdist;
    int targMeshInd;
public:
    void ChangeTargetMeshInd (int count);
    void ResetTransofrms ();

    // drawing to canvas
    QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                           const int width, const int hei) override;


    TestAutoRig();
    TestAutoRig(Rig* rig, QVector<Rig*> mesh);

    float ApplyRotations();
    float JacobianStep ();
    bool Uber ();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


#endif // TESTAUTORIG_H
