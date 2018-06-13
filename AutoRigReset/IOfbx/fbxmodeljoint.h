#ifndef IOFBX_FBXMODELJOINT_H
#define IOFBX_FBXMODELJOINT_H

#include "fbxnode.h"


namespace IOfbx{

class FbxModelJoint : public FbxNode
{
public:
    FbxModelJoint ();
    void addSubName (const QString &subName);
    QString parse(QStringList S, const  int param) override;
private:
    QString subName;
    QVector<double> localTranslation;
    QVector<double> localRotation;
    QVector<double> localScaling;
};

}

#endif // FBXMODELJOINT_H
