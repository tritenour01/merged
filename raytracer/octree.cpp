#include "octree.h"
#include "triangle.h"
#include "raytracer.h"

Octree::Octree(int data, int depth)
{
    maxData = data;
    maxDepth = depth;
    root = NULL;
}

Octree::~Octree(void)
{
    deleteTree(root);
    delete root;
    root = NULL;
}

void Octree::deleteTree(Node* n)
{
    if(n->children){
        for(int i = 0; i < 8; i++)
            deleteTree(&n->children[i]);
        delete[] n->children;
    }
}

void Octree::computeNum(Node* n)
{
    if(n->children == NULL){
        n->num = n->data.size();
        return;
    }

    n->num = 0;
    for(int i = 0; i < 8; i++){
        computeNum(&n->children[i]);
        n->num += n->children[i].num;
    }
}

void Octree::createTree(vector<Triangle*>* newData)
{
    computeBounds(*newData, minBound, maxBound);

    root = new Node();
    root->children = NULL;
    root->parent = NULL;

    insertTriangles(*newData, minBound, maxBound, root, 1);
    computeNum(root);

    Log::writeLine("Mesh Bounds:");
    Log::writeLine("  " + minBound.toString());
    Log::writeLine("  " + maxBound.toString());
}

void Octree::insertTriangles(vector<Triangle*>& triangles, Vector3 minCorner, Vector3 maxCorner, Node* current, int depth)
{
    if(depth >= maxDepth || triangles.size() <= maxData){
        for(int i = 0; i < triangles.size(); i++)
            current->data.push_back(triangles[i]);
        return;
    }

    current->children = new Node[8];
    for(int i = 0; i < 8; i++){
        current->children[i].children = NULL;
        current->children[i].parent = current;
    }

    Vector3 midPoint = (minCorner + maxCorner) * 0.5;

    float bounds[15][3] = {{minCorner[0], minCorner[1], minCorner[2]}, {midPoint[0], minCorner[1], minCorner[2]}, {minCorner[0], minCorner[1], midPoint[2]},
                            {midPoint[0], minCorner[1], midPoint[2]}, {minCorner[0], midPoint[1], minCorner[2]}, {midPoint[0], midPoint[1], minCorner[2]},
                            {minCorner[0], midPoint[1], midPoint[2]}, {midPoint[0], midPoint[1], midPoint[2]}, {maxCorner[0], midPoint[1], midPoint[2]},
                            {midPoint[0], midPoint[1], maxCorner[2]}, {maxCorner[0], midPoint[1], maxCorner[2]}, {midPoint[0], maxCorner[1], midPoint[2]},
                            {maxCorner[0], maxCorner[1], midPoint[2]}, {midPoint[0], maxCorner[1], maxCorner[2]}, {maxCorner[0], maxCorner[1], maxCorner[2]}};

    Vector3 points[8][8];
    for(int i = 0; i < 8; i++)
        computePoints(points[i], bounds[i], bounds[i + 7]);

    vector<Triangle*> groups[8];
    vector<Triangle*>::iterator it;
    for(it = triangles.begin(); it != triangles.end(); it++){
        for(int j = 0; j < 8; j++){
            if(triangleAABBIntersect(*it, points[j]))
                groups[j].push_back(*it);
        }
    }

    for(int i = 0; i < 8; i++)
        insertTriangles(groups[i], Vector3(bounds[i][0], bounds[i][1], bounds[i][2]), Vector3(bounds[i + 7][0], bounds[i + 7][1], bounds[i + 7][2]), &current->children[i], depth + 1);
}

bool Octree::intersectRay(Ray& ray, Hitpoint& h)
{
    char vmask = 0;

    BoundInfo b;

    if(ray.dir.x > 0){
        b.Min[0] = (minBound.x - ray.origin.x) / ray.dir.x;
        b.Max[0] = (maxBound.x - ray.origin.x) / ray.dir.x;
    }
    else if(ray.dir.x < 0){
        vmask |= 1;
        b.Min[0] = (maxBound.x - ray.origin.x) / ray.dir.x;
        b.Max[0] = (minBound.x - ray.origin.x) / ray.dir.x;
    }
    else{
        b.Min[0] = -DBL_MAX;
        b.Max[0] = DBL_MAX;
    }

    if(ray.dir.y >= 0){
        b.Min[1] = (minBound.y - ray.origin.y) / ray.dir.y;
        b.Max[1] = (maxBound.y - ray.origin.y) / ray.dir.y;
    }
    else{
        vmask |= 4;
        b.Min[1] = (maxBound.y - ray.origin.y) / ray.dir.y;
        b.Max[1] = (minBound.y - ray.origin.y) / ray.dir.y;
    }

    if(ray.dir.z > 0){
        b.Min[2] = (minBound.z - ray.origin.z) / ray.dir.z;
        b.Max[2] = (maxBound.z - ray.origin.z) / ray.dir.z;
    }
    else if(ray.dir.z < 0){
        vmask |= 2;
        b.Min[2] = (maxBound.z - ray.origin.z) / ray.dir.z;
        b.Max[2] = (minBound.z - ray.origin.z) / ray.dir.z;
    }
    else{
        b.Min[2] = -DBL_MAX;
        b.Max[2] = DBL_MAX;
    }


    b.lmax = max(max(b.Min[0], b.Min[1]), b.Min[2]);
    b.umin = min(min(b.Max[0], b.Max[1]), b.Max[2]);
    if(b.lmax < b.umin){
        h.t = DBL_MAX;

        return intersectSubTrees(ray, h, root, b, vmask);
    }
    return false;
}

void Octree::computeIntersectRange(char mask, BoundInfo& b, float* Min, float* Mid, float* Max)
{
    if(mask & 1){
        b.Min[0] = Mid[0];
        b.Max[0] = Max[0];
    }
    else{
        b.Min[0] = Min[0];
        b.Max[0] = Mid[0];
    }

    if(mask & 4){
        b.Min[1] = Mid[1];
        b.Max[1] = Max[1];
    }
    else{
        b.Min[1] = Min[1];
        b.Max[1] = Mid[1];
    }

    if(mask & 2){
        b.Min[2] = Mid[2];
        b.Max[2] = Max[2];
    }
    else{
        b.Min[2] = Min[2];
        b.Max[2] = Mid[2];
    }

    b.lmax = max(max(b.Min[0], b.Min[1]), b.Min[2]);
    if(b.lmax < 0.0f)
        b.lmax = 0.0f;

    b.umin = min(min(b.Max[0], b.Max[1]), b.Max[2]);
}

bool Octree::intersectSubTrees(Ray& ray, Hitpoint& Hit, Node* current, BoundInfo& b, char vmask)
{
    if(current->children == NULL){
        bool intersect = false;
        Hitpoint h;
        for(int i = 0; i < current->data.size(); i++){
            if(current->data[i]->intersectRay(ray, h)){
                intersect = true;
                if(h.t > Ray::SMALL && h.t < Hit.t){
                    Hit.t = h.t;
                    Hit.s = current->data[i];
                    Hit.f1 = h.f1;
                    Hit.f2 = h.f2;
                }
            }
        }
        return intersect;
    }

    float midX = (b.Min[0] + b.Max[0]) / 2.0;
    float midY = (b.Min[1] + b.Max[1]) / 2.0;
    float midZ = (b.Min[2] + b.Max[2]) / 2.0;
    float Mid[3] = {midX, midZ, midY};

    char maskList[3];
    if(midX < midZ){
        maskList[0] = 1;
        maskList[1] = 2;
    }
    else{
        maskList[0] = 2;
        maskList[1] = 1;
    }

    if(midY < Mid[maskList[0] - 1]){
        char tmp = maskList[1];
        maskList[1] = maskList[0];
        maskList[2] = tmp;
        maskList[0] = 4;
    }
    else if(midY < Mid[maskList[1] - 1]){
        maskList[2] = maskList[1];
        maskList[1] = 4;
    }
    else
        maskList[2] = 4;

    Mid[1] = midY;
    Mid[2] = midZ;

    char cmask = 0;
    if(midX < b.lmax)
        cmask |= 1;
    if(midY < b.lmax)
        cmask |= 4;
    if(midZ < b.lmax)
        cmask |= 2;

    char lmask = 0;
    if(midX < b.umin)
        lmask |= 1;
    if(midY < b.umin)
        lmask |= 4;
    if(midZ < b.umin)
        lmask |= 2;

    bool intersect = false;
    int i = 0;
    do{
        if(current->children[cmask ^ vmask].num > 0){
            BoundInfo childBound;
            computeIntersectRange(cmask, childBound, b.Min, Mid, b.Max);

            if(childBound.lmax < childBound.umin){
                if(intersectSubTrees(ray, Hit, &current->children[cmask ^ vmask], childBound, vmask)){
                    intersect = true;
                    if(Hit.t < childBound.umin)
                        break;
                }
            }
        }

        if(cmask == lmask || i > 2)
            break;

        char old = cmask;
        do{
            cmask |= maskList[i];
            i++;
        }while(cmask == old && i <= 2);
    }while(true);

    return intersect;
}

void Octree::computeBounds(vector<Triangle*> triangles, Vector3& minBound, Vector3& maxBound)
{
    if(triangles.size() == 0)
        return;

    Vector3* p1 = triangles[0]->p1;
    Vector3* p2 = triangles[0]->p1;
    Vector3* p3 = triangles[0]->p1;

    minBound.x = triangles[0]->p1->x;
    minBound.y = triangles[0]->p1->y;
    minBound.z = triangles[0]->p1->z;

    maxBound.x = triangles[0]->p1->x;
    maxBound.y = triangles[0]->p1->y;
    maxBound.z = triangles[0]->p1->z;

    for(int i = 0; i < triangles.size(); i++){
        p1 = triangles[i]->p1;
        p2 = triangles[i]->p2;
        p3 = triangles[i]->p3;
        minBound.x = min(min(min(minBound.x, p1->x), p2->x), p3->x);
        maxBound.x = max(max(max(maxBound.x, p1->x), p2->x), p3->x);

        minBound.y = min(min(min(minBound.y, p1->y), p2->y), p3->y);
        maxBound.y = max(max(max(maxBound.y, p1->y), p2->y), p3->y);

        minBound.z = min(min(min(minBound.z, p1->z), p2->z), p3->z);
        maxBound.z = max(max(max(maxBound.z, p1->z), p2->z), p3->z);
    }

    minBound -= Vector3(0.01, 0.01, 0.01);
    maxBound += Vector3(0.01, 0.01, 0.01);
}

void Octree::computePoints(Vector3* points, float* minCorner, float* maxCorner)
{
    points[0] = Vector3(minCorner[0], minCorner[1], minCorner[2]);
    points[1] = Vector3(minCorner[0], minCorner[1], maxCorner[2]);
    points[2] = Vector3(minCorner[0], maxCorner[1], minCorner[2]);
    points[3] = Vector3(minCorner[0], maxCorner[1], maxCorner[2]);
    points[4] = Vector3(maxCorner[0], minCorner[1], minCorner[2]);
    points[5] = Vector3(maxCorner[0], minCorner[1], maxCorner[2]);
    points[6] = Vector3(maxCorner[0], maxCorner[1], minCorner[2]);
    points[7] = Vector3(maxCorner[0], maxCorner[1], maxCorner[2]);
}

bool Octree::triangleAABBIntersect(Triangle* triangle, Vector3* points)
{
    Vector3& p1 = *triangle->p1;
    Vector3& p2 = *triangle->p2;
    Vector3& p3 = *triangle->p3;
    if(pointInAABB(p1, points[0], points[7]) || pointInAABB(p2, points[0], points[7]) || pointInAABB(p3, points[0], points[7]))
        return true;

    Vector3 axis[3] = {Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1)};
    float t1, t2, s1, s2;
    for(int i = 0; i < 3; i++){
        projectTriangle(triangle, axis[i], t1, t2);
        projectAABB(i, points, axis[i], s1, s2);
        if(!intervalsOverlap(t1, t2, s1, s2)){
            return false;
        }
    }

    Vector3 n = triangle->getPlanarNormal();
    projectTriangle(triangle, n, t1, t2);
    projectAABB(3, points, n, s1, s2);
    if(!intervalsOverlap(t1, t2, s1, s2))
        return false;

    Vector3 edges[3] = {p2 - p1, p3 - p1, p3 - p2};
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            Vector3 newAxis = Vector3::CrossProduct(axis[i], edges[j]);
            projectTriangle(triangle, newAxis, t1, t2);
            projectAABB(3, points, newAxis, s1, s2);
            if(!intervalsOverlap(t1, t2, s1, s2))
                return false;
        }
    }

    return true;
}

void Octree::projectTriangle(Triangle* triangle, Vector3& axis, float& t1, float& t2)
{
    Vector3& p1 = *triangle->p1;
    Vector3& p2 = *triangle->p2;
    Vector3& p3 = *triangle->p3;
    t1 = Vector3::DotProduct(axis, p1);
    t2 = t1;
    float nextT = Vector3::DotProduct(axis, p2);
    t1 = min(t1, nextT);
    t2 = max(t2, nextT);
    nextT = Vector3::DotProduct(axis, p3);
    t1 = min(t1, nextT);
    t2 = max(t2, nextT);
}

void Octree::projectAABB(int type, Vector3* points, Vector3& axis, float& t1, float& t2)
{
    if(type == 0){
        t1 = points[0].x;
        t2 = points[7].x;
        return;
    }
    else if(type == 1){
        t1 = points[0].y;
        t2 = points[7].y;
        return;
    }
    else if(type == 2){
        t1 = points[0].z;
        t2 = points[7].z;
        return;
    }
    t1 = Vector3::DotProduct(points[0], axis);
    t2 = t1;
    float nextT;
    for(int i = 1; i < 8; i++){
        nextT = Vector3::DotProduct(points[i], axis);
        t1 = min(t1, nextT);
        t2 = max(t2, nextT);
    }
}

bool Octree::intervalsOverlap(float t1, float t2, float s1, float s2)
{
    if(t2 < s1 || s2 < t1)
        return false;
    return true;
}

bool Octree::pointInAABB(Vector3& point, Vector3& minCorner, Vector3& maxCorner)
{
    if(point.x < minCorner.x || point.x > maxCorner.x ||
       point.y < minCorner.y || point.y > maxCorner.y ||
       point.z < minCorner.z || point.z > maxCorner.z)
        return false;
    return true;
}

Vector3 Octree::getNormal(Ray& ray)
{
    return ray.cacheShape->computeNormal(ray);
}

void Octree::getUV(Vector3& p, Ray& ray, float& u, float& v)
{
    ray.cacheShape->getUV(p, ray, u, v);
}
