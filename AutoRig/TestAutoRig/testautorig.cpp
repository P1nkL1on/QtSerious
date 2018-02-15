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

    for (int i = 0; i < 1; i++)
        QVector<float> resAngles = OptimiseMethods::GaussNewtonMethod(loss, firstAngles, 1e-5, 20, gt, i != 0);
    qDebug() << "Uber << Qasi Newtone EXIT SUCCESS";
}

bool TestAutoRig::UberBugHunt()
{
    bool trace = true;
    if (trace)
        qDebug() << "I wish you a good hunt";
    Modeling();

//    bendingRig->skeleton->SetRootTranslation(rootTrans);
//    bendingRig->skeleton->SetRotations(newRotations);
//    bendingRig->skeleton->SetScales(newScales);

//    bendingRig->BendSkinToSkeleton();
//    return bendingRig->bendedMesh->CompareWithAnotherMeshCoords(with);

    QVector<Derivable> values = QVector<Derivable>();
    values << 0<<0<<0<<  0<<0<<0<<0<<0<<0<< 1<<1;
    Matrix<float,-1,-1> resJacobian = Matrix<float,-1,-1>(12, values.length()), resJacobianNum = Matrix<float,-1,-1>(12, values.length());
    Eigen::VectorXf resF = Eigen::VectorXf(12),  resFNum = Eigen::VectorXf(12);

    Derivable dx = 1e-4;

    for (int PARAMWITH1 = 0; PARAMWITH1 < values.length() * 2; PARAMWITH1++){
        //if (PARAMWITH1 == 9 || PARAMWITH1 == 20)
        {
            //int x = 10;
            trace = (PARAMWITH1 == 9);
        }
        if ( PARAMWITH1 < values.length())
            values[PARAMWITH1].setPrValue(1);
        else
            values[PARAMWITH1 - values.length()] = values[PARAMWITH1 - values.length()] + dx;
        Matrix<Derivable,1,3> root = Matrix<Derivable,1,3>(values[0], values[1], values[2]),
                              rot0 = Matrix<Derivable,1,3>(values[3], values[4], values[5]),
                              rot1 = Matrix<Derivable,1,3>(values[6], values[7], values[8]);
        Derivable sc0 = values[9], sc1 = values[10];
        if ( PARAMWITH1 < values.length())
            values[PARAMWITH1].setPrValue(0);
        else
            values[PARAMWITH1 - values.length()] = values[PARAMWITH1 - values.length()] - dx;

        // goto bending

        // go to recalculate all
            // go to recalculate all local recursive
    //    localTransformMatrix = SetDeriveMatrix();

    //    Matrix<Derivable,1,3> currentRotation2 = (pater != NULL)?pater->currentRotation : Matrix<Derivable,1,3>(0,0,0);
    //    if (pater != NULL)
    //         ScaleDeriveMatrix(localTransformMatrix, Derivable(1) / pater->localScale);
    //    ScaleDeriveMatrix(localTransformMatrix, localScale);    // scale self
    //    TranslateDeriveMatrix(localTransformMatrix, localTranslation);
    //    RotateDeriveMatrix(localTransformMatrix, currentRotation2);
        Matrix<Derivable,4,4> localTransform0 = SetDeriveMatrix(), localTransform1 = SetDeriveMatrix(), globalTransform0 = SetDeriveMatrix(), globalTransform1 = SetDeriveMatrix();

        ScaleDeriveMatrix(localTransform0, sc0);    // scale self
        TranslateDeriveMatrix(localTransform0, root);
        RotateDeriveMatrix(localTransform0, rot0);

        ScaleDeriveMatrix(localTransform1,/* Derivable(1) / sc0*/ Derivable(1/sc0.getValue(), -sc0.getProiz()/(sc0.getValue() * sc0.getValue())));
        ScaleDeriveMatrix(localTransform1, sc1);    // scale self
        TranslateDeriveMatrix(localTransform1, Matrix<Derivable,1,3> (0,40,0));
        RotateDeriveMatrix(localTransform1, rot1);



        int X = 10;
         if (trace){
            qDebug() << "local transform 0";
            TraceMatrix(localTransform0);

            qDebug() << "local transform 1";
            TraceMatrix(localTransform1);
        }

        // local a normal
        Matrix<Derivable,1,4> tmp;
        globalTransform0 = globalTransform0 * localTransform0;

        tmp = Matrix<Derivable,1,4>(1,1,1,1) * globalTransform0;
        Matrix<Derivable,1,3> trans0 = Matrix<Derivable,1,3>(tmp(0,0), tmp(0,1), tmp(0,2));


        globalTransform1 = globalTransform1 * localTransform1;
        globalTransform1 = globalTransform1 * localTransform0;

        tmp = Matrix<Derivable,1,4>(1,1,1,1) * globalTransform1;
        Matrix<Derivable,1,3> trans1 = Matrix<Derivable,1,3>(tmp(0,0), tmp(0,1), tmp(0,2));

        // globalTransform1(1,3).setPrValue(40);
        // globalTransform1(3,1).setPrValue(0);

        if (trace){
            qDebug() << "global transform 0";
            TraceMatrix(globalTransform0);
            TraceVector(trans0);

            qDebug() << "global transform 1";
            TraceMatrix(globalTransform1);
            TraceVector(trans1);
        }

        Mesh* newMesh = new Mesh();
        for (int currentVertexInd = 0; currentVertexInd < bendingRig->skin->vertAttends.length(); currentVertexInd ++){
            QVector<Matrix<Derivable,1,3>> bendedVariants;
            QVector<float> weightes;

            // we have 0.3 && QVec3D && from joint a place and rotation
            for (int jointInd = 0; jointInd < bendingRig->skin->vertAttends[currentVertexInd].jointIndexs.length(); jointInd++){
                int jointBendInd = bendingRig->skin->vertAttends[currentVertexInd].jointIndexs[jointInd];

                Matrix<Derivable,4,4>
                       localAttendTransformMatrix = SetDeriveMatrix(),
                       localRotateMatrix = MakeDeriveRotationMatrix((jointBendInd == 0)? rot0 : rot1);

                TranslateDeriveMatrix(localAttendTransformMatrix, - bendingRig->skin->vertAttends[currentVertexInd].localJointCoords[jointInd]);

                tmp =  Matrix<Derivable,1,4>(1,1,1,1) * (localAttendTransformMatrix * localRotateMatrix * ((jointBendInd == 0)? globalTransform0 : globalTransform1));
                bendedVariants << Matrix<Derivable,1,3>(tmp(0,0), tmp(0,1), tmp(0,2));
                weightes << bendingRig->skin->vertAttends[currentVertexInd].weights[jointInd];
            }

            Matrix<Derivable,1,3> result = Matrix<Derivable,1,3>();
            float bendedSumm = 0;
            for (int curPoint = 0; curPoint < weightes.length(); curPoint++)
                bendedSumm += weightes[curPoint];

            // select the middle, based on wighthres
            for (int curPoint = 0; curPoint < bendedVariants.length(); curPoint++)
                result = result +  Derivable(weightes[curPoint]/ bendedSumm) * bendedVariants[curPoint] ;
            //result = bendedVariants[0];

            newMesh->vertexes << result;

        }
        trace = (PARAMWITH1 == 9 || PARAMWITH1 == 10);
         if (trace){
             //targetMeshes[targMeshInd]->bindMesh
             if (PARAMWITH1 == 9){
//                 newMesh->vertexes[2](0,0).setPrValue(0);
//                 newMesh->vertexes[1](0,0).setPrValue(0);
//                 newMesh->vertexes[3](0,1).setPrValue(40);
             }

            qDebug() << ">>>>>>>>>>> VERTEXES <<<<<<<<<<" << PARAMWITH1;
            for (int i = 0; i < newMesh->vertexes.length(); i++)
                TraceVector(newMesh->vertexes[i]);

        }
        QVector<Derivable> res = QVector<Derivable>();
        for (int vInd = 0; vInd < newMesh->vertexes.length(); vInd++){
            Matrix<Derivable,1,3> dist = newMesh->vertexes[vInd] - targetMeshes[targMeshInd]->bindMesh->vertexes[vInd];
            res << dist(0,0) << dist(0,1) << dist(0,2);
        }
//        QString s = "";
//        for (int i = 0; i < res.length(); i++)
//            s +=  QString::number(res[i].getProiz()).leftJustified(14, ' ');
//        qDebug() << s;
        Eigen::VectorXf rescol = Eigen::VectorXf(res.length());
        for (int i = 0; i < res.length(); i++){
            rescol[i] = res[i].getProiz();
            if ( PARAMWITH1 < values.length())
                resF[i] = res[i].getValue();
            else
                resFNum[i] = res[i].getValue();
        }
        if ( PARAMWITH1 < values.length())
            resJacobian.col(PARAMWITH1) = rescol;
        else
            resJacobianNum.col(PARAMWITH1 - values.length()) = ((resFNum - resF) / (dx.getValue())).cast<float>();
    }
    qDebug() << ">>Handmade shit AUTODIFF";
    OptimiseMethods::TraceJacobianM(resJacobian.transpose() * resF * .5);
    OptimiseMethods::TraceJacobianM(resF * .5);
    qDebug() << ">>Handmade shit NUMERICAL";
    OptimiseMethods::TraceJacobianM(resJacobianNum.transpose() * resFNum * .5);
    OptimiseMethods::TraceJacobianM(resFNum * .5);
    return true;
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
    Derivable scale = Derivable(1.29);//0.5 + qrand() % 100 / 100.0;
    Matrix<Derivable,1,3> trans = Matrix<Derivable,1,3>(0,0,0);//(2.7, 6.9, 9.5//Matrix<Derivable,1,3>(qrand() % 200 - 100,qrand() % 200 - 100,qrand() % 200 - 100);
    Matrix<Derivable,1,3> rotat = Matrix<Derivable,1,3>(0,0,0);//102, 350, 162//Matrix<Derivable,1,3>(qrand() % 360,qrand() % 360,qrand() % 360);

    TraceVector(trans);
    TraceVector(rotat);
    qDebug() << "scale: " << scale.getValue();

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
        newScales << ((i % 5 != 0)? Derivable(1) : Derivable(2));  // test an arm shit
    Matrix<Derivable,1,3> assTranslate;// = Matrix<Derivable,1,3>(0,0,++was * .5);

///   was += 1;
///    for (int i = 0; i < newRotations.length(); i++)
///        newRotations[i] = Matrix<Derivable,1,3>(was * (i + 1) , 0, 0);
//    newRotations[8] = Matrix<Derivable,1,3>(0, 90,was * 5 + 90);
//    newRotations[31] = Matrix<Derivable,1,3>(was * 30,0,0);
//    newRotations[36] = Matrix<Derivable,1,3>(180 + was * 30,0,0);

    float res = bendingRig->CompareWithMeshOnRotates(assTranslate, newRotations, newScales, targetMeshes[targMeshInd]->bindMesh).getValue();
    qDebug() << "Test bend success";
    return res;
}




