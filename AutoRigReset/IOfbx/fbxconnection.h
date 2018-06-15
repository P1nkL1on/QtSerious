#ifndef IOFBX_FBXCONNECTION_H
#define IOFBX_FBXCONNECTION_H

#include "fbxnode.h"

namespace IOfbx{

enum class ConnectionType
{
    None = -1,
    BoneToBone = 0,
    ModelToSubDeformer = 1,
    SubDeformerToDeformer = 2,
    DeformderToGeometry = 3,
    GeometryToMesh = 4,
    Other = 100
};

class FbxConnection : public FbxNode
{
public:
    FbxConnection() = default;
    QString parse(const QStringList &buffer) override;
    QString getIdLeft() const;
    QString getIdRight() const;
    ConnectionType getType() const;
    bool setIds (const QString &left, const QString &right);
    void setType(const ConnectionType &value);
private:
    QString idLeft;
    QString idRight;
    ConnectionType type = ConnectionType::None;
};

}

#endif // FBXCONNECTION_H
