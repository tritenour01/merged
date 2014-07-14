#ifndef CONE_H_INCLUDED
#define CONE_H_INCLUDED

#include "vector.h"
#include "shape.h"

class Cone : public Shape
{
    public:

        Cone(Vector3, float, float);

        void getUV(Vector3&, float&, float&);

    private:

        bool Intersection(Ray&, float&);
        Vector3 getNormal(Vector3&);

        enum Sides {SIDE, BOTTOM};
        Sides part;

        Vector3 base;
        float radius;
        float height;
};

#endif // CONE_H_INCLUDED
