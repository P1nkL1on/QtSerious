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
    static FbxStruct::Joint convertJoint(const FbxModelJoint &parsedJoint);
};

}
#endif // FBXTORIGCONVERTER_H
