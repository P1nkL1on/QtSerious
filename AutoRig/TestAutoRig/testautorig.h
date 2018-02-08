#ifndef TESTAUTORIG_H
#define TESTAUTORIG_H

#include "StructFBX/rig.h"
#include "QVector"
#include "qpainter.h"

#include "Eigen/Core"
#include "Derivable/dermatops.h"
#include "Eigen/Dense"
#include "QTime"
#include "mainwindow.h"

using Eigen::Matrix;
using namespace  DerOperations;

class TestAutoRig;

//class CallBackDrawer{
//    public:
//        TestAutoRig* from;
//        QPainter* qp;
//        QMatrix4x4 vm, pm;
//        int wid, he;
//        CallBackDrawer();
//        CallBackDrawer(TestAutoRig*  tar);
//        operator ()(QVector<float> params);
//};

class TestAutoRig : public GraphicsObjectStruct::GraphicsObject
{
private:
    QVector<Rig*> targetMeshes;
    Rig* bendingRig;
    //CallBackDrawer callback;
//    QVector<Matrix<Derivable,1,3>> nowRotations;
//    Matrix<Derivable,1,3> nowRootPose;
//    QVector<QVector3D> angleAdds;
//    float step, prevdist;
    int targMeshInd;

public:
    void ChangeTargetMeshInd (int count);
//    void ResetTransofrms ();

    // drawing to canvas
    QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                           const int width, const int hei) override;

    TestAutoRig();
    TestAutoRig(Rig* rig, QVector<Rig*> mesh);

//    float ApplyRotations();
//    float JacobianStep ();
    bool Uber ();
    float testBend ();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};




#endif // TESTAUTORIG_H
