#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include "vector.h"

class Raytracer;
struct Ray;

class Light
{
    public:

        Light(Raytracer*, Vector3, Vector3, Vector3, float);
        virtual ~Light(){}

        Vector3 getPos(void);
        Vector3 getColor(void);
        float getIntensity(void);
        float getAttenuation(float);

        virtual Vector3 illuminate(Ray&, Vector3&, Vector3&) =0;

    protected:

        Vector3 position;
        Vector3 lightColor;
        Vector3 falloff;
        float intensity;

        Raytracer* raytracer;
};

class PointLight : public Light
{
    public:

        PointLight(Raytracer*, Vector3, Vector3, Vector3, float);

        Vector3 illuminate(Ray&, Vector3&, Vector3&);
};

class DirectionalLight : public Light
{
    public:

        DirectionalLight(Raytracer*, Vector3, Vector3, float);

        Vector3 illuminate(Ray&, Vector3&, Vector3&);

    private:

        Vector3 direction;
};

class Spotlight : public Light
{
    public:

        Spotlight(Raytracer*, Vector3, Vector3, Vector3, Vector3, float, float, float);

        Vector3 illuminate(Ray&, Vector3&, Vector3&);

    private:

        Vector3 lookat;
        float inner;
        float outer;
};

class AreaLight : public Light
{
    public:

        AreaLight(Raytracer*, Vector3, Vector3, Vector3, Vector3, Vector3, float, float, float);

        Vector3 illuminate(Ray&, Vector3&, Vector3&);

    private:

        Vector3 right;
        Vector3 up;

        float samplesx;
        float samplesy;
};

#endif // LIGHT_H_INCLUDED
