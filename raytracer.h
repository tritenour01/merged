#ifndef RAYTRACER_H_INCLUDED
#define RAYTRACER_H_INCLUDED

#include <fstream>
#include <vector>

#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "box.h"
#include "cylinder.h"
#include "camera.h"
#include "light.h"

#include "debug.h"

#include "sampler.h"
#include "parser.h"

//used to store an individual pixel
struct pixel
{
    unsigned char r, g, b, a;

    pixel(){}

    //creates a colored pixel and clamps values with rounding
    pixel(Vector3 color)
    {
        color *= 255.0f;
        r = min((int)(color.x + 0.5f), 255);
        g = min((int)(color.y + 0.5f), 255);
        b = min((int)(color.z + 0.5f), 255);

        //toon shading
        //r = min((int)((truncf(((float)r / 255.0f) * 10.0f) / 9.0f) * 255.0f), 255);
        //g = min((int)((truncf(((float)g / 255.0f) * 10.0f) / 9.0f) * 255.0f), 255);
        //b = min((int)((truncf(((float)b / 255.0f) * 10.0f) / 9.0f) * 255.0f), 255);

        //gray scale
        //float y = 0.0;
        //y += 0.2126 * ((float)r / 255.0f);
        //y += 0.7152 * ((float)g / 255.0f);
        //y += 0.0722 * ((float)b / 255.0f);

        //r = min((int)(y * 255.0f), 255);
        //g = min((int)(y * 255.0f), 255);
        //b = min((int)(y * 255.0f), 255);

        a = 255;
    }
};

struct Config
{
    int width;
    int height;
    float ambient;
    Vector3 backColor;
    int reflectionDepth;

    Camera* camera;
    Sampler* sampler;
};

struct Hitpoint
{
    float t;
    Vector3 point;
};

class Raytracer
{
    public:

        Raytracer(string);
        ~Raytracer();

        pixel* traceImage();
        Vector3 traceRay(Ray&);

        int getWidth(void);
        int getHeight(void);

        void addObject(Shape*);
        void addLight(Light*);

    private:

        void loadScene(string);
        bool badScene;

        bool intersectRay(Ray&);
        Vector3 computeColor(Ray&, int);
        Vector3 calculateLight(Ray&, Vector3&);
        Vector3 calculateReflection(Ray&, Vector3&, int);
        Vector3 calculateRefraction(Ray&, Vector3&, int);

        vector<Shape*> objects;
        vector<Light*> lights;

        Config config;
        Parser* parser;
};

#endif // RAYTRACER_H_INCLUDED
