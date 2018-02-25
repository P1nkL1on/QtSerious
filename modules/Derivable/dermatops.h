#ifndef DERMATOPS_H
#define DERMATOPS_H


#include "QMatrix4x4"
#include "QDebug"
#include "Eigen/Core"
#include "QVector"
#include "Derivable.h"
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

#endif // DERMATOPS_H
