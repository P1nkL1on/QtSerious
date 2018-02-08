#ifndef LOADERFBX_H
#define LOADERFBX_H

#include <iostream>
#include <QTextStream>
#include <rig.h>

namespace loaderFBX
{
    QString loadModelFBXAdress (QString path, Rig& loadedRig);
    QString loadModelFBX (QTextStream& textStream, Rig& loadedRig);
    QString loadMeshOBJAdress (QString path, Mesh& loadedMesh);
    QString loadMeshOBJ (QTextStream& textStream, Mesh& loadedMesh);
}

#endif // LOADERFBX_H
