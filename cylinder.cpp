#include "cylinder.h"
#include "raytracer.h"

Cylinder::Cylinder(Vector3 newBase, float newHeight, float newRadius)
{
    base = newBase;
    height = newHeight;
    radius = newRadius;
}

bool Cylinder::Intersection(Ray& ray, Hitpoint& h)
{
    Vector3 o = ray.origin;
    Vector3 d = ray.dir;
    float dX = o.x - base.x;
    float dZ = o.z - base.z;
    float A = (d.x * d.x) + (d.z * d.z);
    float B = (2.0f * dX * d.x) + (2.0f * dZ * d.z);
    float C = (dX * dX) + (dZ * dZ) - (radius * radius);

    float disc = (B * B) - (4.0f * A * C);
    if(disc < 0.0f)
        return false;

    float sqrtDisc = sqrt(disc);
    float c1 = (-B + sqrtDisc) / (2.0f * A);
    float c2 = (-B - sqrtDisc) / (2.0f * A);

    float minT, maxT;
    if(c1 <= c2){
        minT = c1;
        maxT = c2;
    }
    else{
        minT = c2;
        maxT = c1;
    }

    h.f1 = (float)SIDE;

    float minY, maxY;
    if(d.y <= 0){
        minY = (base.y + height - o.y) / d.y;

        if(minY > maxT)
            return false;

        if(minY > minT){
            minT = minY;
            h.f1 = (float)TOP;
        }

        maxY = (base.y - o.y) / d.y;

        if(maxY < minT)
            return false;
    }
    else{
        minY = (base.y - o.y) / d.y;

        if(minY > maxT)
            return false;

        if(minY > minT){
            minT = minY;
            h.f1 = (float)BOTTOM;
        }

        maxY = (base.y + height - o.y) / d.y;

        if(maxY < minT)
            return false;
    }

    h.t = minT;
    return true;
}

Vector3 Cylinder::getNormal(Ray& ray)
{
    Vector3 result;
    if((PART_ID)ray.cacheFloat1 == TOP)
        result = Vector3(0.0, 1.0, 0.0);
    else if((PART_ID)ray.cacheFloat1 == BOTTOM)
        result = Vector3(0.0, -1.0, 0.0);

    if(isTransformed && (PART_ID)ray.cacheFloat1 == SIDE){
        Vector3 newPoint;
        Matrix4x4::transformPoint(invTrans, newPoint, ray.point);
        result = newPoint - base;
        result.y = 0.0f;
    }
    else if((PART_ID)ray.cacheFloat1 == SIDE){
        result = ray.point - base;
        result.y = 0.0f;
    }
    return result;
}

void Cylinder::getUV(Vector3& point, Ray& ray, float& u, float& v)
{
    if((PART_ID)ray.cacheFloat1 == SIDE){
        Vector3 n = point - base;
        v = n.y / height;
        n.normalize();
        u = 0.5f + (atan2(n.z, n.x) / (2.0f * 3.141592653f));
    }
    else{
        Vector3 n = point - base;
        u = 0.5f + (n.x / (2 * radius));
        v = 0.5f + (n.z / (2 * radius));
    }
}
