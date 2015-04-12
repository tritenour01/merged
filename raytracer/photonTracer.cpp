#include "photonTracer.h"
#include "raytracer.h"

PhotonTracer::PhotonTracer(Raytracer* r, PhotonMap& p, int b) :
    raytracer(r), photonMap(p), maxBounces(b)
{}

void PhotonTracer::tracePhoton(Photon& p)
{
    trace(p, 0);
}

void PhotonTracer::trace(Photon& p, int depth)
{
    if(depth > maxBounces)
        return;

    Ray ray(p.pos, p.direction);
    if(raytracer->intersectRay(ray)){
        if(ray.s->getMaterial().getDiffuseFactor() > 0.0f && depth != 0){
            Vector3 normal = ray.s->computeNormal(ray);
            Photon hit(ray.point, p.direction, p.power);
            hit.normal = normal;
            photonMap.store(hit);
        }
    }
    else
        return;

    Action action = determineAction(ray);

    Vector3 normal = ray.s->computeNormal(ray);
    Vector3 newDir;

    if(action == DIFFUSE){
        newDir = diffuseDirection(normal);
        Vector3& power = p.power;
        Vector3 diffuse = ray.s->getMaterial().getDiffuse(ray);
        power.x *= diffuse.x;
        power.y *= diffuse.y;
        power.z *= diffuse.z;
    }
    else if(action == REFLECT)
        newDir = reflectDirection(p.direction, normal);
    else if(action == REFRACT)
        newDir = refractDirection(p.direction, normal, ray.s->getMaterial().getIOR());
    else
        return;

    p.pos = ray.point;
    p.direction = newDir;

    trace(p, depth + 1);
}

PhotonTracer::Action PhotonTracer::determineAction(Ray& ray)
{
    Shape* s = ray.s;

    Vector3 diffuseColor = s->getMaterial().getDiffuse(ray);
    float diffuse = (diffuseColor.x + diffuseColor.y + diffuseColor.z) / 3.0f;
    float reflect = s->getMaterial().getReflective();
    float refract = s->getMaterial().getRefraction();

    float randomVal = (float)(rand() % 1025) / 1024.0f;

    if(randomVal <= diffuse)
        return DIFFUSE;
    if(randomVal <= reflect)
        return REFLECT;
    if(randomVal <= refract)
        return REFRACT;
    else
        return ABSORB;
}

Vector3 PhotonTracer::diffuseDirection(Vector3& normal)
{
    Hemisphere hemi(normal, 89.0f);
    Vector3 dir = hemi.sample();
    return dir;
}

Vector3 PhotonTracer::reflectDirection(Vector3& dir, Vector3& normal)
{
    Vector3 newDir = (2.0f * Vector3::DotProduct(normal, dir) * normal) - dir;
    return newDir;
}

Vector3 PhotonTracer::refractDirection(Vector3& dir, Vector3& normal, float IOR)
{
    Vector3 result;

    bool TIR = false;
    float n = 1.0f;
    float nt = IOR;

    float incidentDot = Vector3::DotProduct(dir, normal);

    //entering the object
    if(incidentDot < 0){
        //refraction index
        float compN = n / nt;

        //compute the refracted direction
        double c1, cs2;
        c1 = -incidentDot;
        cs2 = 1.0f - compN * compN * (1.0f - c1 * c1);
        result = compN * dir + (compN * c1 - sqrt(cs2)) *  normal;
        result.normalize();
    }
    //exiting the glass
    else{
        //refraction index
        float compN = nt / n;

        double c1, cs2;
        c1 = incidentDot;
        cs2 = 1.0f - compN * compN * (1.0f - c1 * c1);

        if(cs2 < 0.0f)
            TIR = true;

        if(!TIR){
            //compute the refracted direction
            result = compN * dir - (compN * c1 - sqrt(cs2)) *  normal;
            result.normalize();
        }
    }

    if(TIR){
        Vector3 negativeNormal = -normal;
        return reflectDirection(dir, negativeNormal);
    }

    float cos1, cos2;
    if(incidentDot < 0){
        cos1 = -incidentDot;
        cos2 = -Vector3::DotProduct(normal, result);
    }
    else{
        cos1 = incidentDot;
        cos2 = Vector3::DotProduct(normal, result);
    }

    float parallel = (nt * cos1 - n * cos2) / (nt * cos1 + n * cos2);
    float perp = (n * cos1 - nt * cos2) / (n * cos1 + nt * cos2);

    float reflectComp = 0.5f * (parallel * parallel + perp * perp);

    float randomVal = (float)(rand() % 1025) / 1024.0f;
    if(randomVal <= reflectComp){
        if(incidentDot < 0.0f)
            return reflectDirection(dir, normal);
        else{
            Vector3 negativeNormal = -normal;
            return reflectDirection(dir, negativeNormal);
        }
    }
    else
        return result;
}
