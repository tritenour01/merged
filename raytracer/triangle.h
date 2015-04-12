#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

#include "vector.h"
#include "shape.h"
#include "ray.h"

class Triangle : public Shape
{
    public:

        Triangle(Vector3*, Vector3*, Vector3*, bool);

        void setUV(Vector3, Vector3, Vector3);
        void getUV(Vector3&, Ray&, float&, float&);

        Vector3 getPlanarNormal(void);

        void setNormals(Vector3*, Vector3*, Vector3*);

        Vector3* p1;
        Vector3* p2;
        Vector3* p3;

    private:

        bool Intersection(Ray&, Hitpoint&);
        Vector3 getNormal(Ray&);

        Vector3 n;

        Vector3* N1;
        Vector3* N2;
        Vector3* N3;

        float dist;

        Vector3 tex1;
        Vector3 tex2;
        Vector3 tex3;

        bool smooth;
};

#endif // TRIANGLE_H_INCLUDED
