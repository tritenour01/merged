#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include <iostream>
#include "shape.h"
#include <cmath>

using namespace std;

class Plane : public Shape
{
    public:

        Plane(Vector3, Vector3, float, Vector3, float);
        Plane(Vector3, Vector3, Vector3);

        void getUV(Vector3&, Ray&, float&, float&);

    private:

        bool Intersection(Ray&, Hitpoint&);
        Vector3 getNormal(Ray&);

        Vector3 center;
        Vector3 up;
        Vector3 right;
        Vector3 normal;

        float d;
        float upBound;
        float rightBound;
};

#endif // PLANE_H_INCLUDED
