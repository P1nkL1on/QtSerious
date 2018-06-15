#ifndef FBXTORIGCONVERTER_H
#define FBXTORIGCONVERTER_H

#include "fbxparsedcontainer.h"
#include "DerStruct/defines.h"
#include "FbxStruct/skeleton.h"
#include "FbxStruct/mesh.h"
#include "FbxStruct/cluster.h"
#include <QPainter>

namespace IOfbx{

class FbxConverter
{
public:
    static void convertContainerToRig(const FbxParsedContainer *container, QPainter *qp);
private:
    static FbxStruct::Joint convertJoint(const FbxModelJoint &parsedJoint);
    static FbxStruct::Cluster convertCluster(const FbxSubDeformerCluster &parsedCluster);
    static FbxStruct::Mesh convertMesh(const FbxGeometryMesh &parsedMesh);
};

}
#endif // FBXTORIGCONVERTER_H
