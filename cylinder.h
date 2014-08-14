#ifndef CYLINDER_H_INCLUDED
#define CYLINDER_H_INCLUDED

#include "shape.h"
#include "vector.h"
#include "ray.h"
#include "debug.h"

class Cylinder : public Shape
{
    public:

        Cylinder(Vector3, float, float);

        void getUV(Vector3&, Ray&, float&, float&);

    private:

        bool Intersection(Ray&, Hitpoint&);
        Vector3 getNormal(Ray&);

        enum PART_ID {SIDE, TOP, BOTTOM};

        Vector3 base;
        float height;
        float radius;
};

#endif // CYLINDER_H_INCLUDED
