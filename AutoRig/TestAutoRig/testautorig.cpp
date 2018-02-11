#include "testautorig.h"
#include "Derivable/dermatops.h"
#include "Eigen/Dense"
#include "QTime"

#include "meshcomparer.h"
#include "gaussnewton.h"


using namespace DerivableVectorMatrixes;

bool TestAutoRig::Uber()
{
    qDebug() << "Uber << request to constructor";
    MeshComparer loss(bendingRig, targetMeshes[targMeshInd]->bindMesh);

    //  ASS TRANSLATE           JOINT ROTATES  cx3     JOINT SCALES sx1
    QVector<float> firstAngles = QVector<float>(3 + bendingRig->skeleton->joints.length() * 4);

    for (int c = 0; c < 3; c++)firstAngles[c] = bendingRig->skeleton->rootTransate(0,c).getValue();    // set an ass translations
    firstAngles[4] = .1;    // set X angle of ass to .1 to prevent clever system of nothing doing
    for (int c = 3 + bendingRig->skeleton->joints.length() * 3; c < firstAngles.length(); c++)
        firstAngles[c] = 1;

    qDebug() << "Uber << created angles";

    for (int i = 0; i < 2; i++)
        QVector<float> resAngles = OptimiseMethods::GaussNewtonMethod(loss, firstAngles, 1e-5, 20, gt, i == 0);
    qDebug() << "Uber << Qasi Newtone EXIT SUCCESS";
}

void TestAutoRig::ChangeTargetMeshInd(int count)
{
    targMeshInd += count;
    if (targMeshInd < 0) targMeshInd = targetMeshes.length() - 1;
    if (targMeshInd > targetMeshes.length() - 1) targMeshInd = 0;
    qDebug() << "Now applying to pose number " << targMeshInd;
}

TestAutoRig::TestAutoRig()
{
    targetMeshes = QVector<Rig*>();
    targetMeshes << NULL;
    bendingRig = NULL;
    targMeshInd = 0;
    gt = {};
}

TestAutoRig::TestAutoRig(Rig *rig, QVector<Rig *> mesh)
{
    targetMeshes = mesh;
    bendingRig = rig;
    targMeshInd = 0;
    gt = {};
}
void RotateSome (QVector<Matrix<Derivable,1,3>>& verts, const Matrix<Derivable,1,3> rot) {
    Matrix<Derivable,4,4> rotMa = MakeDeriveRotationMatrix(rot);
    for (int i = 0; i < verts.length(); i++){
        verts[i] = Summ3and4(Matrix<Derivable,1,3>(0,0,0), MakeVector4From3(verts[i], Derivable(1)) * rotMa);
    }
}
bool TestAutoRig::Modeling()
{
    // model bending to
    Mesh* ms = new Mesh();
    Derivable scale = /*Derivable(1.29);*/0.5 + qrand() % 100 / 100.0;
    Matrix<Derivable,1,3> trans = /*Matrix<Derivable,1,3>(2.7, 6.9, 9.5);*/Matrix<Derivable,1,3>(qrand() % 200 - 100,qrand() % 200 - 100,qrand() % 200 - 100);
    Matrix<Derivable,1,3> rotat = /*Matrix<Derivable,1,3>(102, 350, 162);*/Matrix<Derivable,1,3>(qrand() % 360,qrand() % 360,qrand() % 360);

    TraceVector(trans);
    TraceVector(rotat);
    qDebug() << "scale: " << scale.getValue();

    ms->vertexes
             << Matrix<Derivable,1,3>(0,0,0) * scale + trans
             << Matrix<Derivable,1,3>(10,80,-40) * scale + trans
             << Matrix<Derivable,1,3>(-10,80,-40) * scale + trans
             << Matrix<Derivable,1,3>(0,80,0) * scale + trans;
    RotateSome(ms->vertexes, rotat);
    ms->polygonIndexes << 0 << 1 << 2 << 1 << 2 << 3;
    ms->polygonStartIndexes << 0 << 3 << 6;
    Rig* r = new Rig(ms, NULL, NULL);
    r->modelColor = QColor(0, 255, 0, 100);
    r->conturColor = Qt::green;

    Mesh* ms2 = new Mesh();
    ms2->vertexes
             << Matrix<Derivable,1,3>(0,0,0)
             << Matrix<Derivable,1,3>(10,40,0)
             << Matrix<Derivable,1,3>(-10,40,0)
             << Matrix<Derivable,1,3>(0,80,0);
    ms2->polygonIndexes << 0 << 1 << 2 << 1 << 2 << 3;
    ms2->polygonStartIndexes << 0 << 3 << 6;



    QVector<Joint*> joints = {};
    joints << new Joint(Matrix<Derivable,1,3>(0, 00, 0),Matrix<Derivable,1,3>(0,0,0));
    joints << new Joint(Matrix<Derivable,1,3>(0, 30, 0),Matrix<Derivable,1,3>(0,0,0));
    joints << new Joint(Matrix<Derivable,1,3>(0, 30, 0),Matrix<Derivable,1,3>(0,0,0));
    joints << new Joint(Matrix<Derivable,1,3>(0, 30, 0),Matrix<Derivable,1,3>(0,0,0));
    joints[0]->kids << joints[1]; joints[1]->pater = joints[0];
    joints[1]->kids << joints[2]; joints[2]->pater = joints[1];
    joints[2]->kids << joints[3]; joints[3]->pater = joints[2];
    Skeleton* boner = new Skeleton(joints);
//    boner->localRotations = QVector<Matrix<Derivable,1,3>>(2);
//    boner->localScales = QVector<Derivable>(2);

    Skin* sk = new Skin();
    sk->addInfo(2, {2}, {1,1,1});
    sk->addInfo(0, {0}, {1,1,1});
    sk->addInfo(1, {1}, {1,1,1});
    sk->addInfo(3, {3}, {1,1,1});

    boner->CalculateGlobalCoordForEachJointMatrix();
    sk->GenerateAttends(ms2->vertexes, boner->getJointsGlobalTranslationsForSkin());


    Rig* rb = new Rig(ms2, boner, sk);
    r->cameraCenter = camCenter;
    rb->cameraCenter = camCenter;
    rb->modelColor = QColor(255, 0, 0, 100);
    rb->conturColor = Qt::red;

    targetMeshes.clear();
    targetMeshes << r;
    //targetMeshes[1] = rb;
    bendingRig = rb;
    qDebug() << targetMeshes;
    return true;
}



QString TestAutoRig::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective, const int width, const int hei)
{
    bendingRig->ApplyDrawToCanvas(painter, view, perspective, width, hei);
    targetMeshes[targMeshInd]->ApplyDrawToCanvas(painter, view, perspective, width, hei);

    painter->setPen(Qt::darkGray);
    painter->drawText(30, 30, 400, 400, 0, "LMK + Shift -- scale;\nLMK + Cntrl -- move;\nLMK + Alt -- rotate;\n\nMethods:\nO -- use GaussNewtone to shape;\nT -- test a bending instrument;\nR -- open on easy models with random scale, pos, rotation;\nE -- pull it thousand times;\n\n");

    for (int i = 0; i < gt.length(); i++)
        gt[i].DrawOn(painter);
    return QString();
}


int was = 0, zad = 0;
float TestAutoRig::TestBend()
{
//    bendingRig = new Rig();
//    bendingRig->skeleton = new Skeleton();
//    Joint* j1 = new Joint(Matrix<Derivable,1,3>(0,1,0),Matrix<Derivable,1,3>(0,0,0));
//    Joint* j2 = new Joint(Matrix<Derivable,1,3>(0,1,0),Matrix<Derivable,1,3>(0,0,0));
//    j1->kids << j2;
//    j2->pater = j1;

//    bendingRig->skeleton->joints << j1 << j2;

//    bendingRig->bindMesh = new Mesh();
//    bendingRig->skin = new Skin();
//    qDebug() << "Model created";
//    return 0.0;

    qDebug() << "Test bend called";
    //  ASS TRANSLATE           JOINT ROTATES       JOINT SCALES
    QVector<Matrix<Derivable,1,3>> newRotations = QVector<Matrix<Derivable,1,3>>(bendingRig->skeleton->joints.length());
    QVector<Derivable> newScales = QVector<Derivable>();
    for (int i = 0; i < newRotations.length(); i++)
        newScales << ((i % 5 != 0)? Derivable(1) : Derivable(1 + zad * .02));  // test an arm shit
    Matrix<Derivable,1,3> assTranslate;// = Matrix<Derivable,1,3>(0,0,++was * .5);

    was += 1;
    for (int i = 0; i < newRotations.length(); i++)
        newRotations[i] = Matrix<Derivable,1,3>(was * (i + 1) , 0, 0);
//    newRotations[8] = Matrix<Derivable,1,3>(0, 90,was * 5 + 90);
//    newRotations[31] = Matrix<Derivable,1,3>(was * 30,0,0);
//    newRotations[36] = Matrix<Derivable,1,3>(180 + was * 30,0,0);

    float res = bendingRig->CompareWithMeshOnRotates(assTranslate, newRotations, newScales, targetMeshes[targMeshInd]->bindMesh).getValue();
    qDebug() << "Test bend success";
    return res;
}




