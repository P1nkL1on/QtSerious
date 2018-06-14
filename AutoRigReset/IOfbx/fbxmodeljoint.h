#ifndef IOFBX_FBXMODELJOINT_H
#define IOFBX_FBXMODELJOINT_H

#include "fbxnode.h"


namespace IOfbx{

class FbxModelJoint : public FbxNode
{
public:
    FbxModelJoint() = default;
    void addSubName (const QString &subName);
    QString parse(const QStringList &buffer) override;
    const QVector<double> &getLocalTranslation() const;
    const QVector<double> &getLocalRotation() const;
    const QVector<double> &getLocalScaling() const;

private:
    QString subName;
    QVector<double> localTranslation;
    QVector<double> localRotation;
    QVector<double> localScaling;
};

}

#endif // FBXMODELJOINT_H
