#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include "vector.h"

class Light
{
    public:

        Light(Vector3, Vector3, Vector3, float);

        Vector3 getPos(void);
        Vector3 getColor(void);
        float getIntensity(void);
        float getAttenuation(float);

    private:

        Vector3 position;
        Vector3 color;
        Vector3 falloff;
        float intensity;
};

#endif // LIGHT_H_INCLUDED
