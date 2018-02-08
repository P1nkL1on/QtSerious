#include "testautorig.h"
#include "Derivable/dermatops.h"
#include "Eigen/Dense"
#include "QTime"

using namespace DerivableVectorMatrixes;

void TestAutoRig::ChangeTargetMeshInd(int count)
{
    targMeshInd += count;
    if (targMeshInd < 0) targMeshInd = targetMeshes.length() - 1;
    if (targMeshInd > targetMeshes.length() - 1) targMeshInd = 0;
    qDebug() << "Now applying to pose number " << targMeshInd;
}

void TestAutoRig::ResetTransofrms()
{
    for (int curJoint = 0; curJoint < nowRotations.length(); curJoint++)
        for (int coord = 0; coord < 3; coord++)
            nowRotations[curJoint](0,coord) = Derivable(0,0);
    Derivable res = bendingRig->CompareWithMeshOnRotates(nowRotations, targetMeshes[targMeshInd]->bindMesh);
    qDebug() << "Model rotations reset";
}


TestAutoRig::TestAutoRig()
{
    targetMeshes = QVector<Rig*>();
    targetMeshes << NULL;
    bendingRig = NULL;
    angleAdds = QVector<QVector3D>();
    step = 2; prevdist = -1;
    targMeshInd = 0;
}

TestAutoRig::TestAutoRig(Rig *rig, QVector<Rig *> mesh)
{
    targetMeshes = mesh;
    bendingRig = rig;
    nowRotations = rig->skeleton->getJointsLocalRotations();
    nowRootPose = Matrix<Derivable,1,3>();
    angleAdds = QVector<QVector3D>();
    step = 2; prevdist = -1;
    targMeshInd = 0;
}


QString TestAutoRig::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective, const int width, const int hei)
{
    bendingRig->ApplyDrawToCanvas(painter, view, perspective, width, hei);
    targetMeshes[targMeshInd]->ApplyDrawToCanvas(painter, view, perspective, width, hei);
    return QString();
}


float TestAutoRig::ApplyRotations()
{
    float dist = -1;
    QVector<QVector3D> addAngles = QVector<QVector3D>();
    for (int curJoint = 0; curJoint < nowRotations.length(); curJoint++){
        QVector3D addAngle = QVector3D(0,0,0);

        for (int coord = 0; coord < 3; coord++){
            if (coord > 0)nowRotations[curJoint](0,coord - 1).setPrValue(0); else {if (curJoint > 0)nowRotations[curJoint - 1](0,2).setPrValue(0);}
            nowRotations[curJoint](0,coord).setPrValue(1);
            Derivable res = bendingRig->CompareWithMeshOnRotates(nowRotations, targetMeshes[0]->bindMesh);
            if (dist < 0) dist = res.getValue();
            addAngle[coord] = res.getProiz();
        }
        addAngles << addAngle;
    }
    nowRotations[nowRotations.length() - 1](0,2).setPrValue(0);//


    // changing a step
    if (prevdist > 0){
        if (dist < prevdist) step *= 1.05;
        if (dist > prevdist) step = (step > 2)? 2 : step * .8;
    }
    prevdist = dist;
    float approximateDist = dist + 1;
    do{
        QVector<Matrix<Derivable,1,3>> willBeRotations = QVector<Matrix<Derivable,1,3>>();
        for (int curJoint = 0; curJoint < nowRotations.length(); curJoint++)
            willBeRotations << nowRotations[curJoint] - SetDerive3DVector(addAngles[curJoint] * step);
        approximateDist = bendingRig->CompareWithMeshOnRotates(willBeRotations, targetMeshes[0]->bindMesh).getValue();
        if (approximateDist > dist){ step *= .8; qDebug() << "Approximate citical dist increase! (" << approximateDist << "). Descrease step to " << step;}
    }while(approximateDist > dist);

    float maxGrad = 0; QString lessThen = "", aboveThen = "";
    for (int curJoint = 0; curJoint < addAngles.length(); curJoint++){
        float thisGrad = addAngles[curJoint].length();
        if (thisGrad > maxGrad) maxGrad = thisGrad;
    }
    for (int curJoint = 0; curJoint < addAngles.length(); curJoint++)
        aboveThen += (addAngles[curJoint].length() > maxGrad * .8)? QString::number(curJoint)+", " : "";

    //qDebug() << aboveThen << " are above middle value, graduated with step " << step * 10;
    qDebug() << ">>>>>>Distance : " << dist << ",   step :" << step << "  above half: " << aboveThen;
    for (int curJoint = 0; curJoint < nowRotations.length(); curJoint++){
        float personalStep = step * (1 - .5 * (addAngles[curJoint].length() < maxGrad * .4) + 9 * (addAngles[curJoint].length() > maxGrad * .8));
        nowRotations[curJoint] = nowRotations[curJoint] - SetDerive3DVector(addAngles[curJoint] * step);
    }

    return dist;
}

void _timecheck (const QString what, QTime& timer){
    qDebug() << what.leftJustified(25, ' ') << timer.elapsed() << " ms";
    timer.restart();
}

float TestAutoRig::JacobianStep()
{
    QTime t;
    t.start();
    const unsigned int angCount = nowRotations.length() * 3;
    const unsigned int vertCount = bendingRig->bindMesh->vertexes.length() * 3;
    Matrix<float, -1, -1>
            jacobMatrix = Matrix<float, -1, -1>(vertCount, angCount),
            jacobTrans = Matrix<float, -1, -1>(vertCount, angCount),
            F = Matrix<float,-1,-1>(vertCount, 1),
            step = Matrix<float,-1,-1>(1, angCount);

    _timecheck("Memory allocated", t);
    QVector<Derivable> jacobColomn = QVector<Derivable>();
    for (int curJoint = 0; curJoint < nowRotations.length(); curJoint++){
        for (int coord = 0; coord < 3; coord++){     
            nowRotations[curJoint](0,coord).setPrValue(1);
            jacobColomn = bendingRig->CompareWithMeshOnRotatesCoord(nowRotations, targetMeshes[targMeshInd]->bindMesh);
            for (int i = 0; i < jacobColomn.length(); i++){
                jacobMatrix(i, curJoint * 3 + coord) = jacobColomn[i].getProiz();
                F(i,0) = .5 * jacobColomn[i].getValue();
            }
            nowRotations[curJoint](0,coord).setPrValue(0);
        }
    }
    qDebug() << "joints->Jacobian " << nowRotations.length() << " x ~" << (t.elapsed() * 1.0 / nowRotations.length()) << " ms"; t.restart();
    jacobTrans = jacobMatrix.transpose();
    _timecheck("J transposed", t);
    step = (jacobTrans * jacobMatrix).colPivHouseholderQr().solve(-jacobTrans * F);
    _timecheck("System solved", t);
    for (int i = 0; i < angCount / 3; i++){
        nowRotations[i] = nowRotations[i] + SetDerive3DVector(QVector3D(step(i * 3, 0),step(i * 3 + 1, 0),step(i * 3 + 2, 0)));
    }
    Derivable res = bendingRig->CompareWithMeshOnRotates(nowRotations, targetMeshes[targMeshInd]->bindMesh);
    _timecheck("Total", t);
    return res.getValue();
}

bool TestAutoRig::Uber()
{
    qDebug() << "Uber << request to constructor";
    LossFunction loss(bendingRig, targetMeshes[targMeshInd]->bindMesh);

    QVector<float> firstAngles = QVector<float>(bendingRig->skeleton->joints.length() * 3);
    firstAngles[0] = .1;
    qDebug() << "Uber << created angles";

    QVector<float> resAngles = QasiNewtone(loss, firstAngles, 1, 20);
    qDebug() << "Uber << Qasi Newtone EXIT";
}
