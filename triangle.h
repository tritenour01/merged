#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

#include "vector.h"
#include "shape.h"
#include "ray.h"

class Triangle : public Shape
{
    public:

        Triangle(Vector3*, Vector3*, Vector3*, bool);

        bool Intersection(Ray&, float&);
        Vector3 getNormal(Vector3&);

        void setUV(Vector3, Vector3, Vector3);
        void getUV(Vector3&, float&, float&);

        Vector3 getPlanarNormal(void);

        void setNormals(Vector3, Vector3, Vector3);

        Vector3* p1;
        Vector3* p2;
        Vector3* p3;

    private:

        Vector3 n;

        Vector3 N1;
        Vector3 N2;
        Vector3 N3;

        float dist;

        Vector3 tex1;
        Vector3 tex2;
        Vector3 tex3;

        float u, v;

        bool destroy;
        bool smooth;
};

#endif // TRIANGLE_H_INCLUDED
