#include "derivable.h"
#include "qmath.h"

using namespace DerOperations;

//QVector<Derivable> Derivable::AutoDiff(QVector<QPair<Derivable, Derivable>> orig,
//                                              QVector<QPair<Derivable, Derivable>> final, QVector3D transform)
//{
//    QVector<Derivable> res = {Derivable(), Derivable(), Derivable()};

//    for (int coord = 0 ; coord < 3; coord++){
//        QVector<Derivable> derTransform = {Derivable(transform.x()), Derivable(transform.y()), Derivable(transform.z())};
//        derTransform[coord].setPrValue(1);  // make a variable x y l
//        res[coord] = CurDist(orig, final, derTransform);
//    }
//    return res;
//}

DerOperations::Derivable DerOperations::cos(const DerOperations::Derivable f) {
    return DerOperations::Derivable(cos(f.getValue()), -sin(f.getValue())*f.getProiz());
}
DerOperations::Derivable DerOperations::sin(const DerOperations::Derivable f) {
    return DerOperations::Derivable(sin(f.getValue()), cos(f.getValue())*f.getProiz());
}
DerOperations::Derivable DerOperations::pow (const DerOperations::Derivable f, const unsigned int power){
    if (power == 1) return f;
    else return DerOperations::Derivable( pow(f.getValue(),power), power * f.getProiz() * pow(f.getValue(), power - 1) );
}
double DerOperations::cos(const double f) {
    return QMATH_H::cos(f);
}
double DerOperations::sin(const double f) {
    return QMATH_H::sin(f);
}
double DerOperations::pow (const double f,const unsigned int power){
    return QMATH_H::pow(f,power);
}
