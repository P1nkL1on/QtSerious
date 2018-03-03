#ifndef LOADERFBX_H
#define LOADERFBX_H

#include <iostream>
#include <QTextStream>
#include <rig.h>

namespace loaderFBX
{
    static QVector3D meshOffset = QVector3D(-1,136,0);
    QString loadModelFBXAdress (QString path, Rig& loadedRig);
    QString loadModelFBX (QTextStream& textStream, Rig& loadedRig);
    QString loadMeshOBJAdress (QString path, Mesh& loadedMesh);
    QString loadMeshOBJ (QTextStream& textStream, Mesh& loadedMesh);
    QString saveModelFBX (QString path, Rig& savingRig);
}

#endif // LOADERFBX_H
