#include "material.h"
#include "shape.h"

Material::Material(Shape* o) : owner(o)
{
    diffuseColor = Vector3(0.0f, 0.0f, 0.0f);
    diffuseFactor = 0.0f;
    specularColor = Vector3(0.0f, 0.0f, 0.0f);
    shineness = 1.0f;
    reflect = 0.0f;
    reflectColor = Vector3(1.0f, 1.0f, 1.0f);
    glossiness = 0.0f;
    refract = 0.0f;
    IOR = 1.0f;
    texture = NULL;
    bumpMap = NULL;
    normalMap = NULL;
    emissive = false;
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

void Material::setSpecularFactor(float s)
{
    specularFactor = s;
}

void Material::setShineness(float s)
{
    shineness = s;
}

void Material::setReflective(float r)
{
    reflect = r;
}

void Material::setReflectColor(Vector3 c)
{
    reflectColor = c;
}

void Material::setGlossiness(float g)
{
    glossiness = g;
}

void Material::setRefraction(float r)
{
    refract = r;
}

void Material::setIOR(float ior)
{
    IOR = ior;
}

void Material::setTexture(Texture* t)
{
    texture = t;
}

void Material::setBumpMap(Texture* b)
{
    bumpMap = b;
}

void Material::setNormalMap(Texture* n)
{
    normalMap = n;
}

void Material::setEmissive(Vector3 c)
{
    emissiveColor = c;
    emissive = true;
}

Vector3 Material::getDiffuse(Ray& ray)
{
    if(texture){
        float u = 0.0f, v = 0.0f;
        if(owner->transformed()){
            Vector3 newPoint;
            Matrix4x4 invTrans = owner->getInvTrans();
            Matrix4x4::transformPoint(invTrans, newPoint, ray.point);
            owner->getUV(newPoint, ray, u, v);
        }
        else
            owner->getUV(ray.point, ray, u, v);
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

float Material::getSpecularFactor(void)
{
    return specularFactor;
}

float Material::getShineness(void)
{
    return shineness;
}

float Material::getReflective(void)
{
    return reflect;
}

Vector3 Material::getReflectColor(void)
{
    return reflectColor;
}

float Material::getGlossiness(void)
{
    return glossiness;
}

float Material::getRefraction(void)
{
    return refract;
}

float Material::getIOR(void)
{
    return IOR;
}

bool Material::normalsAltered(void)
{
    return (bumpMap != NULL || normalMap != NULL);
}

Vector3 Material::getNormal(Ray& ray)
{
    if(bumpMap){
        float u = 0.0f, v = 0.0f;
        if(owner->transformed()){
            Vector3 newPoint;
            Matrix4x4 invTrans = owner->getInvTrans();
            Matrix4x4::transformPoint(invTrans, newPoint, ray.point);
            owner->getUV(newPoint, ray, u, v);
        }
        else
            owner->getUV(ray.point, ray, u, v);
        return bumpMap->sampleBump(u, v);
    }
    if(normalMap){
        float u = 0.0f, v = 0.0f;
        if(owner->transformed()){
            Vector3 newPoint;
            Matrix4x4 invTrans = owner->getInvTrans();
            Matrix4x4::transformPoint(invTrans, newPoint, ray.point);
            owner->getUV(newPoint, ray, u, v);
        }
        else
            owner->getUV(ray.point, ray, u, v);
        return normalMap->sampleNormal(u, v);
    }
    return Vector3(0, 0, 0);
}

bool Material::isEmissive(void)
{
    return emissive;
}

Vector3 Material::getEmissiveColor(void)
{
    return emissiveColor;
}
