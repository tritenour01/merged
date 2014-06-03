#include "cone.h"

Cone::Cone(Vector3 newBase, float newHeight, float newRad)
{
    base = newBase;
    radius = newRad;
    height = newHeight;
}

bool Cone::Intersection(Ray& ray, float& t)
{
    Vector3 o = ray.origin;
    Vector3 d = ray.dir;

    float k = -pow(radius / height, 2.0f);

    float A = (d.x * d.x) + k * (d.y * d.y) + (d.z * d.z);
    float B = (2.0f * (o.x - base.x) * d.x) + k * (2.0f * (o.y - base.y - height) * d.y) + (2.0f * (o.z - base.z) * d.z);
    float C = pow(o.x - base.x, 2.0f) + k * pow(o.y - base.y - height, 2.0f) + pow(o.z - base.z, 2.0f);

    float disc = (B * B) - (4.0 * A * C);
    if(disc < 0.0f)
        return false;

    float sqrtDisc = sqrt(disc);

    float t1 = (-B + sqrtDisc) / (2.0f * A);
    float t2 = (-B - sqrtDisc) / (2.0f * A);

    float minT, maxT;
    if(t1 <= t2){
        minT = t1;
        maxT = t2;
    }
    else{
        minT = t2;
        maxT = t1;
    }

    part = SIDE;

    if(d.y <= 0.0f){
        float minY = (base.y + height - o.y) / d.y;

        if(minY > maxT)
            return false;

        float maxY = (base.y - o.y) / d.y;

        if(maxY < minT)
            return false;
    }
    else{
        float minY = (base.y - o.y) / d.y;

        if(minY > maxT)
            return false;

        if(minY > minT){
            minT = minY;
            part = BOTTOM;
        }

        float maxY = (base.y + height - o.y) / d.y;

        if(maxY < minT)
            return false;
    }

    t = minT;
    return true;
}

Vector3 Cone::getNormal(Vector3& p)
{
    if(part == SIDE){
        Vector3 toPoint = base + Vector3(0, height, 0) - p;
        return Vector3::CrossProduct(Vector3::CrossProduct(toPoint, Vector3(0, 1, 0)), toPoint);
    }
    else
        return Vector3(0, -1, 0);
}
