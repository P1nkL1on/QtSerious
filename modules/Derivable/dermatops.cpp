#include "dermatops.h"
#include "iostream"



using Eigen::Matrix;
using namespace DerOperations;

using namespace DerivableVectorMatrixes;
using namespace  std;

Matrix<Derivable, 1, 4> DerivableVectorMatrixes::SetDerive4DVector (QVector4D vec) {
    Matrix<Derivable, 1, 4> M = Matrix<Derivable, 1, 4>();
    for (int i = 0; i < 4; i++)
        M(0,i) = Derivable(vec[i]);
    return M;
}
Matrix<Derivable, 1, 4> DerivableVectorMatrixes::SetDerive4DVector (){
    return DerivableVectorMatrixes::SetDerive4DVector(QVector4D(0,0,0,0));
}
Matrix<Derivable, 1, 4> DerivableVectorMatrixes::SetDerive4DVector (float x, float y, float z, float w){
    return SetDerive4DVector(QVector4D(x,y,z,w));
}
Matrix<Derivable, 1, 4> DerivableVectorMatrixes::SetDerive4DVector (QVector3D qv3, float w){
    return SetDerive4DVector(QVector4D(qv3.x(),qv3.y(),qv3.z(),w));
}

void DerivableVectorMatrixes::TraceVector (const Matrix<Derivable, 1, 4> Vec){
 QString lin = "Derivable4D (";
 for (int j = 0; j < 4; j++)
     lin += (QString::number( Vec(0, j).getValue()) + " (" + QString::number( Vec(0, j).getProiz()) + ")").rightJustified(19, ' ') + " ";
 qDebug() << lin + ")";
}
void DerivableVectorMatrixes::TraceVector (const Matrix<Derivable, 1, 3> Vec){
 QString lin = "Derivable3D (";
 for (int j = 0; j < 3; j++)
     lin += (QString::number( Vec(0, j).getValue()) + " (" + QString::number( Vec(0, j).getProiz()) + ")").rightJustified(19, ' ') + " ";
 qDebug() << lin + ")";
}

void DerivableVectorMatrixes::TraceMatrix (const Matrix<Derivable, 4, 4> Mat){
    qDebug() << "Matrix4x4 Derivable (type:Custom";
    for (int i = 0; i < 4; i++){
        QString lin = "";
        for (int j = 0; j < 4; j++)
            lin +=( (QString::number( Mat(j,i).getValue()))+ " (" + QString::number( Mat(i,j).getProiz()) + ")").rightJustified(24, ' ') + " ";
        qDebug() << lin;
    }
    qDebug() << ")";
}

Matrix<Derivable, 4, 4> DerivableVectorMatrixes::SetDeriveMatrix (){
    Matrix<Derivable, 4, 4> M = Matrix<Derivable, 4, 4>();
    for (int i = 0; i < 4; i++)
        M(i,i) = Derivable(1);
    return M;
}

Matrix<Derivable, 4, 4> DerivableVectorMatrixes::SetDeriveMatrix (const QVector<Derivable> values){
    Q_ASSERT(values.length() == 16);
    Matrix<Derivable, 4, 4> M = Matrix<Derivable, 4, 4>();
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

Matrix<Derivable, 4, 4> DerivableVectorMatrixes::SetDeriveMatrix (const QMatrix4x4 original){
    Matrix<Derivable, 4, 4> M = Matrix<Derivable, 4, 4>();
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++)M(i,j) = Derivable(original(i,j));
    return M;
}

Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveTranslationMatrix (const Matrix<Derivable,1,3> vec, const bool Reverse){
    Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
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

Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveRotationXMatrix (const Derivable angle0){
    Derivable angle = angle0 / 180.0 * M_PI;
    Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    M(1,1) = DerOperations::cos(angle); M(2,1) = Derivable(-1)* DerOperations::sin(angle);
    M(1,2) = DerOperations::sin(angle); M(2,2) = DerOperations::cos(angle);
    return M;
}
Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveRotationZMatrix (const Derivable angle0){
    Derivable angle = angle0 / 180.0 * M_PI;
    Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    M(0,0) = DerOperations::cos(angle); M(1,0) = Derivable(-1)*  DerOperations::sin(angle);
    M(0,1) = DerOperations::sin(angle); M(1,1) = DerOperations::cos(angle);
    return M;
}
Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveRotationYMatrix (const Derivable angle0){
    Derivable angle = angle0 / 180.0 * M_PI;
    Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    M(0,0) = DerOperations::cos(angle); M(2,0) = DerOperations::sin(angle);
    M(0,2) =  Derivable(-1)*  DerOperations::sin(angle); M(2,2) = DerOperations::cos(angle);
    return M;
}
Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveRotationMatrix (const Matrix<Derivable, 1,3> ang){
    return MakeDeriveRotationZMatrix(ang(0,2)) * MakeDeriveRotationYMatrix(ang(0,1)) * MakeDeriveRotationXMatrix(ang(0,0));
}

void DerivableVectorMatrixes::TranslateDeriveMatrix (Matrix<Derivable, 4, 4>& originalMatrix, const Matrix<Derivable, 1,3> vec){
    originalMatrix = originalMatrix * MakeDeriveTranslationMatrix(vec, true);
}
//void DerivableVectorMatrixes::TranslateRigDeriveMatrix (Matrix<Derivable, 4, 4>& originalMatrix, const Matrix<Derivable, 1,3> vec){
//    originalMatrix = originalMatrix * MakeDeriveTranslationMatrix(vec, false);
//}
void DerivableVectorMatrixes::RotateDeriveMatrix (Matrix<Derivable, 4, 4>& originalMatrix, const Matrix<Derivable, 1,3> rot){
    originalMatrix = originalMatrix * MakeDeriveRotationMatrix(rot);
}
void DerivableVectorMatrixes::TestTrace()
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

    Matrix<Derivable,1,4> vd =Matrix<Derivable,1,4>(10,11,12,1);
    Matrix<Derivable,4,4> dat = SetDeriveMatrix();
    TraceMatrix(dat);
    TranslateDeriveMatrix(dat, Matrix<Derivable,1,3>(13,14,-15));
    TraceMatrix(dat);
    RotateDeriveMatrix(dat, Matrix<Derivable,1,3>(14500,8,12));

    TraceMatrix(dat);
    vd = vd * dat;
    TraceVector(vd);
    return;
}



Matrix<Derivable, 1, 4> DerivableVectorMatrixes::MakeVector4From3(const Matrix<Derivable, 1, 3> vec, const Derivable add)
{
    return Matrix<Derivable, 1, 4>(vec(0,0), vec(0,1), vec(0,2), add);
}

Matrix<Derivable, 1, 3> DerivableVectorMatrixes::Summ3and4(const Matrix<Derivable, 1, 3> v3, const Matrix<Derivable, 1, 4> v4)
{
    return Matrix<Derivable, 1, 3>(v3(0,0) + v4(0,0), v3(0,1) + v4(0,1), v3(0,2) + v4(0,2));
}

Derivable DerivableVectorMatrixes::SquaredLength(const Matrix<Derivable, 1, 3> vect)
{
    return  DerOperations::pow(vect(0,0),2) + DerOperations::pow(vect(0,1),2) + DerOperations::pow(vect(0,2),2);
}

QVector3D DerivableVectorMatrixes::QfromDer3(const Matrix<Derivable, 1, 3> orig)
{
    return QVector3D(orig(0,0).getValue(), orig(0,1).getValue(), orig(0,2).getValue());
}

//void DerivableVectorMatrixes::TurnRig()
//{
//    qDebug() << "!!!!!SWAPPED!!!!!!!!";
//    Rigging = !Rigging;
//    qDebug() << "!!!!!! " << Rigging;
//}

QString DerivableVectorMatrixes::ToString(const Matrix<Derivable, 1, 3> vec)
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

Matrix<Derivable, 1, 3> DerivableVectorMatrixes::SetDerive3DVector(const QVector3D qv3)
{
    return Matrix<Derivable,1,3>(qv3.x(), qv3.y(), qv3.z());
}

Eigen::Matrix<Derivable, 4, 4> DerivableVectorMatrixes::MakeDeriveScaleMatrix(const Derivable scale)
{
    Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    for (int i = 0; i < 3; i++)
        M(i,i) = scale;
    M(3,3) = Derivable(1, /*scale.getProiz()*/0);
    return M;
}
Eigen::Matrix<Derivable, 3, 3> DerivableVectorMatrixes::MakeDeriveScale3Matrix(const Derivable scale)
{
    Matrix<Derivable, 3, 3> M = Matrix<Derivable, 3, 3> ();
    for (int i = 0; i < 3; i++)
        M(i,i) = scale;
    return M;
}

void DerivableVectorMatrixes::ScaleDeriveMatrix(Eigen::Matrix<Derivable, 4, 4> &originalMatrix, const Derivable scale)
{
    originalMatrix = originalMatrix * MakeDeriveScaleMatrix(scale);
}
