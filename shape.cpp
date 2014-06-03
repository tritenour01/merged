#include "shape.h"
#include "raytracer.h"

Shape::Shape(void) : isTransformed(false){}

bool Shape::intersectRay(Ray& ray, Hitpoint& hit)
{
    if(isTransformed){
        Ray newRay;
        Matrix4x4::transformPoint(invTrans, newRay.origin, ray.origin);
        Matrix4x4::transformDirection(invTrans, newRay.dir, ray.dir);
        if(this->Intersection(newRay, hit.t)){
            Vector3 point;
            point = newRay.origin + hit.t * newRay.dir;
            Matrix4x4::transformPoint(trans, hit.point, point);
            return true;
        }
        return false;
    }
    if(this->Intersection(ray, hit.t)){
        hit.point = ray.origin + hit.t * ray.dir;
        return true;
    }
    return false;
}

Material& Shape::getMaterial(void)
{
    return material;
}

void Shape::Translate(Vector3& v)
{
    applyTransform(TRANS, v);
}

void Shape::Rotate(Vector3& v)
{
    applyTransform(ROT, v);
}

void Shape::Scale(Vector3& v)
{
    applyTransform(SCALE, v);
}

void Shape::applyTransform(Transform t, Vector3& v)
{
    switch(t)
    {
        case TRANS:
            Matrix4x4::applyTranslation(trans, v);
            break;
        case ROT:
            Matrix4x4::applyRotation(trans, v);
            break;
        case SCALE:
            Matrix4x4::applyScale(trans, v);
            break;
    }
    invTrans = trans;
    invTrans.invert();
    normalTrans = invTrans;
    normalTrans.transpose();
    isTransformed = true;
}
