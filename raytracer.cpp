#include "raytracer.h"
#include "parser.h"

Raytracer::Raytracer(string scene)
{
    //set the default
    config.width = 100;
    config.height = 100;
    config.camera = NULL;
    config.ambient = 0.0f;
    config.backColor = Vector3(0.0f, 0.0f, 0.0f);
    config.reflectionDepth = 1;
    config.sampler = NULL;

    parser = new Parser(this);

    //load the scene into the tracer
    badScene = !parser->loadScene(scene, config);

    if(config.sampler == NULL)
        config.sampler = new simpleSampler(this, config);
    if(config.camera == NULL)
        config.camera = new Camera(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0), config.width, config.height);
}

Raytracer::~Raytracer(void)
{
    //delete objects

    //delete lights

    if(config.camera)
       delete config.camera;
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

//ray trace the image
pixel* Raytracer::traceImage(void)
{
    if(badScene)
        return NULL;

    pixel* image = new pixel[config.width * config.height];

    //set the color of each pixel
    int counter = 0;
    for(int i = 0; i < config.height; i++){
        cout<<"\r"<<i+1<<"/"<<config.height<<" rows         ";
        for(int j = 0; j < config.width; j++){
            //cout<<j<<endl;
            #ifdef DEBUG
            currentCount = counter;
            debug[counter] = pixel(Vector3(0, 0, 0));
            #endif

            if(j == 200 && i == 200)
                cout<<"HERE";

            image[counter] = pixel(config.sampler->samplePixel(j, i));
            counter++;
        }
    }
    cout<<endl;

    return image;
}

Vector3 Raytracer::traceRay(Ray& ray)
{
    intersectRay(ray);

    if(ray.s)
        return computeColor(ray, 0);
    return config.backColor;
}

//test the ray against all objects
bool Raytracer::intersectRay(Ray& ray)
{
    float minT = DBL_MAX;
    Shape* minS = NULL;
    Vector3 point;
    Hitpoint hit;
    for(int k = 0; k < objects.size(); k++){
        if(objects.at(k)->intersectRay(ray, hit)){
            if(hit.t > Ray::SMALL && hit.t < minT){
                minT = hit.t;
                minS = objects.at(k);
                point = hit.point;
            }
        }
    }

    ray.t = minT;
    ray.s = minS;
    ray.point = point;

    if(!minS)
        return false;

    return true;
}

//determine the color based on the intersection point
Vector3 Raytracer::computeColor(Ray& ray, int depth)
{
    //recursive base case
    if(depth > config.reflectionDepth)
        return Vector3(0, 0, 0);

    //normal at point
    Vector3 n = ray.s->computeNormal(ray.point);

    //reflection amount
    Vector3 reflection(0, 0, 0);
    if(ray.s->getMaterial().getReflective() > 0.0f)
        reflection = calculateReflection(ray, n, depth);

    //refraction amount
    Vector3 refraction(0, 0, 0);
    if(ray.s->getMaterial().getRefraction() > 0.0f)
        refraction = calculateRefraction(ray, n, depth);

    Vector3 amb = config.ambient * ray.s->getMaterial().getDiffuse(ray.point);

    //final color
    return amb + calculateLight(ray, n) + reflection + refraction;
}

//computes the light calculations
Vector3 Raytracer::calculateLight(Ray& ray, Vector3& n)
{
    Vector3 diffuse = ray.s->getMaterial().getDiffuse(ray.point);

    Light* current;
    Vector3 totalColor = Vector3(0, 0, 0);

    //compute the effect of every light in the scene
    for(int i = 0; i < lights.size(); i++){
        current = lights[i];

        //add light color
        //change name of calculateLight to calculateShading

        //determine if the point is affected by light
        Ray shadow = Ray(ray.point, current->getPos() - ray.point);
        if(!intersectRay(shadow) || shadow.t > 1.0f){

            //light direction
            Vector3 l = current->getPos() - ray.point;
            float dist = l.getLength();
            l.normalize();

            //light reflection vector
            Vector3 r = (2.0f * Vector3::DotProduct(n, l) * n) - l;
            r.normalize();

            //view direction
            Vector3 v = config.camera->getPosition() - ray.point;
            v.normalize();

            float atten = current->getAttenuation(dist);

            Vector3 color = Vector3(0, 0, 0);
            if(ray.s->getMaterial().getDiffuseFactor() > 0.0){
                //diffuse term
                color += atten * current->getIntensity() * diffuse * ray.s->getMaterial().getDiffuseFactor() * max(Vector3::DotProduct(l, n), 0.0f);
            }
            if(ray.s->getMaterial().getSpecularFactor() > 0.0){
                //specular term
                color += atten * current->getIntensity() * ray.s->getMaterial().getSpecularFactor() * ray.s->getMaterial().getSpecular() * pow(max(0.0f, Vector3::DotProduct(v, r)), ray.s->getMaterial().getShineness());
            }

            Vector3 lightColor = current->getColor();
            color.x *= lightColor.x;
            color.y *= lightColor.y;
            color.z *= lightColor.z;
            totalColor +=  color;
        }
    }

    return totalColor;
}


//computes the reflection color
Vector3 Raytracer::calculateReflection(Ray& ray, Vector3& n, int depth)
{
    //view direction
    Vector3 view = -ray.dir;

    //reflection vector
    Vector3 R = (2.0f * Vector3::DotProduct(n, view) * n) - view;

    //compute the reflection color
    Ray reflect = Ray(ray.point, R);
    Vector3 c;
    if(intersectRay(reflect))
        c = ray.s->getMaterial().getReflective() * computeColor(reflect, depth + 1);
    else
        c = Vector3(0, 0, 0);
    return c;
}

//compute the refraction color
Vector3 Raytracer::calculateRefraction(Ray& ray, Vector3& normal, int depth)
{
    //view direction
    Vector3 view = ray.dir;
    view.normalize();

    Vector3 result;
    float n;

    //entering the object
    if(Vector3::DotProduct(view, normal) < 0){
        //refraction index
        n = 0.666666f;

        //compute the resultant direction
        float sqrtComponent = sqrt(1 - (n * n * (1 - pow(Vector3::DotProduct(-view, normal), 2.0f))));
        Vector3 other = n * (-view - normal * Vector3::DotProduct(-view, normal));
        result = other - normal * sqrtComponent;
    }
    //exiting the glass
    else{
        //refraction index
        n = 1.5f;

        //compute the resultant direction
        float sqrtComponent = sqrt(1 - (n * n * (1 - pow(Vector3::DotProduct(-view, -normal), 2.0f))));
        Vector3 other = n * (-view + normal * Vector3::DotProduct(-view, -normal));
        Vector3 result = other + normal * sqrtComponent;
    }
    result.normalize();

    //compute the refraction color
    Ray refract(ray.point, result);
    if(!intersectRay(refract))
        return Vector3(0, 0, 0);
    return ray.s->getMaterial().getRefraction() * computeColor(refract, depth);
}
