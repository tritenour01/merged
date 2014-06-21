#include "octree.h"
#include "triangle.h"
#include "raytracer.h"

Octree::Octree(int data, int depth)
{
    maxData = data;
    maxDepth = depth;
    root = NULL;
}

int depth(Node* d)
{
    if(d->children == NULL)
        return 1;

    int maxVal = 0;
    for(int i = 0; i < 8; i++){
        maxVal = max(maxVal, depth(&d->children[i]));
    }

    return maxVal + 1;
}

void compute(Node* d)
{
    if(d->children == NULL){
        d->num = d->data.size();
        return;
    }

    d->num = 0;
    for(int i = 0; i < 8; i++){
        compute(&d->children[i]);
        d->num += d->children[i].num;
    }
}

void Octree::createTree(vector<Triangle*>* newData)
{
    AABB aabb;
    Vector3 minBound;
    Vector3 maxBound;
    computeBounds(*newData, minBound, maxBound);
    aabb.bounds = new Box(minBound, maxBound);
    computePoints(aabb);

    root = new Node;
    root->children = NULL;
    root->parent = NULL;
    root->aabb = aabb;

    for(int i = 0; i < newData->size(); i++)
        insertTriangle(newData->at(i), root, 1);

    compute(root);
}

void Octree::insertTriangle(Triangle* triangle, Node* current, int depth)
{
    if(current->children != NULL){
        for(int i = 0; i < 8; i++){
            if(triangleAABBIntersect(triangle, current->children[i].aabb))
                insertTriangle(triangle, &current->children[i], depth + 1);
        }
    }
    else{
        current->data.push_back(triangle);
        if(current->data.size() > maxData && depth < maxDepth){
            createNodes(current);
            int num = current->data.size();
            for(int j = 0; j < num; j++){
                Triangle* t = current->data.back();
                current->data.pop_back();
                for(int i = 0; i < 8; i++){
                    if(triangleAABBIntersect(t, current->children[i].aabb))
                        insertTriangle(t, &current->children[i], depth + 1);
                }
            }
        }
    }
}

void Octree::createNodes(Node* current)
{
    current->children = new Node[8];
    for(int i = 0; i < 8; i++){
        current->children[i].children = NULL;
        current->children[i].parent = current;
    }

    Vector3& minBound = current->aabb.bounds->minCorner;
    Vector3& maxBound = current->aabb.bounds->maxCorner;

    Vector3 midPoint = (minBound + maxBound) * 0.5;

    current->children[0].aabb.bounds = new Box(minBound, midPoint);

    current->children[1].aabb.bounds = new Box(Vector3(midPoint.x, minBound.y, minBound.z),
                                                Vector3(maxBound.x, midPoint.y, midPoint.z));

    current->children[2].aabb.bounds = new Box(Vector3(minBound.x, minBound.y, midPoint.z),
                                                Vector3(midPoint.x, midPoint.y, maxBound.z));

    current->children[3].aabb.bounds = new Box(Vector3(midPoint.x, minBound.y, midPoint.z),
                                                  Vector3(maxBound.x, midPoint.y, maxBound.z));

    current->children[4].aabb.bounds = new Box (Vector3(minBound.x, midPoint.y, minBound.z),
                                                 Vector3(midPoint.x, maxBound.y, midPoint.z));

    current->children[4].aabb.bounds = new Box(Vector3(minBound.x, midPoint.y, minBound.z),
                                                Vector3(midPoint.x, maxBound.y, midPoint.z));

    current->children[5].aabb.bounds = new Box(Vector3(midPoint.x, midPoint.y, minBound.z),
                                                Vector3(maxBound.x, maxBound.y, midPoint.z));

    current->children[6].aabb.bounds = new Box(Vector3(minBound.x, midPoint.y, midPoint.z),
                                                Vector3(midPoint.x, maxBound.y, maxBound.z));

    current->children[7].aabb.bounds = new Box(midPoint, maxBound);

    for(int i = 0; i < 8; i++)
        computePoints(current->children[i].aabb);
}

bool Octree::intersectRay(Ray& ray, float& t)
{
    Vector3& minBound = root->aabb.bounds->minCorner;
    Vector3& maxBound = root->aabb.bounds->maxCorner;

    char mask = 0;

    float x1, x2, y1, y2, z1, z2;
    if(ray.dir.x > 0){
        x1 = (minBound.x - ray.origin.x) / ray.dir.x;
        x2 = (maxBound.x - ray.origin.x) / ray.dir.x;
    }
    else if(ray.dir.x < 0){
        mask |= 1;
        x1 = (maxBound.x - ray.origin.x) / ray.dir.x;
        x2 = (minBound.x - ray.origin.x) / ray.dir.x;
    }
    else{
        x1 = -DBL_MAX;
        x2 = DBL_MAX;
    }

    if(ray.dir.y >= 0){
        y1 = (minBound.y - ray.origin.y) / ray.dir.y;
        y2 = (maxBound.y - ray.origin.y) / ray.dir.y;
    }
    else{
        mask |= 4;
        y1 = (maxBound.y - ray.origin.y) / ray.dir.y;
        y2 = (minBound.y - ray.origin.y) / ray.dir.y;
    }

    if(ray.dir.z > 0){
        z1 = (minBound.z - ray.origin.z) / ray.dir.z;
        z2 = (maxBound.z - ray.origin.z) / ray.dir.z;
    }
    else if(ray.dir.z < 0){
        mask |= 2;
        z1 = (maxBound.z - ray.origin.z) / ray.dir.z;
        z2 = (minBound.z - ray.origin.z) / ray.dir.z;
    }
    else{
        z1 = -DBL_MAX;
        z2 = DBL_MAX;
    }

    if(max(max(x1, y1), z1) < min(min(x2, y2), z2)){
        t = DBL_MAX;
        float Min[3] = {x1, y1, z1};
        float Max[3] = {x2, y2, z2};

        return intersectSubTrees(ray, t, root, Min, Max, mask);
        //return drawTree(ray, t, root, 0, Min, Max, mask);
    }
    return false;
}

inline bool overlap(float* Min, float* Max)
{
    return max(max(max(Min[0], Min[1]), Min[2]), 0.0f) < min(min(Max[0], Max[1]), Max[2]);
}

bool Octree::drawTree(Ray& ray, float& t, Node* current, int depth, float* Min, float* Max, char mask)
{
    if(current->num == 0 || !overlap(Min, Max))
        return false;
    if(depth >= d || current->children == NULL){
        bool intersect = false;
        float testT;
        if(current->num > 0 && current->aabb.bounds->Intersection(ray, testT)){
            intersect = true;
            if(testT > Ray::SMALL && testT < t){
                t = testT;
                intersectObj = current->aabb.bounds;
            }
        }
        return intersect;
    }

    float midX = (Min[0] + Max[0]) / 2.0;
    float midY = (Min[1] + Max[1]) / 2.0;
    float midZ = (Min[2] + Max[2]) / 2.0;
    float Mid[3] = {midX, midY, midZ};

    float otherBounds[12][3] = {{Mid[0], Min[1], Min[2]}, {Min[0], Min[1], Mid[2]}, {Mid[0], Min[1], Mid[2]},
                                {Min[0], Mid[1], Min[2]}, {Mid[0], Mid[1], Min[2]}, {Min[0], Mid[1], Mid[2]},
                                {Max[0], Mid[1], Mid[2]}, {Mid[0], Mid[1], Max[2]}, {Max[0], Mid[1], Max[2]},
                                {Mid[0], Max[1], Mid[2]}, {Max[0], Max[1], Mid[2]}, {Mid[0], Max[1], Max[2]}};

    bool intersect = false;
    drawTree(ray, t, &current->children[mask], depth + 1, Min, Mid, mask) ? intersect = true : intersect;
    drawTree(ray, t, &current->children[mask ^ 1], depth + 1, otherBounds[0], otherBounds[6], mask) ? intersect = true : intersect;
    drawTree(ray, t, &current->children[mask ^ 2], depth + 1, otherBounds[1], otherBounds[7], mask) ? intersect = true : intersect;
    drawTree(ray, t, &current->children[mask ^ 3], depth + 1, otherBounds[2], otherBounds[8], mask) ? intersect = true : intersect;
    drawTree(ray, t, &current->children[mask ^ 4], depth + 1, otherBounds[3], otherBounds[9], mask) ? intersect = true : intersect;
    drawTree(ray, t, &current->children[mask ^ 5], depth + 1, otherBounds[4], otherBounds[10], mask) ? intersect = true : intersect;
    drawTree(ray, t, &current->children[mask ^ 6], depth + 1, otherBounds[5], otherBounds[11], mask) ? intersect = true : intersect;
    drawTree(ray, t, &current->children[mask ^ 7], depth + 1, Mid, Max, mask) ? intersect = true : intersect;

    return intersect;
}

bool Octree::intersectSubTrees(Ray& ray, float& t, Node* current, float* Min, float* Max, char mask)
{
    if(current->children == NULL){
        bool intersect = false;
        float testT;
        for(int i = 0; i < current->data.size(); i++){
            if(current->data[i]->Intersection(ray, testT)){
                intersect = true;
                if(testT > Ray::SMALL && testT < t){
                    t = testT;
                    intersectObj = current->data[i];
                }
            }
        }
        return intersect;
    }

    float midX = 0.0f;
    if(Min[0] > -DBL_MAX)
        midX = (Min[0] + Max[0]) / 2.0;
    else{
        if(ray.origin.x < (current->aabb.bounds->minCorner.x + current->aabb.bounds->maxCorner.x) / 2.0f)
            midX = DBL_MAX;
        else
            midX = -DBL_MAX;
    }
    float midY = (Min[1] + Max[1]) / 2.0;
    float midZ = 0.0f;
    if(Min[2] > -DBL_MAX)
        midZ = (Min[2] + Max[2]) / 2.0;
    else{
        if(ray.origin.x < (current->aabb.bounds->minCorner.z + current->aabb.bounds->maxCorner.z) / 2.0f)
            midZ = DBL_MAX;
        else
            midZ = -DBL_MAX;
    }
    float Mid[3] = {midX, midY, midZ};

    float otherBounds[12][3] = {{Mid[0], Min[1], Min[2]}, {Min[0], Min[1], Mid[2]}, {Mid[0], Min[1], Mid[2]},
                                {Min[0], Mid[1], Min[2]}, {Mid[0], Mid[1], Min[2]}, {Min[0], Mid[1], Mid[2]},
                                {Max[0], Mid[1], Mid[2]}, {Mid[0], Mid[1], Max[2]}, {Max[0], Mid[1], Max[2]},
                                {Mid[0], Max[1], Mid[2]}, {Max[0], Max[1], Mid[2]}, {Mid[0], Max[1], Max[2]}};

    int numHit = 0;
    int ID[8];
    float value[8];
    if(current->children[mask].num > 0 && overlap(Min, Mid)){
        ID[0] = 0;
        value[0] = max(max(max(Min[0], Min[1]), Min[2]), 0.0f);
        numHit = 1;
    }
    for(int i = 0; i < 6; i++){
        if(current->children[mask ^ (i + 1)].num > 0 && overlap(otherBounds[i], otherBounds[6 + i])){
            float val =  max(max(max(otherBounds[i][0], otherBounds[i][1]), otherBounds[i][2]), 0.0f);
            int j;
            for(j = 0; j < numHit && val > value[j]; j++);
            int newPos = j;
            for(j = numHit; j > newPos; j--){
                ID[j] = ID[j - 1];
                value[j] = value[j - 1];
            }
            ID[newPos] = i + 1;
            value[newPos] = val;
            numHit++;
        }
    }
    if(current->children[mask ^ 7].num > 0 && overlap(Mid, Max)){
        float val =  max(max(max(Mid[0], Mid[1]), Mid[2]), 0.0f);
        int j;
        for(j = 0; j < numHit && val > value[j]; j++);
        int newPos = j;
        for(j = numHit; j > newPos; j--){
            ID[j] = ID[j - 1];
            value[j] = value[j - 1];
        }
        ID[newPos] = 7;
        value[newPos] = val;
        numHit++;
    }

    bool intersect = false;
    bool temp;
    for(int i = 0; i < numHit; i++)
    {
        if(ID[i] == 0)
            temp = intersectSubTrees(ray, t, &current->children[mask], Min, Mid, mask);
        else if(ID[i] == 7)
            temp = intersectSubTrees(ray, t, &current->children[mask ^ 7], Mid, Max, mask);
        else
            temp = intersectSubTrees(ray, t, &current->children[mask ^ ID[i]], otherBounds[ID[i] - 1], otherBounds[ID[i] + 5], mask);

        if(temp){
            intersect = true;
            if(i < numHit - 1 && t < value[i + 1])
                break;
        }
    }
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

    cout<<minBound<<maxBound;
}

void Octree::computePoints(AABB& aabb)
{
    Vector3& minBound = aabb.bounds->minCorner;
    Vector3& maxBound = aabb.bounds->maxCorner;
    aabb.points[0] = minBound;
    aabb.points[1] = Vector3(minBound.x, minBound.y, maxBound.z);
    aabb.points[2] = Vector3(minBound.x, maxBound.y, minBound.z);
    aabb.points[3] = Vector3(minBound.x, maxBound.y, maxBound.z);
    aabb.points[4] = Vector3(maxBound.x, minBound.y, minBound.z);
    aabb.points[5] = Vector3(maxBound.x, minBound.y, maxBound.z);
    aabb.points[6] = Vector3(maxBound.x, maxBound.y, minBound.z);
    aabb.points[7] = maxBound;
}

bool Octree::triangleAABBIntersect(Triangle* triangle, AABB& aabb)
{
    Vector3& p1 = *triangle->p1;
    Vector3& p2 = *triangle->p2;
    Vector3& p3 = *triangle->p3;
    if(pointInAABB(p1, aabb) || pointInAABB(p2, aabb) || pointInAABB(p3, aabb))
        return true;

    Vector3 axis[3] = {Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1)};
    float t1, t2, s1, s2;
    for(int i = 0; i < 3; i++){
        projectTriangle(triangle, axis[i], t1, t2);
        projectAABB(i, aabb, axis[i], s1, s2);
        if(!intervalsOverlap(t1, t2, s1, s2)){
            return false;
        }
    }

    Vector3 n = triangle->getPlanarNormal();
    projectTriangle(triangle, n, t1, t2);
    projectAABB(3, aabb, n, s1, s2);
    if(!intervalsOverlap(t1, t2, s1, s2))
        return false;

    Vector3 edges[3] = {p2 - p1, p3 - p1, p3 - p2};
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            Vector3 newAxis = Vector3::CrossProduct(axis[i], edges[j]);
            projectTriangle(triangle, newAxis, t1, t2);
            projectAABB(3, aabb, newAxis, s1, s2);
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

void Octree::projectAABB(int type, AABB& aabb, Vector3& axis, float& t1, float& t2)
{
    if(type == 0){
        t1 = aabb.points[0].x;
        t2 = aabb.points[7].x;
        return;
    }
    else if(type == 1){
        t1 = aabb.points[0].y;
        t2 = aabb.points[7].y;
        return;
    }
    else if(type == 2){
        t1 = aabb.points[0].z;
        t2 = aabb.points[7].z;
        return;
    }
    t1 = Vector3::DotProduct(aabb.points[0], axis);
    t2 = t1;
    float nextT;
    for(int i = 1; i < 8; i++){
        nextT = Vector3::DotProduct(aabb.points[i], axis);
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

bool Octree::pointInAABB(Vector3& point, AABB& aabb)
{
    Vector3& minBound = aabb.bounds->minCorner;
    Vector3& maxBound = aabb.bounds->maxCorner;
    if(point.x < minBound.x || point.x > maxBound.x ||
       point.y < minBound.y || point.y > maxBound.y ||
       point.z < minBound.z || point.z > maxBound.z)
        return false;
    return true;
}

Vector3 Octree::getNormal(Vector3& p)
{
    return intersectObj->getNormal(p);
}

void Octree::getUV(Vector3& p, float& u, float& v)
{
    intersectObj->getUV(p, u, v);
}
