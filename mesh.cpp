#include "mesh.h"
#include "raytracer.h"

Mesh::Mesh(std::vector<Triangle*>* t, std::vector<Vector3>* p)
{
    triangles = t;
    points = p;

    if(useOctree){
        data = new Octree(10, 10);
        data->createTree(t);
    }
}

bool Mesh::Intersection(Ray& ray, float& t)
{
    if(useOctree){
        return data->intersectRay(ray, t);
    }
    else{
        float minT = FLT_MAX;
        Hitpoint h;
        bool hit = false;
        for(int i = 0; i < triangles->size(); i++){
            if(triangles->at(i)->intersectRay(ray, h)){
                if(h.t < minT && h.t >= Ray::SMALL){
                    minT = h.t;
                    intersectIndex = i;
                }
                hit = true;
            }
        }
        t = minT;
        return hit;
    }
}

Vector3 Mesh::getNormal(Vector3& p)
{
    if(useOctree)
        return data->getNormal(p);
    return triangles->at(intersectIndex)->computeNormal(p);
}

void Mesh::getUV(Vector3& point, float& u, float& v)
{
    if(useOctree)
        data->getUV(point, u, v);
}
