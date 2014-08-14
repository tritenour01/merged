#include "raytracer.h"
#include "parser.h"

Raytracer::Raytracer(void)
{
    //set the default
    config.width = 100;
    config.height = 100;
    config.camera = NULL;
    config.ambient = 0.0f;
    config.backColor = Vector3(0.0f, 0.0f, 0.0f);
    config.reflectionDepth = 1;
    config.recursionThreshold = 1.0f / 256.0f;
    config.sampler = NULL;

    parser = new Parser(this);
}

Raytracer::~Raytracer(void)
{
    //delete objects

    for(int i = 0; i < lights.size(); i++)
        delete lights.at(i);

    if(config.camera)
       delete config.camera;
}

bool Raytracer::loadScene(string fileName)
{
    //load the scene into the tracer
    bool result = parser->loadScene(fileName, config);

    if(config.sampler == NULL)
        config.sampler = new simpleSampler(this, config);
    if(config.camera == NULL)
        config.camera = new Camera(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0), config.width, config.height);

    return result;
}

//return the image width
int Raytracer::getWidth(void)
{
    return config.width;
}

//return the image height
int Raytracer::getHeight(void)
{
    return config.height;
}

void Raytracer::addObject(Shape* newObject)
{
    objects.push_back(newObject);
}

void Raytracer::addLight(Light* newLight)
{
    lights.push_back(newLight);
}

Vector3 Raytracer::tracePixel(int x, int y)
{
    return config.sampler->samplePixel(x, y);
}

Vector3 Raytracer::traceRay(Ray& ray)
{
    intersectRay(ray);

    if(ray.s)
        return computeColor(ray, 0, 1.0f);
    return config.backColor;
}

//test the ray against all objects
bool Raytracer::intersectRay(Ray& ray)
{
    float minT = DBL_MAX;
    Shape* minS = NULL;
    Vector3 point;
    Hitpoint hit;
    float f1;
    float f2;
    Shape* s;
    for(int k = 0; k < objects.size(); k++){
        if(objects.at(k)->intersectRay(ray, hit)){
            if(hit.t > Ray::SMALL && hit.t < minT){
                minT = hit.t;
                minS = objects.at(k);
                point = hit.point;
                f1 = hit.f1;
                f2 = hit.f2;
                s = hit.s;
            }
        }
    }

    ray.t = minT;
    ray.s = minS;
    ray.point = point;
    ray.cacheFloat1 = f1;
    ray.cacheFloat2 = f2;
    ray.cacheShape = s;

    if(!minS)
        return false;

    return true;
}

float Raytracer::computeShadowFactor(Ray& ray, float range)
{
    float minT = DBL_MAX;
    Hitpoint hit;
    float factor = 1.0f;
    for(int k = 0; k < objects.size(); k++){
        if(objects.at(k)->getMaterial().isEmissive())
            continue;
        if(objects.at(k)->intersectRay(ray, hit)){
            if(hit.t > Ray::SMALL && hit.t <= range && hit.t < minT){
                minT = hit.t;
                if(objects.at(k)->getMaterial().getRefraction() > 0.0f)
                    factor = 0.5f;
                else
                    return 0.0f;
            }
        }
    }

    ray.t = minT;
    return factor;
}

//determine the color based on the intersection point
Vector3 Raytracer::computeColor(Ray& ray, int depth, float factor)
{
    //recursive base case
    if(depth > config.reflectionDepth)
        return Vector3(0, 0, 0);

    //contrabution to recursion is too small
    if(factor < config.recursionThreshold)
        return Vector3(0, 0, 0);

    if(ray.s->getMaterial().isEmissive())
        return ray.s->getMaterial().getEmissiveColor();

    //normal at point
    Vector3 n = ray.s->computeNormal(ray);

    //reflection amount
    Vector3 reflection(0, 0, 0);
    if(ray.s->getMaterial().getReflective() > 0.0f)
        reflection = calculateReflection(ray, n, depth, factor);

    //refraction amount
    Vector3 refraction(0, 0, 0);
    if(ray.s->getMaterial().getRefraction() > 0.0f)
        refraction = calculateRefraction(ray, n, depth, factor);

    Vector3 amb = config.ambient * ray.s->getMaterial().getDiffuse(ray);

    //final color
    return amb + calculateLight(ray, n) + reflection + refraction;
}

//computes the light calculations
Vector3 Raytracer::calculateLight(Ray& ray, Vector3& n)
{
    Vector3 diffuse = ray.s->getMaterial().getDiffuse(ray);

    Light* current;
    Vector3 totalColor = Vector3(0, 0, 0);

    //compute the effect of every light in the scene
    for(int i = 0; i < lights.size(); i++){
        current = lights[i];

        totalColor += current->illuminate(ray, n, diffuse);
    }

    return totalColor;
}

Vector3 Raytracer::calculateShading(Ray& ray, Vector3& n, Vector3& l, Vector3& diffuse)
{
    Vector3 color = Vector3(0, 0, 0);
    if(ray.s->getMaterial().getDiffuseFactor() > 0.0){
        //diffuse term
        color += diffuse * ray.s->getMaterial().getDiffuseFactor() * max(Vector3::DotProduct(l, n), 0.0f);
    }
    if(ray.s->getMaterial().getSpecularFactor() > 0.0){

        //light reflection vector
        Vector3 r = (2.0f * Vector3::DotProduct(n, l) * n) - l;
        r.normalize();

        //view direction
        Vector3 v = config.camera->getPosition() - ray.point;
        v.normalize();

        //specular term
        color += ray.s->getMaterial().getSpecularFactor() * ray.s->getMaterial().getSpecular() * pow(max(0.0f, Vector3::DotProduct(v, r)), ray.s->getMaterial().getShineness());
    }

    return color;
}

//computes the reflection color
Vector3 Raytracer::calculateReflection(Ray& ray, Vector3& n, int depth, float factor)
{
    //view direction
    Vector3 view = -ray.dir;

    //reflection vector
    Vector3 R = (2.0f * Vector3::DotProduct(n, view) * n) - view;

    //compute the reflection color
    Ray reflect = Ray(ray.point, R);
    Vector3 c;
    if(intersectRay(reflect))
        c = ray.s->getMaterial().getReflective() * computeColor(reflect, depth + 1, factor * ray.s->getMaterial().getReflective());
    else
        c = Vector3(0, 0, 0);
    return c;
}

//compute the refraction color
Vector3 Raytracer::calculateRefraction(Ray& ray, Vector3& normal, int depth, float factor)
{
    //view direction
    Vector3 view = ray.dir;
    view.normalize();

    float cos1;
    float cos2;
    float n = 1.0f;
    float nt = 1.5f;

    Vector3 result;

    bool TIR = false;

    //entering the object
    if(Vector3::DotProduct(view, normal) < 0){
        //refraction index
        float compN = n / nt;

        //compute the resultant direction
        float sqrtComponent = sqrt(1 - (compN * compN * (1 - pow(Vector3::DotProduct(-view, normal), 2.0f))));
        Vector3 other = compN * (-view - normal * Vector3::DotProduct(-view, normal));
        result = other - normal * sqrtComponent;
        result.normalize();

        cos1 = -Vector3::DotProduct(view, normal);
        cos2 = -Vector3::DotProduct(normal, result);
    }
    //exiting the glass
    else{
        //refraction index
        float compN = nt / n;

        float sinSqrd = compN * compN * (1.0f - Vector3::DotProduct(view, normal) * Vector3::DotProduct(view, normal));
        if(sinSqrd > 1.0f)
            TIR = true;

        if(!TIR){
            //compute the resultant direction
            float sqrtComponent = sqrt(1 - (compN * compN * (1 - pow(Vector3::DotProduct(-view, -normal), 2.0f))));
            Vector3 other = compN * (-view + normal * Vector3::DotProduct(-view, -normal));
            result = other + normal * sqrtComponent;
            result.normalize();

            cos1 = Vector3::DotProduct(view, normal);
            cos2 = Vector3::DotProduct(normal, result);
        }
    }

    //reflection vector
    Vector3 R = (2.0f * Vector3::DotProduct(normal, -view) * normal) + view;

    float parallel = (nt * cos1 - n * cos2) / (nt * cos1 + n * cos2);
    float perp = (n * cos1 - nt * cos2) / (n * cos1 + nt * cos2);

    float reflectComp = 0.5f * (parallel * parallel + perp * perp);

    //compute the reflection color
    Ray reflect = Ray(ray.point, R);
    Vector3 c;
    if(intersectRay(reflect)){
        if(TIR)
            c = computeColor(reflect, depth + 1, factor);
        else
            c = computeColor(reflect, depth + 1, factor * reflectComp);
    }
    else
        c = Vector3(0, 0, 0);

    if(TIR)
        return c;

    //compute the refraction color
    Ray refract(ray.point, result);
    if(!intersectRay(refract))
        return Vector3(0, 0, 0);
    return (1.0f - reflectComp) * ray.s->getMaterial().getRefraction() * computeColor(refract, depth + 1, factor * (1.0f - reflectComp)) + reflectComp * c;
}
