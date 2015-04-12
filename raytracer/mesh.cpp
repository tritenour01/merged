#include "mesh.h"
#include "raytracer.h"

Mesh::Mesh(std::vector<Triangle*>* t, std::vector<Vector3>* p, std::vector<Vector3>* n)
{
    triangles = t;
    points = p;
    normals = n;

    if(useOctree){
        data = new Octree(10, 10);
        data->createTree(t);
    }
}

Mesh::~Mesh(void)
{
    delete data;
    delete points;
    delete normals;

    for(int i = 0; i < triangles->size(); i++)
        delete triangles->at(i);
    delete triangles;
}

bool Mesh::Intersection(Ray& ray, Hitpoint& hit)
{
    if(useOctree){
        return data->intersectRay(ray, hit);
    }
    else{
        float minT = FLT_MAX;
        Hitpoint h;
        bool ret = false;
        for(int i = 0; i < triangles->size(); i++){
            if(triangles->at(i)->intersectRay(ray, h)){
                if(h.t < minT && h.t >= Ray::SMALL){
                    minT = h.t;
                    intersectIndex = i;
                }
                ret = true;
            }
        }
        hit.t = minT;
        return ret;
    }
}

Vector3 Mesh::getNormal(Ray& p)
{
    if(useOctree)
        return data->getNormal(p);
    return triangles->at(intersectIndex)->computeNormal(p);
}

void Mesh::getUV(Vector3& point, Ray& ray, float& u, float& v)
{
    if(useOctree)
        data->getUV(point, ray, u, v);
}
