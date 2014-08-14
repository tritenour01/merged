#include "box.h"
#include "raytracer.h"

Box::Box(Vector3 bound1, Vector3 bound2)
{
    //sets the bounds of the AABB
    minCorner = bound1;
    maxCorner = bound2;
}

bool Box::Intersection(Ray& ray, Hitpoint& h)
{
    //min and max times in overlap interval
    float tMin = 0.0f;
    float tMax = FLT_MAX;

    //intersection point and direction
    Vector3 P = ray.origin;
    Vector3 D = ray.dir;

    float xMin, xMax, yMin, yMax, zMin, zMax;

    //check the left and right side first
    if(D.x >= 0.0){
        xMin = (minCorner.x - P.x) / D.x;

        //xMin out of intersection interval
        if(xMin > tMax)
            return false;

        //check and set new interval minimum
        if(xMin > tMin){
            tMin = xMin;
            h.f1 = (float)LEFT;
        }

        xMax = (maxCorner.x - P.x) / D.x;

        //xMax out of intersection interval
        if(xMax < tMin)
            return false;

        //check and set new interval maximum
        if(xMax < tMax)
            tMax = xMax;
    }
    else{
        xMin = (maxCorner.x - P.x) / D.x;

        //xMin out of intersection interval
        if(xMin > tMax)
            return false;

        //check and set new interval minimum
        if(xMin > tMin){
            tMin = xMin;
            h.f1 = (float)RIGHT;
        }

        xMax = (minCorner.x - P.x) / D.x;

        //xMax out of intersection interval
        if(xMax < tMin)
            return false;

        //check and set new interval maximum
        if(xMax < tMax)
            tMax = xMax;
    }

    //check the top and bottom
    if(D.y >= 0.0f){
        yMin = (minCorner.y - P.y) / D.y;

        //yMin out of intersection interval
        if(yMin > tMax)
            return false;

        //check and set new interval minimum
        if(yMin > tMin){
            tMin = yMin;
            h.f1 = (float)BOTTOM;
        }

        yMax = (maxCorner.y - P.y) / D.y;

        //yMax out of intersection interval
        if(yMax < tMin)
            return false;

        //check and set new interval maximum
        if(yMax < tMax)
            tMax = yMax;
    }
    else{
        yMin = (maxCorner.y - P.y) / D.y;

        //yMin out of intersection interval
        if(yMin > tMax)
            return false;

        //check and set new interval minimum
        if(yMin > tMin){
            tMin = yMin;
            h.f1 = (float)TOP;
        }

        yMax = (minCorner.y - P.y) / D.y;

        //yMax out of intersection interval
        if(yMax < tMin)
            return false;

        //check and set new interval maximum
        if(yMax < tMax)
            tMax = yMax;
    }

    //check the front and back
    if(D.z >= 0.0f){
        zMin = (minCorner.z - P.z) / D.z;

        //zMin out of intersection interval
        if(zMin > tMax)
            return false;

        //check and set new interval minimum
        if(zMin > tMin){
            tMin = zMin;
            h.f1 = (float)FRONT;
        }

        zMax = (maxCorner.z - P.z) / D.z;

        //zMax out of intersection interval
        if(zMax < tMin)
            return false;

        //check and set new interval maximum
        if(zMax < tMax)
            tMax = zMax;
    }
    else{
        zMin = (maxCorner.z - P.z) / D.z;

        //zMin out of intersection interval
        if(zMin > tMax)
            return false;

        //check and set new interval minimum
        if(zMin > tMin){
            tMin = zMin;
            h.f1 = (float)BACK;
        }

        zMax = (minCorner.z - P.z) / D.z;

        //zMax out of intersection interval
        if(zMax < tMin)
            return false;

        //check and set new interval maximum
        if(zMax < tMax)
            tMax = zMax;
    }

    h.t = tMin;
    return true;
}

Vector3 Box::getNormal(Ray& ray)
{
    //determine the normal based on the intersection side
    switch((SIDE_ID)ray.cacheFloat1){
        case LEFT:
            return Vector3(-1.0f, 0.0f, 0.0f);
        case RIGHT:
            return Vector3(1.0f, 0.0f, 0.0f);
        case TOP:
            return Vector3(0.0f, 1.0f, 0.0f);
        case BOTTOM:
            return Vector3(0.0f, -1.0f, 0.0f);
        case FRONT:
            return Vector3(0.0f, 0.0f, -1.0f);
        case BACK:
            return Vector3(0.0f, 0.0f, 1.0f);
    }
}

void Box::getUV(Vector3& point, Ray& ray, float& u, float& v)
{
    switch((SIDE_ID)ray.cacheFloat1){
        case LEFT:
            u = (maxCorner.z - point.z) / (maxCorner.z - minCorner.z);
            v = (point.y - minCorner.y) / (maxCorner.y - minCorner.y);
            break;
        case RIGHT:
            u = (point.z - minCorner.z) / (maxCorner.z - minCorner.z);
            v = (point.y - minCorner.y) / (maxCorner.y - minCorner.y);
            break;
        case TOP:
            u = (point.x - minCorner.x) / (maxCorner.x - minCorner.x);
            v = (point.z - minCorner.z) / (maxCorner.z - minCorner.z);
            break;
        case BOTTOM:
            u = (point.x - minCorner.x) / (maxCorner.x - minCorner.x);
            v = (maxCorner.z - point.z) / (maxCorner.z - minCorner.z);
            break;
        case FRONT:
            u = (point.x - minCorner.x) / (maxCorner.x - minCorner.x);
            v = (point.y - minCorner.y) / (maxCorner.y - minCorner.y);
            break;
        case BACK:
            u = (maxCorner.x - point.x) / (maxCorner.x - minCorner.x);
            v = (point.y - minCorner.y) / (maxCorner.y - minCorner.y);
            break;
    }
}
