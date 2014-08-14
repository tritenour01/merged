#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED

#include <float.h>
#include "vector.h"

class Shape;

class Ray
{
    public:

        Ray(){}

        Ray(Vector3 o, Vector3 d)
        {
            origin = o;
            dir = d;
        }

        Vector3 origin;
        Vector3 dir;

        static constexpr float SMALL = 0.0005f;

        float t;
        Shape* s;
        Vector3 point;

        Shape* cacheShape;
        float cacheFloat1;
        float cacheFloat2;
};

#endif // RAY_H_INCLUDED
