#include "sampler.h"
#include "raytracer.h"

Sampler::Sampler(Raytracer* r, Config& c) : raytracer(r), config(c){}

simpleSampler::simpleSampler(Raytracer* r, Config& c) :
    Sampler(r, c)
{}

Vector3 simpleSampler::samplePixel(int x, int y)
{
    Ray ray;
    config.camera->computeRay(x, 0.5f, y, 0.5f, ray);

    return raytracer->traceRay(ray);
}

uniformSampler::uniformSampler(Raytracer* r, Config& c, int s) :
    Sampler(r, c)
{
    sampling = s;
}

Vector3 uniformSampler::samplePixel(int x, int y)
{
    Vector3 color = Vector3(0, 0, 0);

    //current offset in the pixel
    float initX = 1.0f / (2.0f * (float)sampling);
    float Xoffset;
    float Yoffset = 1.0f / (2.0f * (float)sampling);

    //offset to next row/column
    float Xincrement = 1.0f / (float)sampling;
    float Yincrement = 1.0f / (float)sampling;

    //determine the average color in the pixel
    //based on the amount of super sampling
    for(int i = 0; i < sampling; i++){
        Xoffset = initX;
        for(int j = 0; j < sampling; j++){

            //determine the color at the sample
            Ray ray;
            config.camera->computeRay(x, Xoffset, y, Yoffset, ray);

            color += raytracer->traceRay(ray);

            Xoffset += Xincrement;
        }
        Yoffset += Yincrement;
    }

    //compute the average of the samples
    color /= (sampling * sampling);

    return color;
}

adaptiveSampler::adaptiveSampler(Raytracer* r, Config& c, int s, Vector3 t) :
    Sampler(r, c)
{
    sampling = s;
    threshold = t;
}

Vector3 adaptiveSampler::samplePixel(int x, int y)
{

}
