#ifndef PHOTONTRACER_H_INCLUDED
#define PHOTONTRACER_H_INCLUDED

#include "photonMap.h"

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

        Action determineAction(Ray&);

        Raytracer* raytracer;
        PhotonMap& photonMap;

        int maxBounces;
};

#endif // PHOTONTRACER_H_INCLUDED
