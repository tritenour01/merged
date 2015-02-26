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
        Photon hit(ray.point, p.direction, p.power);
        photonMap.store(hit);
    }
    else
        return;

    Action action = determineAction(ray);

    Vector3 normal = ray.s->computeNormal(ray);
    Vector3 newDir;

    if(action == DIFFUSE)
        newDir = diffuseDirection(normal);
    else
        return;

    p.pos = ray.point;
    p.direction = newDir;

    trace(p, depth + 1);
}

PhotonTracer::Action PhotonTracer::determineAction(Ray& ray)
{
    Shape* s = ray.s;

    float diffuse = s->getMaterial().getDiffuseFactor();

    float randomVal = (float)(rand() % 1025) / 1024.0f;

    if(randomVal <= diffuse)
        return DIFFUSE;
    else
        return ABSORB;
}

Vector3 PhotonTracer::diffuseDirection(Vector3& normal)
{
    Vector3 tangent;
    if(fabs(normal.x) >= Ray::SMALL || fabs(normal.z) >= Ray::SMALL)
        tangent = Vector3::CrossProduct(normal, Vector3(0, 1, 0));
    else
        tangent = Vector3(1, 0, 0);

    Vector3 bitangent = Vector3::CrossProduct(tangent, normal);
    tangent.normalize();
    bitangent.normalize();

    Matrix4x4 tangentSpace;
    tangentSpace[0] = tangent.x;
    tangentSpace[1] = tangent.y;
    tangentSpace[2] = tangent.z;
    tangentSpace[4] = bitangent.x;
    tangentSpace[5] = bitangent.y;
    tangentSpace[6] = bitangent.z;
    tangentSpace[8] = normal.x;
    tangentSpace[9] = normal.y;
    tangentSpace[10] = normal.z;

    float maxAngle = 89.0f;
    float m = cosf(maxAngle * 3.1415938f / 180.0f);

    float u = (float)(rand() % 1025) / 1024.0f;
    float v = (float)(rand() % 1025) / 1024.0f;
    float theta = 2.0f * 3.1415938f * v;
    float factor = sqrtf(1.0f - powf(1.0f - u * (1.0f - m), 2.0f));
    Vector3 d(factor * cosf(theta), factor * sinf(theta), 1 - u * (1.0f - m));

    Vector3 dir;
    Matrix4x4::transformDirection(tangentSpace, dir, d);

    return dir;
}
