#ifndef FBXSTRUCT_FBXCONVERTER_H
#define FBXSTRUCT_FBXCONVERTER_H

#include "FbxStruct/skeleton.h"
#include "IOfbx/fbxparsedcontainer.h"

namespace FbxStruct{

class FbxConverter
{
public:
     void convertContainerToRig(const IOfbx::FbxParsedContainer *container);
};

}
#endif // FBXCONVERTER_H
