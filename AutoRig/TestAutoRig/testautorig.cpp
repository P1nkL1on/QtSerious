#include "testautorig.h"
#include "Derivable/dermatops.h"
#include "Eigen/Dense"
#include "QTime"

#include "meshcomparer.h"
#include "gaussnewton.h"

#include "qapplication.h"

using namespace DerivableVectorMatrixes;

QVector<float> TestAutoRig::BendSkeletonIntoMesh(bool isGaussNewton)
{
    qDebug() << "Bending call;";
    MeshComparer loss(bendingRig, targetMeshes[targMeshInd]->bindMesh);
    CallBackDrawing callback (window);
    //qDebug() << "Loss and callback functions created;";

    QVector<float> parameters = QVector<float>(3 + bendingRig->skeleton->joints.length() * 6);

    for (int c = 0; c < 3; c++)
        parameters[c] = bendingRig->skeleton->rootTransate(0,c).getValue();    // set an ass translations
    parameters[4] = 1e-5;
    for (int c = 3 + bendingRig->skeleton->joints.length() * 3; c < parameters.length(); c++)
        parameters[c] = 1;

    QVector<float> resParams = OptimiseMethods::GaussNewtonMethod(loss, callback, parameters, eps, maxSteps, !isGaussNewton);
    qDebug() << "Bend success;";
    return resParams;
}

bool TestAutoRig::RewrapSkeletonToMesh( QVector<float> params )
{
    MeshComparer mc = MeshComparer(bendingRig, NULL);
    Matrix<Derivable,1,3> root;
    QVector<Matrix<Derivable,1,3>> rotat;
    QVector<Matrix<Derivable,1,3>> scals;
    static QVector<Derivable> withParams = QVector<Derivable>();
    withParams.clear();
    for (int j =0; j < params.length(); j++)
        withParams << Derivable(params[j]);

    //qDebug() << params;
    bendingRig->skeleton->CalculateGlobalCoordForEachJointMatrix();
    //qDebug() << bendingRig->skin;

    bendingRig->bindMesh = targetMeshes[targMeshInd]->bindMesh;
    bendingRig->skin->GenerateAttends(bendingRig->bindMesh->vertexes, bendingRig->skeleton->SetBonesScaleAsBoneLength());

    qDebug() << "Attends regenerated";

//    // set new scales to normal 1
    for (int jC = (withParams.length() - 3 ) / 6, i = 3 + jC * 3; i < 3 + jC * 6; i++)
       withParams[i] = Derivable(1);//withParams[i];

    //return false;



    for (int i = 0; i < 100; i++){
       for (int j = 3; j < ((withParams.length() - 3) / 2) + 3; j++)
            withParams[j] = withParams[j] / Derivable(1.05);
        mc.DistributeToParams(withParams, root, rotat, scals);
        bendingRig->ApplyBending(root, rotat, scals);
        //qDebug() << "Maaag!";
        window->repaint();
    }
    qDebug() << "Bend finish;";
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
    eps = 1e-4; maxSteps = 5;
}

TestAutoRig::TestAutoRig(Rig *rig, QVector<Rig *> mesh)
{
    targetMeshes = mesh;
    bendingRig = rig;
    targMeshInd = 0;
    eps = 1e-4; maxSteps = 5;
}
void RotateSome (QVector<Matrix<Derivable,1,3>>& verts, const Matrix<Derivable,1,3> rot) {
    Matrix<Derivable,4,4> rotMa = MakeDeriveRotationMatrix(rot);
    for (int i = 0; i < verts.length(); i++){
        verts[i] = Summ3and4(Matrix<Derivable,1,3>(0,0,0), MakeVector4From3(verts[i], Derivable(1)) * rotMa);
    }
}
bool TestAutoRig::SetCustomLowModel()
{
    qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));
    // model bending to
    Mesh* ms = new Mesh();
    Derivable scale = 0.5 + qrand() % 100 / 100.0;
    Matrix<Derivable,1,3> trans = Matrix<Derivable,1,3>(qrand() % 200 - 100,qrand() % 200 - 100,qrand() % 200 - 100);
    Matrix<Derivable,1,3> rotat = Matrix<Derivable,1,3>(qrand() % 360,qrand() % 360,qrand() % 360);

    TraceVector(trans);
    TraceVector(rotat);
    //qDebug() << "scale: " << scale.getValue();

    ms->vertexes
             << Matrix<Derivable,1,3>(0,0,0) * scale + trans
             << Matrix<Derivable,1,3>(10,40,0) * scale + trans
             << Matrix<Derivable,1,3>(-10,40,0) * scale + trans
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
    joints << new Joint(Matrix<Derivable,1,3>(0, 40, 0),Matrix<Derivable,1,3>(0,0,0));
    //joints << new Joint(Matrix<Derivable,1,3>(0, 30, 0),Matrix<Derivable,1,3>(0,0,0));
    //joints << new Joint(Matrix<Derivable,1,3>(0, 30, 0),Matrix<Derivable,1,3>(0,0,0));
    joints[0]->kids << joints[1]; joints[1]->pater = joints[0];
    //joints[1]->kids << joints[2]; joints[2]->pater = joints[1];
    //joints[2]->kids << joints[3]; joints[3]->pater = joints[2];
    Skeleton* boner = new Skeleton(joints);
//    boner->localRotations = QVector<Matrix<Derivable,1,3>>(2);
//    boner->localScales = QVector<Derivable>(2);

    Skin* sk = new Skin();
    sk->addInfo(1, {1,3,2}, {1,1,1});
    sk->addInfo(0, {0}, {1,1,1});
//    sk->addInfo(1, {1}, {1,1,1});
//    sk->addInfo(3, {3}, {1,1,1});

    boner->CalculateGlobalCoordForEachJointMatrix();
    sk->GenerateAttends(ms2->vertexes, boner->getJointsGlobalTranslationsForSkin());


    Rig* rb = new Rig(ms2, boner, sk);
    r->cameraCenter = camCenter;
    rb->cameraCenter = camCenter;
    rb->modelColor = QColor(255, 0, 0, 100);
    rb->conturColor = Qt::red;

    targetMeshes.clear();
    targetMeshes << r;
    targMeshInd = 0;
    //targetMeshes[1] = rb;
    bendingRig = rb;
    //qDebug() << targetMeshes;
    eps = 1e-6; maxSteps = 300;
    return true;
}

bool TestAutoRig::SetCustomHighModel(float maxAngle)
{
    qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));
    QVector<Matrix<Derivable,1,3>> newRotations = QVector<Matrix<Derivable,1,3>>();
    QVector<Matrix<Derivable,1,3>> newScales = QVector<Matrix<Derivable,1,3>>();

    Matrix<Derivable,1,3> assTranslate = bendingRig->skeleton->rootTransate;//Matrix<Derivable,1,3>(qrand() % 200 - 100,qrand() % 200 - 100,qrand() % 200 - 100);
//    for (int i = 0; i < bendingRig->skeleton->joints.length(); i++){
//        newScales << Matrix<Derivable,1,3>(1,1,1);
//        newRotations <<  Matrix<Derivable,1,3>(0,0,0);
//    }
//    newScales[4] = Matrix<Derivable,1,3>(1.5,1.5,1.5);
    for (int i = 0; i < bendingRig->skeleton->joints.length(); i++){

        //float newScale = qrand() % 100 / 100.0 + .5;
        newScales << Matrix<Derivable,1,3>(qrand() % 100 / 100.0 + .5,qrand() % 100 / 100.0 + .5,qrand() % 100 / 100.0 + .5);

        newRotations <<
                     Matrix<Derivable,1,3>(0,0,0);
                     //  Matrix<Derivable,1,3>((newScales[i](0,0) - .5) * 30 - 15,(newScales[i](0,1) - .5) * 30 - 15,(newScales[i](0,2) - .5) * 30 - 15);
        //newScales[newScales.length() - 1] = Matrix<Derivable,1,3>(1 + i / 20.0,1 + i / 60.0,1 + i / 200.0);//;
        //qDebug() << "";
        //qDebug() << "Joint #"<<i;
        //TraceVector(newRotations[i]);
        //TraceVector(newScales[i]);
    }


    bendingRig->ApplyBending(assTranslate, newRotations, newScales); //CompareWithMeshOnRotates(assTranslate, newRotations, newScales, targetMeshes[targMeshInd]->bindMesh).getValue();
//    for (int i = 0; i < bendingRig->skeleton->joints.length(); i++)
//        if (bendingRig->skeleton->joints[i]->pater == NULL){
//            qDebug() << "Ass : " ;
//            TraceVector(bendingRig->skeleton->rootTransate);
//        }
//    else
//        {
//            qDebug() << "Joint " << i;
//            Matrix<Derivable,1,3> t0 = bendingRig->skeleton->joints[i]->currentTranslation - bendingRig->skeleton->joints[i]->pater->currentTranslation;
//            TraceVector(t0);
//        }

    Skeleton* sk = new Skeleton(*(bendingRig->skeleton));


    Mesh* createdMesh = new Mesh();
    createdMesh->vertexes = bendingRig->bendedMesh->vertexes;
    createdMesh->polygonIndexes = bendingRig->bendedMesh->polygonIndexes;
    createdMesh->polygonStartIndexes = bendingRig->bendedMesh->polygonStartIndexes;

    targetMeshes.clear();

    Rig* newMesh = new Rig(createdMesh, sk, NULL);
    newMesh->modelColor = QColor(0, 200, 0, 20);
    newMesh->conturColor = QColor(0,0,0,0);

    targetMeshes << newMesh;
    targMeshInd = 0;
    qDebug() << "Mesh created and puted in queue;";

    newRotations.clear(); newScales.clear();
    for (int i = 0; i < bendingRig->skeleton->joints.length(); i++){
        newRotations << Matrix<Derivable,1,3>(0,0,0);
        newScales << Matrix<Derivable,1,3>(1,1,1);
    }
    bendingRig->ApplyBending(bendingRig->skeleton->rootTransate, newRotations, newScales); //CompareWithMeshOnRotates(assTranslate, newRotations, newScales, targetMeshes[targMeshInd]->bindMesh).getValue();
    qDebug() << "Model transforms reseted;";
    eps = 1e-6; maxSteps = 20;
}

QString TestAutoRig::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective, const int width, const int hei)
{
    bendingRig->ApplyDrawToCanvas(painter, view, perspective, width, hei);
    targetMeshes[targMeshInd]->ApplyDrawToCanvas(painter, view, perspective, width, hei);

    painter->setPen(Qt::darkGray);
    painter->drawText(30, 30, 400, 400, 0,
                      "LMK + Shift -- scale;\nLMK + Cntrl -- move;\nLMK + Alt -- rotate;\n\nUp/Down -- change a target mesh;\n\nMethods:\nT -- test bending instrument and callbacking;\nB -- bend skeleton to current shape (green);\nL -- create custom low model with random scale, pos, rotation;\nH -- create custon high model with random scale, pos, rotation;\n\n\n");
    return QString();
}

float was = 0, zad = 0;
float TestAutoRig::TestSkinBending()
{
    QVector<Matrix<Derivable,1,3>> newRotations = QVector<Matrix<Derivable,1,3>>(bendingRig->skeleton->joints.length());
    QVector<Matrix<Derivable,1,3>> newScales = QVector<Matrix<Derivable,1,3>>();
    for (int i = 0; i < newRotations.length(); i++)
        newScales << Matrix<Derivable,1,3>(1,1,1);

    static float ang = 0; int times = 0;
    while (times++ < 4){
        newRotations[2](0,1) = ang;
        newRotations[3](0,2) = ang / 2;
        ang += 10;
        bendingRig->ApplyBending(bendingRig->skeleton->rootTransate, newRotations, newScales);
        window->repaint();
    }
    for (int i = 0; i < bendingRig->skeleton->joints.length(); i++)
    {
        if (bendingRig->skeleton->joints[i]->pater == NULL){ qDebug() << "ROOT";}
        qDebug() << " Joint " << i;
        if (bendingRig->skeleton->joints[i]->pater == NULL)
            TraceVector(bendingRig->skeleton->rootTransate);
        else{
            TraceVector(bendingRig->skeleton->joints[i]->localTranslation);
            qDebug()<< "^ WANT          v REAL";
            Matrix<Derivable,1,3> tmp = (bendingRig->skeleton->joints[i]->currentTranslation - bendingRig->skeleton->joints[i]->pater->currentTranslation);
            TraceVector(tmp);
        }
        TraceVector(bendingRig->skeleton->joints[i]->localScale);
    }
    qDebug() << "tested";
    return 0;

//    float res = -1; was = 0;
//    qDebug() << "Test bend called";
//    //  ASS TRANSLATE           JOINT ROTATES       JOINT SCALES
//    while (was < 100){


//        for (int i = 0; i < newRotations.length(); i++){
//            newScales << ((i != 20)? Matrix<Derivable,1,3>(1,1,1) : Matrix<Derivable,1,3>(1+was / 50.0,1+was / 50.0,1+was / 50.0));  // test an arm shit
//            newRotations[i] = Matrix<Derivable,1,3>(0,was * (i>5) * .02, 0);
//        }
//        //newRotations[0] = Matrix<Derivable,1,3>(was,was,was);
//        newRotations[20] = Matrix<Derivable,1,3>(0, was ,0);
//        Matrix<Derivable,1,3> assTranslate = Matrix<Derivable,1,3>(0,0,/*++was * .5*/0);
//        was += 2;
//        res = bendingRig->CompareWithMeshOnRotates(assTranslate, newRotations, newScales, targetMeshes[targMeshInd]->bindMesh).getValue();
//        qDebug() << "Call redraw " << was;
//        window->repaint();
//    }
//    qDebug() << "Test bend success";
//    return res;
}




