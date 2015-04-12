#ifndef SAMPLERS_H_INCLUDED
#define SAMPLERS_H_INCLUDED

#include "vector.h"

class Raytracer;
struct Config;

class Sampler
{
    public:

        Sampler(Raytracer*, Config&);
        virtual ~Sampler(){}

        virtual Vector3 samplePixel(int, int) =0;

    protected:

        Raytracer* raytracer;
        Config& config;
};

class simpleSampler : public Sampler
{
    public:

        simpleSampler(Raytracer*, Config&);
        Vector3 samplePixel(int, int);
};

class uniformSampler : public Sampler
{
    public:

        uniformSampler(Raytracer*, Config&, int);
        Vector3 samplePixel(int, int);

    private:

        int sampling;
};

class jitterSampler : public Sampler
{
    public:

        jitterSampler(Raytracer*, Config&, int);
        Vector3 samplePixel(int, int);

    private:

        int sampling;
};

class adaptiveSampler : public Sampler
{
    public:

        adaptiveSampler(Raytracer*, Config&, int, Vector3);
        Vector3 samplePixel(int, int);

    private:

        struct samplerNode
        {
            Vector3 color;
            int id;
            float xOffset;
            float yOffset;
        };

        int sampling;
        Vector3 threshold;

        int cacheSize;
        samplerNode** samplerCache;
        void setupCache(void);

        Vector3 recursiveSample(int, int, int, int, int, int, int);

        bool checkVariance(Vector3&, Vector3&);
};

#endif // SAMPLERS_H_INCLUDED
