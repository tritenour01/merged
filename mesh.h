#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <vector>
#include "shape.h"
#include "vector.h"
#include "triangle.h"
#include "octree.h"

class Mesh : public Shape
{
    public:

        Mesh(std::vector<Triangle*>*, std::vector<Vector3>*, std::vector<Vector3>*);

        void getUV(Vector3&, Ray&, float&, float&);

    private:

        bool Intersection(Ray&, Hitpoint&);
        Vector3 getNormal(Ray&);

        const bool useOctree = true;

        std::vector<Triangle*>* triangles;
        std::vector<Vector3>* points;
        std::vector<Vector3>* normals;
        int intersectIndex;

        Octree* data;
};

#endif // MESH_H_INCLUDED
