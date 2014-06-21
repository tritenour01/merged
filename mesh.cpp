#include "mesh.h"

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
        float tempT;
        bool hit = false;
        for(int i = 0; i < triangles->size(); i++){
            if(triangles->at(i)->Intersection(ray, tempT)){
                if(tempT < minT && tempT >= Ray::SMALL){
                    minT = tempT;
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
    if(useOctree){
        if(isTransformed){
            Vector3 normal = data->getNormal(p);
            Vector3 newNormal;
            Matrix4x4::transformDirection(normalTrans, newNormal, normal);
            return newNormal;
        }
        return data->getNormal(p);
    }
    return triangles->at(intersectIndex)->getNormal(p);
}

void Mesh::getUV(Vector3& point, float& u, float& v)
{
    if(useOctree)
        data->getUV(point, u, v);
}
