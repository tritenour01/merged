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

        Mesh(std::vector<Triangle*>*, std::vector<Vector3>*);

        bool Intersection(Ray&, float&);
        Vector3 getNormal(Vector3&);

    private:

        const bool useOctree = true;

        std::vector<Triangle*>* triangles;
        std::vector<Vector3>* points;
        int intersectIndex;

        Octree* data;
};

#endif // MESH_H_INCLUDED
