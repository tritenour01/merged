#include "plane.h"

//constructor for finite planes
Plane::Plane(Vector3 c, Vector3 r, float rBound, Vector3 u, float uBound)
{
    //set the plane normal and axis
    center = c;
    up = u;
    normal = Vector3::CrossProduct(u, r);
    right = Vector3::CrossProduct(normal, up);

    //normalize the normal and axis
    up.normalize();
    right.normalize();
    normal.normalize();

    //compute the distance value
    Vector3 x0 = center + up;
    d = -Vector3::DotProduct(normal, x0);

    //set the bounds
    upBound = uBound;
    rightBound = rBound;
}

//constructor for an infinite plane
Plane::Plane(Vector3 c, Vector3 r, Vector3 u)
{
    //set the plane normal and axis
    center = c;
    up = u;
    normal = Vector3::CrossProduct(u, r);
    right = Vector3::CrossProduct(normal, up);

    //normalize the normal and axis
    up.normalize();
    right.normalize();
    normal.normalize();

    //compute the distance value
    Vector3 x0 = center + up;
    d = -Vector3::DotProduct(normal, x0);

    //set the bounds for an infinite plane
    upBound = -1.0f;
    rightBound = -1.0f;
}

bool Plane::Intersection(Ray& r, float& t)
{
    //compute the intersection time
    float newT = -(Vector3::DotProduct(r.origin, normal) + d) / Vector3::DotProduct(r.dir, normal);

    //if the value is not infinte
    if(!isinf(newT)){
        //intersection point
        Vector3 P = r.origin + newT * r.dir;

        //vector to center of plane
        Vector3 vec = P - center;

        //right and up distance from center to intersection
        float rightDist = Vector3::DotProduct(vec, right);
        float upDist = Vector3::DotProduct(vec, up);

        //if its an infinite plane
        if(rightBound < 0 && upBound < 0){
            t = newT;
            return true;
        }
        //its a finite plane
        if(abs(rightDist) <= rightBound && abs(upDist) <= upBound){
            t = newT;
            return true;
        }
    }

    return false;
}

//return the plane's normal
Vector3 Plane::getNormal(Vector3& p)
{
    return normal;
}

void Plane::getUV(Vector3& point, float& u, float& v)
{
    u = Vector3::DotProduct(point, right);
    v = Vector3::DotProduct(point, up);
}
