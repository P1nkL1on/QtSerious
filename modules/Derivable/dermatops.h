#ifndef DERMATOPS_H
#define DERMATOPS_H


#include "QMatrix4x4"
#include "QDebug"
#include "Eigen/Core"
#include "QVector"
#include "derivable.h"
#include "QString"

namespace DerivableVectorMatrixes {
    static bool Rigging = true;
    void TestTrace();
    Eigen::Matrix<DerOperations::Derivable, 1, 4> SetDerive4DVector (QVector4D vec);
    Eigen::Matrix<DerOperations::Derivable, 1, 4> SetDerive4DVector ();
    Eigen::Matrix<DerOperations::Derivable, 1, 4> SetDerive4DVector (float x, float y, float z, float w);
    Eigen::Matrix<DerOperations::Derivable, 1, 4> SetDerive4DVector (QVector3D qv3, float w);
    Eigen::Matrix<DerOperations::Derivable, 1, 3> SetDerive3DVector (const QVector3D qv3);

    void TraceVector (const Eigen::Matrix<DerOperations::Derivable, 1, 4> Vec);
    void TraceVector (const Eigen::Matrix<DerOperations::Derivable, 1, 3> Vec);

    void TraceMatrix (const Eigen::Matrix<DerOperations::Derivable, 4, 4> Mat);

    Eigen::Matrix<DerOperations::Derivable, 4, 4> SetDeriveMatrix ();

    Eigen::Matrix<DerOperations::Derivable, 4, 4> SetDeriveMatrix (const QVector<DerOperations::Derivable> values);

    Eigen::Matrix<DerOperations::Derivable, 4, 4> SetDeriveMatrix (const QMatrix4x4 original);

    Eigen::Matrix<DerOperations::Derivable, 4, 4> MakeDeriveTranslationMatrix (const Eigen::Matrix<DerOperations::Derivable, 1,3> vec, const bool Reverse);

    Eigen::Matrix<DerOperations::Derivable, 4, 4> MakeDeriveRotationXMatrix (const DerOperations::Derivable angle0);
    Eigen::Matrix<DerOperations::Derivable, 4, 4> MakeDeriveRotationZMatrix (const DerOperations::Derivable angle0);
    Eigen::Matrix<DerOperations::Derivable, 4, 4> MakeDeriveRotationYMatrix (const DerOperations::Derivable angle0);
    Eigen::Matrix<DerOperations::Derivable, 4, 4> MakeDeriveRotationMatrix (const Eigen::Matrix<DerOperations::Derivable,1,3> ang);

    Eigen::Matrix<DerOperations::Derivable, 4, 4> MakeDeriveScaleMatrix (const DerOperations::Derivable scale);
    Eigen::Matrix<DerOperations::Derivable, 4, 4> MakeDeriveScaleMatrix (const Eigen::Matrix<DerOperations::Derivable,1,3> scale);

    Eigen::Matrix<DerOperations::Derivable,1,4> MakeVector4From3 (const Eigen::Matrix<DerOperations::Derivable, 1,3> vec, const DerOperations::Derivable add);
    Eigen::Matrix<DerOperations::Derivable,1,3> Summ3and4 (const Eigen::Matrix<DerOperations::Derivable, 1,3> v3, const Eigen::Matrix<DerOperations::Derivable, 1,4> v4);
    DerOperations::Derivable SquaredLength (const Eigen::Matrix<DerOperations::Derivable, 1, 3> vect);
    QVector3D QfromDer3 (const Eigen::Matrix<DerOperations::Derivable, 1, 3> orig);

    QString ToString (const Eigen::Matrix<DerOperations::Derivable,1,3> vec);

    void TranslateDeriveMatrix (Eigen::Matrix<DerOperations::Derivable, 4, 4>& originalMatrix, const Eigen::Matrix<DerOperations::Derivable, 1,3> vec);
    //void TranslateRigDeriveMatrix (Matrix<DerOperations::Derivable, 4, 4>& originalMatrix, const Matrix<DerOperations::Derivable, 1,3> vec);
    void RotateDeriveMatrix (Eigen::Matrix<DerOperations::Derivable, 4, 4>& originalMatrix, const Eigen::Matrix<DerOperations::Derivable, 1, 3> rot);
    //void TurnRig();
    void ScaleDeriveMatrix (Eigen::Matrix<DerOperations::Derivable, 4, 4>& originalMatrix, const DerOperations::Derivable scale);
    //void TurnRig();
    void ScaleDeriveMatrix (Eigen::Matrix<DerOperations::Derivable, 4, 4>& originalMatrix, const Eigen::Matrix<DerOperations::Derivable,1,3> scale);
    //void TurnRig();
}

//IMPLEMENTATIONS


//using Eigen::Matrix;
using namespace DerOperations;
using namespace DerivableVectorMatrixes;


inline Eigen::Matrix<Derivable, 1, 4> DerivableVectorMatrixes::SetDerive4DVector (QVector4D vec) {
    Eigen::Matrix<Derivable, 1, 4> M = Eigen::Matrix<Derivable, 1, 4>();
    for (int i = 0; i < 4; i++)
        M(0,i) = Derivable(vec[i]);
    return M;
}
inline Eigen::Matrix<Derivable, 1, 4> DerivableVectorMatrixes::SetDerive4DVector (){
    return DerivableVectorMatrixes::SetDerive4DVector(QVector4D(0,0,0,0));
}
inline Eigen::Matrix<Derivable, 1, 4> DerivableVectorMatrixes::SetDerive4DVector (float x, float y, float z, float w){
    return SetDerive4DVector(QVector4D(x,y,z,w));
}
inline Eigen::Matrix<Derivable, 1, 4> DerivableVectorMatrixes::SetDerive4DVector (QVector3D qv3, float w){
    return SetDerive4DVector(QVector4D(qv3.x(),qv3.y(),qv3.z(),w));
}

inline void DerivableVectorMatrixes::TraceVector (const Eigen::Matrix<Derivable, 1, 4> Vec){
 QString lin = "Derivable4D (";
 for (int j = 0; j < 4; j++)
     lin += (QString::number( Vec(0, j).getValue()) + " (" + QString::number( Vec(0, j).getProiz()) + ")").rightJustified(19, ' ') + " ";
 qDebug() << lin + ")";
}
inline void DerivableVectorMatrixes::TraceVector (const Eigen::Matrix<Derivable, 1, 3> Vec){
 QString lin = "Derivable3D (";
 for (int j = 0; j < 3; j++)
     lin += (QString::number( Vec(0, j).getValue()) + " (" + QString::number( Vec(0, j).getProiz()) + ")").rightJustified(19, ' ') + " ";
 qDebug() << lin + ")";
}

inline void DerivableVectorMatrixes::TraceMatrix (const Eigen::Matrix<Derivable, 4, 4> Mat){
    qDebug() << "Matrix4x4 Derivable (type:Custom";
    for (int i = 0; i < 4; i++){
        QString lin = "";
        for (int j = 0; j < 4; j++)
            lin +=( (QString::number( Mat(j,i).getValue()))+ " (" + QString::number( Mat(i,j).getProiz()) + ")").rightJustified(24, ' ') + " ";
        qDebug() << lin;
    }
    qDebug() << ")";
}

inline Eigen::Matrix<Derivable, 4, 4> DerivableVectorMatrixes::SetDeriveMatrix (){
    Eigen::Matrix<Derivable, 4, 4> M =Eigen::Matrix<Derivable, 4, 4>();
    for (int i = 0; i < 4; i++)
        M(i,i) = Derivable(1);
    return M;
}

inline Eigen::Matrix<Derivable, 4, 4> DerivableVectorMatrixes::SetDeriveMatrix (const QVector<Derivable> values){
    Q_ASSERT(values.length() == 16);
    Eigen::Matrix<Derivable, 4, 4> M = Eigen::Matrix<Derivable, 4, 4>();
    M(0,0) = values[0];
    M(0,1) = values[1];
    M(0,2) = values[2];
    M(0,3) = values[3];
    M(1,0) = values[4];
    M(1,1) = values[5];
    M(1,2) = values[6];
    M(1,3) = values[7];
    M(2,0) = values[8];
    M(2,1) = values[9];
    M(2,2) = values[10];
    M(2,3) = values[11];
    M(3,0) = values[12];
    M(3,1) = values[13];
    M(3,2) = values[14];
    M(3,3) = values[15];
    return M;
}

inline Eigen::Matrix<Derivable, 4, 4> DerivableVectorMatrixes::SetDeriveMatrix (const QMatrix4x4 original){
    Eigen::Matrix<Derivable, 4, 4> M = Eigen::Matrix<Derivable, 4, 4>();
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++)M(i,j) = Derivable(original(i,j));
    return M;
}

inline Eigen::Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveTranslationMatrix (const Eigen::Matrix<Derivable,1,3> vec, const bool Reverse){
    Eigen::Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
        if (!Reverse){
            M(0,3) = vec(0,0);
            M(1,3) = vec(0,1);
            M(2,3) = vec(0,2);
        }else{
            M(3, 0) = vec(0,0);
            M(3, 1) = vec(0,1);
            M(3, 2) = vec(0,2);
        }
    return M;
}

inline Eigen::Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveRotationXMatrix (const Derivable angle0){
    Derivable angle = angle0 / 180.0 * M_PI;
    Eigen::Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    M(1,1) = DerOperations::cos(angle); M(2,1) = Derivable(-1)* DerOperations::sin(angle);
    M(1,2) = DerOperations::sin(angle); M(2,2) = DerOperations::cos(angle);
    return M;
}
inline Eigen::Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveRotationZMatrix (const Derivable angle0){
    Derivable angle = angle0 / 180.0 * M_PI;
    Eigen::Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    M(0,0) = DerOperations::cos(angle); M(1,0) = Derivable(-1)*  DerOperations::sin(angle);
    M(0,1) = DerOperations::sin(angle); M(1,1) = DerOperations::cos(angle);
    return M;
}
inline Eigen::Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveRotationYMatrix (const Derivable angle0){
    Derivable angle = angle0 / 180.0 * M_PI;
    Eigen::Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    M(0,0) = DerOperations::cos(angle); M(2,0) = DerOperations::sin(angle);
    M(0,2) =  Derivable(-1)*  DerOperations::sin(angle); M(2,2) = DerOperations::cos(angle);
    return M;
}
inline Eigen::Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveRotationMatrix (const Eigen::Matrix<Derivable, 1,3> ang){
    return MakeDeriveRotationZMatrix(ang(0,2)) * MakeDeriveRotationYMatrix(ang(0,1)) * MakeDeriveRotationXMatrix(ang(0,0));
}

inline void DerivableVectorMatrixes::TranslateDeriveMatrix (Eigen::Matrix<Derivable, 4, 4>& originalMatrix, const Eigen::Matrix<Derivable, 1,3> vec){
    originalMatrix = originalMatrix * MakeDeriveTranslationMatrix(vec, true);
}
//void DerivableVectorMatrixes::TranslateRigDeriveMatrix (Matrix<Derivable, 4, 4>& originalMatrix, const Matrix<Derivable, 1,3> vec){
//    originalMatrix = originalMatrix * MakeDeriveTranslationMatrix(vec, false);
//}
inline void DerivableVectorMatrixes::RotateDeriveMatrix (Eigen::Matrix<Derivable, 4, 4>& originalMatrix, const Eigen::Matrix<Derivable, 1,3> rot){
    originalMatrix = originalMatrix * MakeDeriveRotationMatrix(rot);
}
inline void DerivableVectorMatrixes::TestTrace()
{
    QVector4D vt = QVector4D(10,11,12,1);
    QMatrix4x4 mat = QMatrix4x4();
    qDebug() << mat;
    mat.rotate(14500, 1, 0, 0);
    mat.rotate(8, 0, 1, 0);
    mat.rotate(12, 0, 0, 1);
    qDebug() << mat;
    mat.translate(13,14,-15);
    qDebug() << mat;
    qDebug() << mat * vt;

    Eigen::Matrix<Derivable,1,4> vd =Eigen::Matrix<Derivable,1,4>(10,11,12,1);
    Eigen::Matrix<Derivable,4,4> dat = SetDeriveMatrix();
    TraceMatrix(dat);
    TranslateDeriveMatrix(dat, Eigen::Matrix<Derivable,1,3>(13,14,-15));
    TraceMatrix(dat);
    RotateDeriveMatrix(dat, Eigen::Matrix<Derivable,1,3>(14500,8,12));

    TraceMatrix(dat);
    vd = vd * dat;
    TraceVector(vd);
    return;
}



inline Eigen::Matrix<Derivable, 1, 4> DerivableVectorMatrixes::MakeVector4From3(const Eigen::Matrix<Derivable, 1, 3> vec, const Derivable add)
{
    return Eigen::Matrix<Derivable, 1, 4>(vec(0,0), vec(0,1), vec(0,2), add);
}

inline Eigen::Matrix<Derivable, 1, 3> DerivableVectorMatrixes::Summ3and4(const Eigen::Matrix<Derivable, 1, 3> v3, const Eigen::Matrix<Derivable, 1, 4> v4)
{
    return Eigen::Matrix<Derivable, 1, 3>(v3(0,0) + v4(0,0), v3(0,1) + v4(0,1), v3(0,2) + v4(0,2));
}

inline Derivable DerivableVectorMatrixes::SquaredLength(const Eigen::Matrix<Derivable, 1, 3> vect)
{
    return  DerOperations::pow(vect(0,0),2) + DerOperations::pow(vect(0,1),2) + DerOperations::pow(vect(0,2),2);
}

inline QVector3D DerivableVectorMatrixes::QfromDer3(const Eigen::Matrix<Derivable, 1, 3> orig)
{
    return QVector3D(orig(0,0).getValue(), orig(0,1).getValue(), orig(0,2).getValue());
}

//void DerivableVectorMatrixes::TurnRig()
//{
//    qDebug() << "!!!!!SWAPPED!!!!!!!!";
//    Rigging = !Rigging;
//    qDebug() << "!!!!!! " << Rigging;
//}

inline QString DerivableVectorMatrixes::ToString(const Eigen::Matrix<Derivable, 1, 3> vec)
{
    QString res = "", respr = "     pr:(";
    for (int i = 0; i < 3; i ++)
    {
        res += QString::number(vec(0,i).getValue());
        respr += QString::number(vec(0,i).getProiz());
        if (i < 2){res += ", "; respr += ", ";}
    }
    return res + respr + ")";
}

inline Eigen::Matrix<Derivable, 1, 3> DerivableVectorMatrixes::SetDerive3DVector(const QVector3D qv3)
{
    return Eigen::Matrix<Derivable,1,3>(qv3.x(), qv3.y(), qv3.z());
}

inline Eigen::Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveScaleMatrix(const Derivable scale)
{
    Eigen::Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    for (int i = 0; i < 3; i++)
        M(i,i) = scale;
    M(3,3) = Derivable(1, /*scale.getProiz()*/0);
    return M;
}
inline Eigen::Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveScaleMatrix(const Eigen::Matrix<Derivable,1,3> scale)
{
    Eigen::Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    for (int i = 0; i < 3; i++)
        M(i,i) = scale(0,i);
    M(3,3) = Derivable(1, /*scale.getProiz()*/0);
    return M;
}

inline void DerivableVectorMatrixes::ScaleDeriveMatrix(Eigen::Matrix<Derivable, 4, 4> &originalMatrix, const Derivable scale)
{
    originalMatrix = originalMatrix * MakeDeriveScaleMatrix(scale);
}
inline void DerivableVectorMatrixes::ScaleDeriveMatrix(Eigen::Matrix<Derivable, 4, 4> &originalMatrix, const Eigen::Matrix<Derivable,1,3> scale)
{
    originalMatrix = originalMatrix * MakeDeriveScaleMatrix(scale);
}


#endif // DERMATOPS_H
