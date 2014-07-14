#include "shape.h"
#include "raytracer.h"

Shape::Shape(void) : isTransformed(false), material(this){}

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

Vector3 Shape::computeNormal(Vector3& point)
{
    if(material.normalsAltered()){
        Vector3 bumpNormal = material.getNormal(point);
        bumpNormal.normalize();
        Vector3 objectNormal = this->getNormal(point);

        Vector3 tangent;
        if(fabs(objectNormal.x) >= Ray::SMALL || fabs(objectNormal.z) >= Ray::SMALL)
            tangent = Vector3::CrossProduct(objectNormal, Vector3(0, 1, 0));
        else
            tangent = Vector3(1, 0, 0);

        Vector3 bitangent = Vector3::CrossProduct(tangent, objectNormal);

        if(isTransformed){
            Vector3 temp = objectNormal;
            Matrix4x4::transformDirection(normalTrans, objectNormal, temp);
            temp = tangent;
            Matrix4x4::transformDirection(normalTrans, tangent, temp);
            temp = bitangent;
            Matrix4x4::transformDirection(normalTrans, bitangent, temp);
        }

        objectNormal.normalize();
        tangent.normalize();
        bitangent.normalize();

        Matrix4x4 tangentSpace;
        tangentSpace[0] = tangent.x;
        tangentSpace[1] = tangent.y;
        tangentSpace[2] = tangent.z;
        tangentSpace[4] = bitangent.x;
        tangentSpace[5] = bitangent.y;
        tangentSpace[6] = bitangent.z;
        tangentSpace[8] = objectNormal.x;
        tangentSpace[9] = objectNormal.y;
        tangentSpace[10] = objectNormal.z;

        Vector3 newNormal;
        Matrix4x4::transformDirection(tangentSpace, newNormal, bumpNormal);
        newNormal.normalize();
        return newNormal;
    }

    Vector3 n = this->getNormal(point);
    if(isTransformed){
        Vector3 transformNormal;
        Matrix4x4::transformDirection(normalTrans, transformNormal, n);
        transformNormal.normalize();
        return transformNormal;
    }
    n.normalize();
    return n;
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

bool Shape::transformed(void)
{
    return isTransformed;
}

Matrix4x4 Shape::getInvTrans(void)
{
    return invTrans;
}
