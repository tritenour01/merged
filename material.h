#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include "vector.h"

struct Material
{
    public:

        Material(void);

        //setters for the material
        void setDiffuse(Vector3);
        void setDiffuseFactor(float);
        void setSpecular(Vector3);
        void setShineness(float);
        void setReflective(float);
        void setRefraction(float);

        //getters for the material
        Vector3 getDiffuse(void);
        float getDiffuseFactor(void);
        Vector3 getSpecular(void);
        float getShineness(void);
        float getReflective(void);
        float getRefraction(void);

    private:

        Vector3 diffuseColor;
        float diffuseFactor;
        Vector3 specularColor;
        float shineness;
        float reflect;
        float refract;
};

#endif // MATERIAL_H_INCLUDED
