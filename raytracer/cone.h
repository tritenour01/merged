#ifndef CONE_H_INCLUDED
#define CONE_H_INCLUDED

#include "vector.h"
#include "shape.h"

class Cone : public Shape
{
    public:

        Cone(Vector3, float, float);

        void getUV(Vector3&, Ray&, float&, float&);

    private:

        bool Intersection(Ray&, Hitpoint&);
        Vector3 getNormal(Ray&);

        enum Sides {SIDE, BOTTOM};

        Vector3 base;
        float radius;
        float height;
};

#endif // CONE_H_INCLUDED
