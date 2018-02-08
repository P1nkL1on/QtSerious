#include "lightsourse.h"
#include "qvector3d.h"

LightSourse::LightSourse()
{

}

LightSourse::LightSourse(QVector3D cent, float maxLig, float maxDist)
{
    center = cent;
    maxLight = maxLig;
    maxDistance = maxDist;
}

float LightSourse::lightPower(float onDistance) const
{
    return (onDistance > maxDistance)? 0 : maxLight * (onDistance / maxDistance);
}
