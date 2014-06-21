#include "box.h"

Box::Box(Vector3 bound1, Vector3 bound2)
{
    //sets the bounds of the AABB
    minCorner = bound1;
    maxCorner = bound2;
}

bool Box::Intersection(Ray& ray, float& t)
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
            side = LEFT;
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
            side = RIGHT;
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
            side = BOTTOM;
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
            side = TOP;
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
            side = FRONT;
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
            side = BACK;
        }

        zMax = (minCorner.z - P.z) / D.z;

        //zMax out of intersection interval
        if(zMax < tMin)
            return false;

        //check and set new interval maximum
        if(zMax < tMax)
            tMax = zMax;
    }

    t = tMin;
    return true;
}

Vector3 Box::getNormal(Vector3& p)
{
    Vector3 result;

    //determine the normal based on the intersection side
    switch(side){
        case LEFT:
            result = Vector3(-1.0f, 0.0f, 0.0f);
            break;
        case RIGHT:
            result = Vector3(1.0f, 0.0f, 0.0f);
            break;
        case TOP:
            result = Vector3(0.0f, 1.0f, 0.0f);
            break;
        case BOTTOM:
            result = Vector3(0.0f, -1.0f, 0.0f);
            break;
        case FRONT:
            result = Vector3(0.0f, 0.0f, -1.0f);
            break;
        case BACK:
            result = Vector3(0.0f, 0.0f, 1.0f);
            break;
    }

    if(isTransformed){
        Vector3 transformNormal;
        Matrix4x4::transformDirection(normalTrans, transformNormal, result);
        return transformNormal;
    }

    return result;
}

void Box::getUV(Vector3& point, float& u, float& v)
{
    switch(side){
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

    //cout<<u<<endl;
}
