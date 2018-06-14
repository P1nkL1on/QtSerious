#ifndef IOFBX_FBXMESSAGES_H
#define IOFBX_FBXMESSAGES_H
#include <QString>
namespace IOfbx {
    const QString errMessageIncorrectFilePath = "The path of readable file is incorrect : ";
    const QString errMessageFileEmpty       = "The file is empty : ";
    const QString errMessageMeshVert        = "x   Error in parsing a mesh vertices: ";
    const QString errMessageMeshPolygons    = "x   Error in parsing a mesh polygon idnexes: ";
    const QString errMessageJoint           = "x   Error in parsing a joint: ";
    const QString errMessagePoseNode        = "x   Error in parsing a posenode: ";
    const QString errMessageCluster         = "x   Error in parsing a subdeformer cluster: ";
    const QString errMessageConnection      = "x   Error in parsing connection: ";
    const QString errMessageOnlyMesh        = "x   File contains only mesh!";
    const QString errMessageNoRigs          = "x   File does not contain any rigs!";
}
#endif // FBXMESSAGES_H
