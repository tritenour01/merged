#ifndef PHOTONMAP_H_INCLUDED
#define PHOTONMAP_H_INCLUDED

#include <vector>
#include <algorithm>
#include "vector.h"
#include "log.h"

struct Photon
{
    Vector3 pos;
    Vector3 direction;
    Vector3 power;
    Vector3 normal;

    Photon(Vector3& p, Vector3& d, Vector3& c)
    {
        pos = p;
        direction = d;
        power = c;
        normal = Vector3(0, 0, 0);
    }
};

struct NearestN
{
    Vector3 pos;
    Vector3 normal;
    std::vector<float>* distances;
    std::vector<Photon*>* results;
    int num;
    float radius;
};

struct KDnode
{
    KDnode* LeftChild;
    KDnode* rightChild;
    int axis;
    float median;
    std::vector<Photon*> data;

    KDnode(void)
    {
        LeftChild = NULL;
        rightChild = NULL;
    }
};

class PhotonMap
{
    public:

        PhotonMap(void);

        void store(Photon&);

        void setup(void);

        Photon& nearest(Vector3&);
        void nearestN(Vector3&, Vector3&, std::vector<Photon*>&, int, float);

    private:

        KDnode* constructTree(std::vector<Photon*>&, int);

        Photon* nearestSearch(Vector3&, KDnode*);
        void nearestNSearch(KDnode*, NearestN*);

        float distanceBetween(Vector3&, Vector3&);

        KDnode* root;
        int maxDepth;

        std::vector<Photon> photons;
};

#endif // PHOTONMAP_H_INCLUDED
