#ifndef RAYTRACER_H_INCLUDED
#define RAYTRACER_H_INCLUDED

#include <fstream>
#include <vector>
#include <time.h>

#include "shape.h"
#include "camera.h"
#include "light.h"

#include "debug.h"

#include "sampler.h"
#include "parser.h"

#include "image.h"

#include "photonMap.h"

struct Config
{
    enum Mode {STANDARD, PHOTON};
    Mode mode;

    int width;
    int height;
    float ambient;
    Vector3 backColor;
    int reflectionDepth;
    float recursionThreshold;
    int glossyReflectSampling;
    int glossyRefractSampling;

    int photonCount;
    int maxPhotonSamples;
    float photonSearchRadius;
    int photonBounces;

    float gamma;

    Camera* camera;
    Sampler* sampler;
};

struct Hitpoint
{
    float t;
    Vector3 point;

    Shape* s;
    float f1;
    float f2;
};

class Raytracer
{
    public:

        Raytracer();
        ~Raytracer();

        bool loadScene(string);

        Vector3 tracePixel(int, int);
        Vector3 traceRay(Ray&);
        bool intersectRay(Ray&);
        float computeShadowFactor(Ray&, float);
        Vector3 calculateShading(Ray&, Vector3&, Vector3&, Vector3&);

        int getWidth(void);
        int getHeight(void);

        void addObject(Shape*);
        void addLight(Light*);

    private:

        void setupPhotonMap(void);

        Vector3 computeColor(Ray&, int, float);
        Vector3 calculateLightStandard(Ray&, Vector3&);
        Vector3 calculateLightPhoton(Ray&, Vector3&);
        Vector3 calculateReflection(Ray&, Vector3&, int, float);
        Vector3 calculateGlossyReflection(Ray&, Vector3&, int, float);
        Vector3 calculateRefraction(Ray&, Vector3&, int, float);
        bool refractVector(Vector3&, Vector3&, Vector3&, float);
        Vector3 calculateGlossyRefraction(Ray&, Vector3&, int, float);
        float calculateAO(Ray&, int);

        void gammaCorrection(Vector3&);

        vector<Shape*> objects;
        vector<Light*> lights;

        Config config;
        Parser* parser;
        PhotonMap* photonMap;
};

#endif // RAYTRACER_H_INCLUDED
