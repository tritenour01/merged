#include "material.h"

Material::Material(void)
{
    diffuseColor = Vector3(0.0f, 0.0f, 0.0f);
    diffuseFactor = 0.0f;
    specularColor = Vector3(0.0f, 0.0f, 0.0f);
    shineness = 1.0f;
    reflect = 0.0f;
    refract = 0.0f;
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

Vector3 Material::getDiffuse(void)
{
    return diffuseColor;
}

float Material::getDiffuseFactor(void)
{
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
