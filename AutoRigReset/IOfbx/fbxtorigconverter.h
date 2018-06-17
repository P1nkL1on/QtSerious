#ifndef FBXTORIGCONVERTER_H
#define FBXTORIGCONVERTER_H

#include "fbxparsedcontainer.h"
#include "DerStruct/defines.h"
#include "FbxStruct/rig.h"

namespace IOfbx{

class FbxConverter
{
public:
    static FbxStruct::Rig *convertContainerToRig(const FbxParsedContainer *container);
private:
    static FbxStruct::Joint convertJoint(const FbxModelJoint &parsedJoint);
    static FbxStruct::Cluster convertCluster(const FbxSubDeformerCluster &parsedCluster);
    static FbxStruct::Mesh convertMesh(const FbxGeometryMesh &parsedMesh);
};

}
#endif // FBXTORIGCONVERTER_H
