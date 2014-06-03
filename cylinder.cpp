#include "cylinder.h"
#include "raytracer.h"

Cylinder::Cylinder(Vector3 newBase, float newHeight, float newRadius)
{
    base = newBase;
    height = newHeight;
    radius = newRadius;
}

bool Cylinder::Intersection(Ray& ray, float& t)
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

    part = SIDE;

    float minY, maxY;
    if(d.y <= 0){
        minY = (base.y + height - o.y) / d.y;

        if(minY > maxT)
            return false;

        if(minY > minT){
            minT = minY;
            part = TOP;
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
            part = BOTTOM;
        }

        maxY = (base.y + height - o.y) / d.y;

        if(maxY < minT)
            return false;
    }

    t = minT;
    return true;
}

Vector3 Cylinder::getNormal(Vector3& r)
{
    Vector3 result;
    if(part == TOP)
        result = Vector3(0.0, 1.0, 0.0);
    else if(part == BOTTOM)
        result = Vector3(0.0, -1.0, 0.0);

    if(isTransformed){
        if(part == SIDE){
            Vector3 newPoint;
            Matrix4x4::transformPoint(invTrans, newPoint, r);
            result = newPoint - base;
            result.y = 0.0f;
        }
        Vector3 newNormal;
        Matrix4x4::transformDirection(normalTrans, newNormal, result);
        return newNormal;
    }
    else{
        if(part == SIDE){
            result = r - base;
            result.y = 0.0f;
        }
    }

    return result;
}
