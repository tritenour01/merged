#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include "vector.h"
#include "texture.h"

class Shape;
class Ray;

class Material
{
    public:

        Material(Shape*);

        //setters for the material
        void setDiffuse(Vector3);
        void setDiffuseFactor(float);
        void setSpecular(Vector3);
        void setSpecularFactor(float);
        void setShineness(float);
        void setReflective(float);
        void setRefraction(float);
        void setTexture(Texture*);
        void setBumpMap(Texture*);
        void setNormalMap(Texture*);
        void setEmissive(Vector3);

        //getters for the material
        Vector3 getDiffuse(Ray&);
        float getDiffuseFactor(void);
        Vector3 getSpecular(void);
        float getSpecularFactor(void);
        float getShineness(void);
        float getReflective(void);
        float getRefraction(void);
        bool normalsAltered(void);
        Vector3 getNormal(Ray&);
        bool isEmissive(void);
        Vector3 getEmissiveColor(void);

    private:

        Vector3 diffuseColor;
        float diffuseFactor;
        Vector3 specularColor;
        float specularFactor;
        float shineness;
        float reflect;
        float refract;
        Vector3 transparentColor;
        float transparentFactor;
        Texture* texture;
        Texture* bumpMap;
        Texture* normalMap;
        Vector3 emissiveColor;
        bool emissive;

        Shape* owner;
};

#endif // MATERIAL_H_INCLUDED
