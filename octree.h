#ifndef OCTREE_H_INCLUDED
#define OCTREE_H_INCLUDED

#include <vector>
#include "vector.h"
#include "ray.h"
#include "box.h"
#include "debug.h"

class Triangle;

struct AABB
{
    Box* bounds;
    Vector3 points[8];
};

struct Node
{
    Node* parent;
    Node* children;
    AABB aabb;
    vector<Triangle*> data;
    int num;
};

class Octree
{
    public:

        Octree(int, int);
        void createTree(vector<Triangle*>*);
        bool intersectRay(Ray&, float&);
        Vector3 getNormal(Vector3&);
        void getUV(Vector3&, float&, float&);

    private:

        bool triangleAABBIntersect(Triangle*, AABB&);
        void projectTriangle(Triangle*, Vector3&, float&, float&);
        void projectAABB(int, AABB&, Vector3&, float&, float&);
        bool intervalsOverlap(float, float, float, float);
        bool pointInAABB(Vector3&, AABB&);

        void insertTriangle(Triangle*, Node*, int);
        void createNodes(Node*);

        void computeBounds(vector<Triangle*>, Vector3&, Vector3&);
        void computePoints(AABB&);

        bool intersectSubTrees(Ray&, float&, Node*, float*, float*, char);

        bool drawTree(Ray&, float&, Node*, int, float*, float*, char);

        Node* root;
        int maxData;
        int maxDepth;

        Shape* intersectObj;

        const int d = 10;
};

#endif // OCTREE_H_INCLUDED
