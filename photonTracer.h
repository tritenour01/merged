#ifndef PHOTONTRACER_H_INCLUDED
#define PHOTONTRACER_H_INCLUDED

#include "photonMap.h"
#include "hemisphere.h"
#include "hemisphere.h"

class Raytracer;
class Ray;

class PhotonTracer
{
    public:

        PhotonTracer(Raytracer*, PhotonMap&, int);

        void tracePhoton(Photon&);

    private:

        enum Action {ABSORB, DIFFUSE, REFLECT, REFRACT};

        void trace(Photon&, int);

        Vector3 diffuseDirection(Vector3&);
        Vector3 reflectDirection(Vector3&, Vector3&);
        Vector3 refractDirection(Vector3&, Vector3&, float);

        Action determineAction(Ray&);

        Raytracer* raytracer;
        PhotonMap& photonMap;

        int maxBounces;
};

#endif // PHOTONTRACER_H_INCLUDED
