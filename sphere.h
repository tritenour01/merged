#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED

#include <iostream>
#include "vector.h"
#include "shape.h"

using namespace std;

class Sphere : public Shape
{
    public:

        Sphere(Vector3, float);

        bool Intersection(Ray&, float&);

        Vector3 getNormal(Vector3&);

        void getUV(Vector3&, float&, float&);

    private:

        Vector3 center;
        float radius;
};

#endif // SPHERE_H_INCLUDED
