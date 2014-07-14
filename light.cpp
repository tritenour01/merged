#include "light.h"

Light::Light(Vector3 p, Vector3 c, Vector3 f, float i)
{
    position = p;
    color = c;
    falloff = f;
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

float Light::getAttenuation(float dist)
{
    float poly = falloff.x + falloff.y * dist + falloff.z * dist * dist;
    return min(1.0f / poly, 1.0f);
}
