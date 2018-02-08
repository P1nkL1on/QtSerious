#ifndef LIGHTSOURSE_H
#define LIGHTSOURSE_H

#include "qvector3d.h"

class LightSourse
{
private:
    float maxDistance;
    float maxLight;

public:
    QVector3D center;
    LightSourse();
    LightSourse(QVector3D cent, float maxLig, float maxDist);
    float lightPower (float onDistance) const;
};

#endif // LIGHTSOURSE_H
