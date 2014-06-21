#include "material.h"
#include "shape.h"

Material::Material(Shape* o) : owner(o)
{
    diffuseColor = Vector3(0.0f, 0.0f, 0.0f);
    diffuseFactor = 0.0f;
    specularColor = Vector3(0.0f, 0.0f, 0.0f);
    shineness = 1.0f;
    reflect = 0.0f;
    refract = 0.0f;
    texture = NULL;
}

void Material::setDiffuse(Vector3 d)
{
    diffuseColor = d;
}

void Material::setDiffuseFactor(float d)
{
    diffuseFactor = d;
}

void Material::setSpecular(Vector3 s)
{
    specularColor = s;
}

void Material::setShineness(float s)
{
    shineness = s;
}

void Material::setReflective(float r)
{
    reflect = r;
}

void Material::setRefraction(float r)
{
    refract = r;
}

void Material::setTexture(Texture* t)
{
    texture = t;
}

Vector3 Material::getDiffuse(Vector3& point)
{
    if(texture){
        float u = 0.0f, v = 0.0f;
        if(owner->transformed()){
            Vector3 newPoint;
            Matrix4x4 invTrans = owner->getInvTrans();
            Matrix4x4::transformPoint(invTrans, newPoint, point);
            owner->getUV(newPoint, u, v);
        }
        else
            owner->getUV(point, u, v);
        return texture->sampleTexture(u, v);
    }
    return diffuseColor;
}

float Material::getDiffuseFactor(void)
{
    if(texture)
        return 1.0f;
    return diffuseFactor;
}

Vector3 Material::getSpecular(void)
{
    return specularColor;
}

float Material::getShineness(void)
{
    return shineness;
}

float Material::getReflective(void)
{
    return reflect;
}

float Material::getRefraction(void)
{
    return refract;
}
