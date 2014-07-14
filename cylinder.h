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

        void getUV(Vector3&, float&, float&);

    private:

        bool Intersection(Ray&, float&);
        Vector3 getNormal(Vector3&);

        enum PART_ID {SIDE, TOP, BOTTOM};
        PART_ID part;

        Vector3 base;
        float height;
        float radius;
};

#endif // CYLINDER_H_INCLUDED
