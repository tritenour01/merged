#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

#include "vector.h"
#include "matrix.h"
#include "ray.h"
#include "material.h"

struct Hitpoint;

class Shape
{
    public:

        Shape(void);

        //viretual functions for all objects
        virtual bool Intersection(Ray&, float&) =0;
        virtual Vector3 getNormal(Vector3&) =0;

        bool intersectRay(Ray&, Hitpoint&);

        Material& getMaterial(void);

        void Translate(Vector3&);
        void Rotate(Vector3&);
        void Scale(Vector3&);

    private:

        enum Transform {TRANS, SCALE, ROT};

        void applyTransform(Transform, Vector3&);

        Material material;

    protected:

        bool isTransformed;
        Matrix4x4 trans;
        Matrix4x4 invTrans;
        Matrix4x4 normalTrans;
};

#endif // SHAPE_H_INCLUDED
