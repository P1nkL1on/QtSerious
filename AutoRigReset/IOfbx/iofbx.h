#ifndef IOFBX_IOFBX_H
#define IOFBX_IOFBX_H

#include "fbxparsedcontainer.h"

namespace IOfbx {


enum class ParseType
{
    None = -1,
    FbxGeometryMeshVertices = 0,
    FbxGeometryMeshPolygonIndexes = 1,
    FbxObjectModelLimbNodeProperty = 2,
    FbxObjectPoseNodeID = 3,
    FbxObjectPoseNodeMatrix = 4,
    FbxObjectDeformerCluster = 5,
    FbxConnection = 6,
};

ParseType pushHeader(const QString fromLine, QVector<QString> &stackHeaders);
int indexOfHeaders (const QStringList &tags, QVector<QString> &stackHeaders);

// the method of loading
FbxParsedContainer *loadFromPath(const QString &path, QString &error);
void findIdAndNameInLine(const QString line, QString& id, QString& name, QString &subName);

}

#endif // IOFBX_H
