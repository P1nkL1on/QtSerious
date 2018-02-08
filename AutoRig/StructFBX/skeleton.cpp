#include "skeleton.h"
#include "qdebug.h"

using namespace DerOperations;
using Eigen::Matrix;


Skeleton::Skeleton()
{
    localRotations = {};
    joints = {};
}

Skeleton::Skeleton(QVector<Joint *> j)
{
    joints = j;
    localRotations = {};
    for (int lclId = 0; lclId < j.length(); lclId++){
        localRotations << Matrix<Derivable,1,3>();
        if (joints[lclId]->pater == NULL)
            rootTransate = joints[lclId]->localTranslation;
    }
}

void Skeleton::SetNullRotations()
{
    localRotations = {};
    for (int lclId = 0; lclId < joints.length(); lclId++)
        localRotations << Matrix<Derivable,1,3>();
}


void TraceJoint (const Joint* who, const int depth){
    QString res = ""; for (int i = 0; i < depth; i++)res += "|  ";
    qDebug() << res + who->name;
    for (int kidInd = 0; kidInd < who->kids.length(); kidInd++)
        TraceJoint (who->kids[kidInd], depth + 1);
    return;
}

void Skeleton::DebugTree() const
{
    if (joints.length() <= 0)
    { qDebug () << "No bones;"; return; }
    TraceJoint(joints[0], 0);

    for (int curJoint = 0; curJoint < joints.length(); curJoint++)
        qDebug () << QString::number(curJoint) + ". " + joints[curJoint]->name;
}

void Skeleton::RecursiveApplyLocalRotations(Joint *joint, Matrix<Derivable,1,3> currentRotation)
{
    transformesApplied ++ ;
    Matrix<Derivable,1,3> wasRotation = (joint->pater == NULL)? joint->currentRotation : joint->pater->currentRotation,
              newRotation = wasRotation + currentRotation,
              cTranslation = (joint->pater == NULL)? joint->currentTranslation : joint->pater->currentTranslation;

    //qDebug() << joint->name << wasRotation << currentRotation << " ----  " << newRotation<< " ----  " << joint->localTranslation;
    joint->currentTranslation =
            CommonFuncs::AddDirectWtParent(cTranslation, joint->localTranslation, wasRotation, currentRotation);
            //CommonFuncs::AddDirect(cTranslation, joint->localTranslation, newRotation);
    joint->currentRotation = newRotation;

    for (int childId = 0; childId < joint->kids.length(); childId++)
        RecursiveApplyLocalRotations(joint->kids[childId], newRotation);

}

bool EqualSkeletonRotations (QVector<Matrix<Derivable,1,3>> a, QVector<Matrix<Derivable,1,3>> b){
    Q_ASSERT(a.length() == b.length());
    for (int rotInd = 0; rotInd < a.length(); rotInd++)
        if (a != b)//((a[rotInd](0,0) != b[rotInd](0,0)) || (a[rotInd](0,1) != b[rotInd](0,1)) || (a[rotInd](0,2) != b[rotInd](0,2)))
            return false;
    return true;
}

void RecursiveGlobalCalculateCall (Joint* joint){
    joint->CalculateGlobalTransformMatrix();

    //qDebug() << joint->name << joint->currentTranslation;
    joint->currentTranslation = CommonFuncs::AddDirectMatrx(/*joint->localTranslation*/Matrix<Derivable,1,3>(1,1,1),
                                                            joint->globalTransformMatrix);
    //qDebug() << joint->name << joint->currentTranslation;

    for (int childId = 0; childId < joint->kids.length(); childId++)
        RecursiveGlobalCalculateCall(joint->kids[childId]);
}

// MAIN METHOD  OF GETTING SKELETON SEEMS <CHOOL'>

bool Skeleton::CalculateGlobalCoordForEachJointMatrix()
{
    transformesApplied = 0;
    if (EqualSkeletonRotations(getJointsLocalRotations(), localRotations))
        return false;
    //
    QVector<int> rootInds = {};

    for (int curJoint = 0; curJoint < joints.length(); curJoint++){
        joints[curJoint]->currentTranslation = Matrix<Derivable,1,3>();              // reset XYZ to calculate them further
        joints[curJoint]->currentRotation = localRotations[curJoint];               // now each joint have info about it need angles
        //qDebug() << curJoint << joints[curJoint]->currentRotation;
        if (joints[curJoint]->pater == NULL){
            rootInds << curJoint;
            joints[curJoint]->localTranslation = rootTransate;
            for (int i = 0; i < 3; i++)
                if (rootTransate(0,i).getProiz() != 0)
                    qDebug() << "WOWOWOWOOW" << i;
        }
    }
    for (int jointInd = 0; jointInd < joints.length(); jointInd++)
        joints[jointInd]->RecaulculateLocalTransformMatrix();

    // apply for roots and go further
    Q_ASSERT(rootInds.length() > 0);
    for (int curRootInd = 0; curRootInd < rootInds.length(); curRootInd ++){
        //qDebug() << "Root from " << curRootInd;
        Joint* root = (joints[rootInds[curRootInd]]);
        RecursiveGlobalCalculateCall(root);
    }
    return true;
}


QVector<Matrix<Derivable,1,3>> Skeleton::getJointsLocalRotations() const
{
    QVector<Matrix<Derivable,1,3>> res;
    for (int curJoint = 0; curJoint < joints.length(); curJoint++)
        res << joints[curJoint]->currentRotation;
    return res;
}



Matrix<Derivable,1,3> Skeleton::getJointCoordByIndex(int index, Matrix<Derivable,1,3> &paterCoord)
{
    if (joints[index]->pater == NULL)
        paterCoord = joints[index]->currentTranslation;
    else
        paterCoord = joints[index]->pater->currentTranslation;
    return joints[index]->currentTranslation;
}

void Skeleton::SetRotation(const Matrix<Derivable,1,3> newRotation, int jointInd)
{
    localRotations[jointInd] = newRotation; //! change -
}

void Skeleton::SetRotations(const QVector<Matrix<Derivable, 1, 3> > newRotations){
    Q_ASSERT(newRotations.length() == localRotations.length());
    for (int curJ = 0; curJ < localRotations.length(); curJ++)
        SetRotation(newRotations[curJ], curJ);
}

void Skeleton::SetRootTranslation(const Eigen::Matrix<Derivable, 1, 3> assTranslate)
{
    rootTransate = assTranslate;
}

void Skeleton::SetScales(const QVector<Derivable> newScales)
{
    Q_ASSERT(newScales.length() == localScales.length());
    for (int curJ = 0; curJ < localScales.length(); curJ++)
        localScales[curJ] = newScales[curJ];
}

bool Skeleton::getJointTranslationAndRotation(const int jointIndex, Matrix<Derivable,1,3> &translation, Matrix<Derivable,1,3> &rotation) const
{
    translation = joints[jointIndex]->currentTranslation;
    rotation = Matrix<Derivable,1,3>(); Joint* jn = joints[jointIndex];
    do{
        rotation = rotation + jn->currentRotation;
        jn = jn->pater;
    } while ( jn != NULL );
    return true;
}

QVector<Matrix<Derivable,1,3>> Skeleton::getJointsGlobalTranslationsForSkin() const
{
    QVector<Matrix<Derivable,1,3>> res;
    for (int curJoint = 0; curJoint < joints.length(); curJoint++)
            res << joints[curJoint]->currentTranslation;
    return res;
}
