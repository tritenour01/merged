#ifndef BOX_H_INCLUDED
#define BOX_H_INCLUDED

#include "vector.h"
#include "shape.h"
#include "float.h"

class Box : public Shape
{
    public:

        Box(Vector3, Vector3);

        void getUV(Vector3&, Ray&, float&, float&);

        Vector3 minCorner;
        Vector3 maxCorner;

    private:

        bool Intersection(Ray&, Hitpoint&);
        Vector3 getNormal(Ray&);

        enum SIDE_ID {LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK};
};

#endif // BOX_H_INCLUDED
