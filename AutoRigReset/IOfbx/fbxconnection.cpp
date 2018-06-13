#include "fbxconnection.h"

QString IOfbx::FbxConnection::parse(QStringList S, const int param)
{
    if (S.length() != 2)
        return "Incomplete connection type!";
    if (S[0].indexOf(',') < 0 || S[1].indexOf(',') < 0)
        return "Not pairable connection! (Required: a,b)";
    QStringList names = S[0].split(',');
    QStringList ids = S[1].split(',');
    if (names.length() != 2 || ids.length() < 3){
        for (int i = 0; i < S.length(); i++)
            traceMessage ( S[i]);
        return "Unusuall connection format!";
    }
    idLeft = ids[1];
    idRight = ids[2];
    type = ConnectionType::Other;
    if (names[0].indexOf("Model::") >= 0){
        if (names[1].indexOf("Model::") >= 0)
            type = ConnectionType::BoneToBone;
        if (names[1].indexOf("SubDeformer::") >= 0)
            type = ConnectionType::ModelToSubDeformer;
    }
    return QString();
}

QString IOfbx::FbxConnection::getIdLeft() const
{
    return idLeft;
}

QString IOfbx::FbxConnection::getIdRight() const
{
    return idRight;
}

IOfbx::ConnectionType IOfbx::FbxConnection::getType() const
{
    return type;
}


bool IOfbx::FbxConnection::setIds(const QString left, const QString right)
{
    idLeft = left;
    idRight = right;
    return true;
}

void IOfbx::FbxConnection::setType(const IOfbx::ConnectionType &value)
{
    type = value;
}

IOfbx::FbxConnection::FbxConnection()
{
    id = ""; name = "";
    idLeft = "";
    idRight = "";
    type = ConnectionType::None;
}
