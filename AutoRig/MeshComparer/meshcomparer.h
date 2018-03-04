#ifndef MESHCOMPARER_H
#define MESHCOMPARER_H

#include "rig.h"
#include "Derivable/derivable.h"
#include "Eigen/Core"

using Eigen::Matrix;
using namespace DerOperations;
#include "qmainwindow.h"

class CallBackDrawing
{
private:
    QMainWindow* wind;
public:
    CallBackDrawing(QMainWindow* window){
        wind = window;
    }
    void operator ()(){
        //qDebug() << "Repaint called";
        wind->repaint();
    }
};

class MeshComparer
{
private:
    Rig* currentRig;
    Mesh* targetMesh;


public:
    bool DistributeToParams (const QVector<Derivable> params,
                             Matrix<Derivable,1,3>& root ,
                             QVector<Matrix<Derivable,1,3>>& newRotations,
                             QVector<Matrix<Derivable,1,3>>& newScales) const
    {
        newRotations.clear();
        newScales.clear();
        root = Matrix<Derivable,1,3> (params[0], params[1], params[2]);
        for (int i = 1 ; i < 1 + currentRig->skeleton->joints.length(); i++)
            newRotations << Matrix<Derivable,1,3>(params[i * 3], params[i * 3 + 1], params[i * 3 + 2]);
        for (int i = 1 + currentRig->skeleton->joints.length(); i * 3 < params.length(); i++)
            newScales << Matrix<Derivable,1,3>(params[i * 3], params[i * 3 + 1], params[i * 3 + 2]);
        //qDebug() << "rot count " << newRotations.length() << "    scal count" << newScales.length();
        return true;
    }
    MeshComparer(Rig* rig, Mesh* mesh): currentRig(rig), targetMesh(mesh)
    {
        qDebug() << "Distance calculkator aka LossFunction contructed!";
        Q_ASSERT(targetMesh == NULL || rig->bindMesh->vertexes.length() == targetMesh->vertexes.length());
    }

    QVector<Derivable> operator()(const QVector<Derivable> params) const
    {
        QVector<Matrix<Derivable,1,3>> newRotations;
        QVector<Matrix<Derivable,1,3>> newScales;
        Matrix<Derivable,1,3> rootTrans;

        DistributeToParams(params, rootTrans, newRotations, newScales);
        return currentRig->CompareWithMeshOnRotatesCoord(rootTrans, newRotations, newScales, targetMesh);
    }
    float operator()(const QVector<float> params) const
    {
        QVector<Derivable> derParams; derParams.clear();
        for (int i = 0; i < params.length(); i++)
            derParams << Derivable(params[i]);
        QVector<Matrix<Derivable,1,3>> newRotations;
        QVector<Matrix<Derivable,1,3>> newScales;
        Matrix<Derivable,1,3> rootTrans;

        DistributeToParams(derParams, rootTrans, newRotations, newScales);
        return currentRig->CompareWithMeshOnRotates(rootTrans, newRotations, newScales, targetMesh).getValue();
    }

};

#endif // MESHCOMPARER_H
