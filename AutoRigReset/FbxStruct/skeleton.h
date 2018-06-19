#ifndef FBXSTRUCT_SKELETON_H
#define FBXSTRUCT_SKELETON_H


#include "DerStruct/defines.h"
#include "FbxStruct/joint.h"

namespace FbxStruct{

//class Skeleton
//{
//public:
//    Skeleton() = default;
//    Skeleton(const QVector<Joint> &joints);
//    void calculateMatrixes();
//    QVector<Df::Vector3<double>> jointTranslations;
//    QVector<int> getKidsByIndex (const int index) const;
//    int getPaterByIndex (const int index) const;
//private:
//    QVector<Joint> joints;
//    QVector<int> rootIndexes;
//    Df::Matrix4<double> calculateLocalTransformByIndex (const int index);
//    Df::Matrix4<double> getLocalMatrixByIndex (const int index) const;
//    Df::Matrix4<double> getGlobalMatrixByIndex (const int index) const;
//    Df::Vector3<double> getInverseScale (const int index) const;
//    void calculateMatrix(const int currentJointIndex);


//    //Matrix4d computeLocalMatrix(/*some stuff like translation, rotation, etc*/)
//    //void computeLocalMatrices(/*Some information about all translations, rotations, etc*/, QVector<Matrix4d> &outLocalMatrices)
//    //QVector<Matrix4d> computeLocalMatrices(/*Some information about all translations, rotations, etc*/)
//    //global ...

//};

class Skeleton
{
public:
    Skeleton() = default;
    Skeleton(const QVector<Joint> &joints,
             const QVector<JointTransform> &jointTransforms,
             const QVector<Df::Matrix4<double>> &bindMatrices);
    static QVector<Df::Matrix4<double>> computeLocalMatrices(
            const QVector<JointTransform> &jointTrans,
            const QVector<Df::Vector3<double>> &paterInverseScales);
    static QVector<Df::Matrix4<double>> computeGlobalMatrices(const QVector<Joint> &joints,
            const QVector<Df::Matrix4<double>> &localMatrices);
    QVector<Df::Vector3<double>> computeJointPositions() const;

    int getPaterByIndex (const int index) const;
    QString getNameByIndex (const int index) const;
    QVector<int> getClustersByJointIndex (const int index) const;
private:
    QVector<Joint> joints;
    QVector<JointTransform> jointTransforms;
    QVector<Df::Matrix4<double>> bindMatrices;
    static Df::Matrix4<double> computeLocalMatrix(
            const JointTransform &jointTrans,
            const Df::Vector3<double> &paterInverseScale);
    static bool computeGlobalMatrix(const QVector<Joint> &joints,
            const QVector<Df::Matrix4<double>> &localMatrices,
            QVector<Df::Matrix4<double>> &globalMatrices,
            const int currentJointIndex);
};

}

#endif // FBXSTRUCT_SKELETON_H
