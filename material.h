#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include "vector.h"
#include "texture.h"

class Shape;

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

        //getters for the material
        Vector3 getDiffuse(Vector3&);
        float getDiffuseFactor(void);
        Vector3 getSpecular(void);
        float getSpecularFactor(void);
        float getShineness(void);
        float getReflective(void);
        float getRefraction(void);
        bool normalsAltered(void);
        Vector3 getNormal(Vector3&);

    private:

        Vector3 diffuseColor;
        float diffuseFactor;
        Vector3 specularColor;
        float specularFactor;
        float shineness;
        float reflect;
        float refract;
        Texture* texture;
        Texture* bumpMap;
        Texture* normalMap;

        Shape* owner;
};

#endif // MATERIAL_H_INCLUDED
