#ifndef FBXTORIGCONVERTER_H
#define FBXTORIGCONVERTER_H

#include "fbxparsedcontainer.h"
#include "DerStruct/defines.h"
#include "FbxStruct/skeleton.h"



namespace IOfbx{

class FbxConverter
{
public:
    static void convertContainerToRig(const FbxParsedContainer *container);
private:
    static void convertJoint(const FbxModelJoint &parsedJoint, FbxStruct::Joint &fbxJoint);
};

}
#endif // FBXTORIGCONVERTER_H
