#ifndef MESHCOMPARER_H
#define MESHCOMPARER_H

#include "rig.h"
#include "Derivable/derivable.h"
#include "Eigen/Core"

using Eigen::Matrix;
using namespace DerOperations;

class MeshComparer
{
public:
    MeshComparer(Rig* rig, Mesh* mesh): currentRig(rig), targetMesh(mesh)
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

#endif // MESHCOMPARER_H
