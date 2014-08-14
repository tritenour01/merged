#include "light.h"
#include "raytracer.h"

Light::Light(Raytracer* r, Vector3 p, Vector3 c, Vector3 f, float i)
{
    raytracer = r;
    position = p;
    lightColor = c;
    falloff = f;
    intensity = i;
}

//returns the light postion
Vector3 Light::getPos(void)
{
    return position;
}

//returns the light color
Vector3 Light::getColor(void)
{
    return lightColor;
}

//returns the light intensity
float Light::getIntensity(void)
{
    return intensity;
}

float Light::getAttenuation(float dist)
{
    float poly = falloff.x + falloff.y * dist + falloff.z * dist * dist;
    return min(1.0f / poly, 1.0f);
}

PointLight::PointLight(Raytracer* r, Vector3 p, Vector3 c, Vector3 f, float i)
 : Light(r, p, c, f, i) {}

Vector3 PointLight::illuminate(Ray& ray, Vector3& n, Vector3& diffuse)
{
    Ray shadow(ray.point, position - ray.point);
    float factor = raytracer->computeShadowFactor(shadow, 1.0f);
    if(factor > 0.0f){
        Vector3 l = shadow.dir;
        float atten = getAttenuation(l.getLength());
        l.normalize();

        Vector3 color = factor * atten * intensity * raytracer->calculateShading(ray, n, l, diffuse);
        color.x *= lightColor.x;
        color.y *= lightColor.y;
        color.z *= lightColor.z;
        return color;
    }

    return Vector3(0, 0, 0);
}

DirectionalLight::DirectionalLight(Raytracer* r, Vector3 d, Vector3 c, float i)
 : Light(r, Vector3(0, 0, 0), c, Vector3(0, 0, 0), i)
{
    direction = -d;
    direction.normalize();
}

Vector3 DirectionalLight::illuminate(Ray& ray, Vector3& n, Vector3& diffuse)
{
    Ray shadow(ray.point, direction);
    float factor = raytracer->computeShadowFactor(shadow, FLT_MAX);
    if(factor > 0.0f){
        Vector3 color = factor * intensity * raytracer->calculateShading(ray, n, direction, diffuse);
        color.x *= lightColor.x;
        color.y *= lightColor.y;
        color.z *= lightColor.z;
        return color;
    }
    return Vector3(0, 0, 0);
}

Spotlight::Spotlight(Raytracer* r, Vector3 p, Vector3 l, Vector3 c, Vector3 f, float i, float in, float out)
    : Light(r, p, c, f, i)
{
    lookat = p - l;
    lookat.normalize();

    inner = cosf((in * 3.1415926f) / 180.0f);
    outer = cosf(((in + out) * 3.1415926f) / 180.0f);
}

Vector3 Spotlight::illuminate(Ray& ray, Vector3& n, Vector3& diffuse)
{
    Ray shadow(ray.point, position - ray.point);
    float factor = raytracer->computeShadowFactor(shadow, 1.0f);
    if(factor > 0.0f){
        Vector3 l = shadow.dir;
        float atten = getAttenuation(l.getLength());
        l.normalize();

        float newIntensity;
        float diff = Vector3::DotProduct(lookat, l);

        if(diff >= inner)
            newIntensity = intensity;
        else if(diff > outer){
            float val = (1 - ((diff - inner) / (outer - inner)));
            newIntensity = intensity * val * val * (3.0f - 2.0f * val);
        }
        else
            return Vector3(0, 0, 0);

        Vector3 color = factor * atten * newIntensity * raytracer->calculateShading(ray, n, l, diffuse);
        color.x *= lightColor.x;
        color.y *= lightColor.y;
        color.z *= lightColor.z;
        return color;
    }

    return Vector3(0, 0, 0);
}

AreaLight::AreaLight(Raytracer* ray, Vector3 p, Vector3 r, Vector3 u, Vector3 c, Vector3 f, float i, float sx, float sy)
    : Light(ray, p, c, f, i)
{
    right = r;
    up = u;
    samplesx = sx;
    samplesy = sy;
}

Vector3 AreaLight::illuminate(Ray& ray, Vector3& n, Vector3& diffuse)
{
    /*
    *******random sampling*******
    Vector3 totalColor(0, 0, 0);
    for(int i = 0; i < samplesx; i++){
        float xRand = (rand() % 1025) / 1024.0f;
        float yRand = (rand() % 1025) / 1024.0f;
        Vector3 p = position + (xRand * right) + (yRand * up);
        Ray shadow(ray.point, p - ray.point);
        if(!raytracer->intersectRay(shadow) || shadow.t > 1.0f){
            Vector3 l = shadow.dir;
            float atten = getAttenuation(l.getLength());
            l.normalize();

            Vector3 color = atten * intensity * raytracer->calculateShading(ray, n, l, diffuse);
            color.x *= lightColor.x;
            color.y *= lightColor.y;
            color.z *= lightColor.z;
            totalColor += color;
        }
    }

    return totalColor * (1.0f / (float)samplesx);
    */

    /*
    //*********uniform sampling********
    Vector3 totalColor(0, 0, 0);
    float currentX = 0.0f;
    float currentY = 0.0f;
    float xStep = 1.0f / (float)samplesx;
    float yStep = 1.0f / (float)samplesy;

    for(int i = 0; i < samplesy; i++){
        currentX = 0.0f;
        for(int j = 0; j < samplesx; j++){
            Vector3 p = position + ((currentX + xStep / 2.0f) * right) + ((currentY + yStep / 2.0f) * up);
            Ray shadow(ray.point, p - ray.point);
            if(!raytracer->intersectRay(shadow) || shadow.t > 1.0f){
                Vector3 l = shadow.dir;
                float atten = getAttenuation(l.getLength());
                l.normalize();

                Vector3 color = atten * intensity * raytracer->calculateShading(ray, n, l, diffuse);
                color.x *= lightColor.x;
                color.y *= lightColor.y;
                color.z *= lightColor.z;
                totalColor += color;
            }
            currentX += xStep;
        }
        currentY += yStep;
    }

    return totalColor * (1.0f / (float)(samplesx * samplesy));
    */

    //*******jitter sampling******
    Vector3 totalColor(0, 0, 0);
    float currentX = 0.0f;
    float currentY = 0.0f;
    float xStep = 1.0f / (float)samplesx;
    float yStep = 1.0f / (float)samplesy;

    for(int i = 0; i < samplesy; i++){
        currentX = 0.0f;
        for(int j = 0; j < samplesx; j++){
            float xRand = (rand() % 1025) / 1024.0f;
            float yRand = (rand() % 1025) / 1024.0f;
            Vector3 p = position + ((currentX + xRand * xStep) * right) + ((currentY + yRand * yStep) * up);
            Ray shadow(ray.point, p - ray.point);
            float factor = raytracer->computeShadowFactor(shadow, 1.0f);
            if(factor > 0.0f){
                Vector3 l = shadow.dir;
                float atten = getAttenuation(l.getLength());
                l.normalize();

                Vector3 color = factor * atten * intensity * raytracer->calculateShading(ray, n, l, diffuse);
                color.x *= lightColor.x;
                color.y *= lightColor.y;
                color.z *= lightColor.z;
                totalColor += color;
            }
            currentX += xStep;
        }
        currentY += yStep;
    }

    return totalColor * (1.0f / (float)(samplesx * samplesy));
}
