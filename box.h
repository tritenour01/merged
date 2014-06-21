#ifndef BOX_H_INCLUDED
#define BOX_H_INCLUDED

#include "vector.h"
#include "shape.h"
#include "float.h"

class Box : public Shape
{
    public:

        Box(Vector3, Vector3);

        bool Intersection(Ray&, float&);
        Vector3 getNormal(Vector3&);

        void getUV(Vector3&, float&, float&);

        Vector3 minCorner;
        Vector3 maxCorner;

    private:

        enum SIDE_ID {LEFT, RIGHT, TOP, BOTTOM, FRONT, BACK};
        SIDE_ID side;
};

#endif // BOX_H_INCLUDED
