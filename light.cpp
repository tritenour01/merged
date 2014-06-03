#include "light.h"

Light::Light(Vector3 p, Vector3 c, float i)
{
    position = p;
    color = c;
    intensity = i;
}

//returns the light postion
Vector3 Light::getPos(void)
{
    return position;
}

//returns the light color
Vector3 Light::getColor(void)
{
    return color;
}

//returns the light intensity
float Light::getIntensity(void)
{
    return intensity;
}
