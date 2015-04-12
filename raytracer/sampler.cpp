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

jitterSampler::jitterSampler(Raytracer* r, Config& c, int s) :
    Sampler(r, c)
{
    sampling = s;
}

Vector3 jitterSampler::samplePixel(int x, int y)
{
    Vector3 color = Vector3(0, 0, 0);

    //current offset in the pixel
    float Xoffset;
    float Yoffset = 0.0f;

    //offset to next row/column
    float Xincrement = 1.0f / (float)sampling;
    float Yincrement = 1.0f / (float)sampling;

    //determine the average color in the pixel
    //based on the amount of super sampling
    for(int i = 0; i < sampling; i++){
        Xoffset = 0.0f;
        for(int j = 0; j < sampling; j++){

            float randX = (float)(rand() % 101) / 100.0f;
            float randY = (float)(rand() % 101) / 100.0f;

            //determine the color at the sample
            Ray ray;
            config.camera->computeRay(x, Xoffset + randX * Xincrement, y, Yoffset + randY * Yincrement, ray);

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
    setupCache();
}

void adaptiveSampler::setupCache(void)
{
    cacheSize = 2;
    for(int i = 0; i < sampling; i++, cacheSize += cacheSize - 1);

    samplerCache = new samplerNode*[cacheSize];
    for(int i = 0; i < cacheSize; i++)
        samplerCache[i] = new samplerNode[cacheSize];

    for(int i = 0; i < cacheSize; i++){
        for(int j = 0; j < cacheSize; j++){
            samplerCache[j][i].id = -1;
            samplerCache[j][i].xOffset = (float)j / (float)(cacheSize - 1);
            samplerCache[j][i].yOffset = (float)i / (float)(cacheSize - 1);
        }
    }
}

bool adaptiveSampler::checkVariance(Vector3& v1, Vector3& v2)
{
    Vector3 dif = v1 - v2;
    return (fabs(dif.x) >= threshold.x ||
            fabs(dif.y) >= threshold.y ||
            fabs(dif.z) >= threshold.z);
}

Vector3 adaptiveSampler::samplePixel(int x, int y)
{
    static int i = 0;
    i++;
    return recursiveSample(x, y, 0, 0, cacheSize - 1, 0, i);
}

Vector3 adaptiveSampler::recursiveSample(int pixelX, int pixelY, int x, int y, int width, int depth, int ID)
{
    Vector3 colors[4];
    Vector3 c(0, 0, 0);
    Ray r;

    int currentX, currentY;
    for(int i = 0; i < 4; i++){
        if(i % 2 == 0)
            currentX = x;
        else
            currentX = x + width;
        if(i < 2)
            currentY = y;
        else
            currentY = y + width;

        if(samplerCache[currentX][currentY].id == ID)
            colors[i] = samplerCache[currentX][currentY].color;
        else{
            config.camera->computeRay(pixelX, samplerCache[currentX][currentY].xOffset,
                                      pixelY, samplerCache[currentX][currentY].yOffset, r);
            colors[i] = raytracer->traceRay(r);
            samplerCache[currentX][currentY].id = ID;
            samplerCache[currentX][currentY].color = colors[i];
        }

        c += colors[i];
    }

    if(depth < sampling && (checkVariance(colors[0], colors[1]) || checkVariance(colors[0], colors[2]) ||
       checkVariance(colors[1], colors[3]) || checkVariance(colors[2], colors[3])))
    {
        Vector3 color = recursiveSample(pixelX, pixelY, x, y, width / 2, depth + 1, ID) +
                        recursiveSample(pixelX, pixelY, x + width / 2, y, width / 2, depth + 1, ID) +
                        recursiveSample(pixelX, pixelY, x, y + width / 2, width / 2, depth + 1, ID) +
                        recursiveSample(pixelX, pixelY, x + width / 2, y + width / 2, width / 2, depth + 1, ID);
        color /= 4.0f;
        return color;
    }

    c /= 4.0f;
    return c;
}
