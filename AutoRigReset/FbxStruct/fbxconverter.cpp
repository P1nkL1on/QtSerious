#include "fbxconverter.h"

using namespace FbxStruct;

void FbxStruct::FbxConverter::convertContainerToRig(const IOfbx::FbxParsedContainer *container)
{
    QVector<Joint> fbxJoints;
    for (const auto parsedJoint : container->getJoints())
        fbxJoints << Joint(parsedJoint);

    return;
}
