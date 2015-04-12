#include "photonMap.h"

PhotonMap::PhotonMap(void)
{
    root = NULL;
}

void PhotonMap::setup(void)
{
    std::vector<Photon*> dataSet;
    for(int i = 0; i < photons.size(); i++)
        dataSet.push_back(&photons[i]);

    if(dataSet.size() > 0)
        root = constructTree(dataSet, 0);
    else
        root = NULL;
}

bool sortByX(Photon* lhs, Photon* rhs)
{
    return lhs->pos.x < rhs->pos.x;
}

bool sortByY(Photon* lhs, Photon* rhs)
{
    return lhs->pos.y < rhs->pos.y;
}

bool sortByZ(Photon* lhs, Photon* rhs)
{
    return lhs->pos.z < rhs->pos.z;
}

KDnode* PhotonMap::constructTree(std::vector<Photon*>& dataSet, int depth)
{
    if(dataSet.size() == 0)
        return NULL;
    else if(dataSet.size() == 1){
        KDnode* node = new KDnode;
        node->data = dataSet[0];
        node->axis = -1;
        return node;
    }
    else{
        int axis = depth % 3;

        if(axis == 0)
            std::sort(dataSet.begin(), dataSet.end(), sortByX);
        else if(axis == 1)
            std::sort(dataSet.begin(), dataSet.end(), sortByY);
        else
            std::sort(dataSet.begin(), dataSet.end(), sortByZ);

        KDnode* node = new KDnode;
        node->data = dataSet[dataSet.size() / 2];
        node->axis = axis;

        dataSet.erase(dataSet.begin() + dataSet.size() / 2);
        float median = node->data->pos.elements[axis];

        std::vector<Photon*> leftData;
        std::vector<Photon*> rightData;

        for(int i = 0; i < dataSet.size(); i++){
            Photon* photon = dataSet[i];
            if(photon->pos.elements[axis] < median)
                leftData.push_back(photon);
            else
                rightData.push_back(photon);
        }

        node->LeftChild = constructTree(leftData, depth + 1);
        node->rightChild = constructTree(rightData, depth + 1);

        return node;
    }
}

void PhotonMap::store(Photon& p)
{
    photons.push_back(p);
}

float PhotonMap::distanceBetweenSqr(Vector3& v1, Vector3& v2)
{
    Vector3 distVector = v1 - v2;
    return distVector.getSqrLength();
}

void PhotonMap::nearestN(Vector3& pos, Vector3& normal, std::vector<Photon*>& results, int num, float radius)
{
    if(root == NULL)
        return;
    std::vector<float> distances;
    NearestN near;
    near.pos = pos;
    near.normal = normal;
    near.results = &results;
    near.sqrDistances = &distances;
    near.maxDistSqr = -1.0f;
    near.num = num;
    near.radiusSqr = radius * radius;
    near.isHeap = false;
    nearestNSearch(root, &near);
}

void PhotonMap::nearestNSearch(KDnode* node, NearestN* near)
{
    if(node->LeftChild && node->rightChild){
        if(near->pos.elements[node->axis] < node->data->pos.elements[node->axis]){
            nearestNSearch(node->LeftChild, near);
            float maxDist = near->maxDistSqr;
            if(maxDist < 0)
                maxDist = 1e8f;
            float dist = node->data->pos.elements[node->axis] - near->pos.elements[node->axis];
            if(dist * dist <= near->radiusSqr &&
               (near->sqrDistances->size() != near->num || (near->sqrDistances->size() == near->num &&
               dist * dist <= maxDist))){
                nearestNSearch(node->rightChild, near);
            }
        }
        else{
            nearestNSearch(node->rightChild, near);
            float maxDist = near->maxDistSqr;
            if(maxDist < 0)
                maxDist = 1e8f;
            float dist = near->pos.elements[node->axis] - node->data->pos.elements[node->axis];
            if(dist * dist <= near->radiusSqr &&
               (near->sqrDistances->size() != near->num || (near->sqrDistances->size() == near->num &&
               dist * dist <= maxDist))){
                nearestNSearch(node->LeftChild, near);
            }
        }
    }
    else if(node->LeftChild){
        nearestNSearch(node->LeftChild, near);
    }
    else if(node->rightChild){
        nearestNSearch(node->rightChild, near);
    }

    float maxDist = -1.0f;
    Photon* data = node->data;
    float dot = Vector3::DotProduct(near->normal, data->normal);
    float distSqr = distanceBetweenSqr(near->pos, data->pos);
    if(distSqr <= near->radiusSqr && dot >= 0.9){
        if(near->sqrDistances->size() < near->num){
            near->sqrDistances->push_back(distSqr);
            near->results->push_back(data);
            near->maxDistSqr = max(near->maxDistSqr, distSqr);
        }
        else{
            if(!near->isHeap){
                int half = near->num / 2;
                for(int i = half; i >= 0; i--){
                    float distanceSqr = near->sqrDistances->at(i);
                    Photon* photon = near->results->at(i);
                    int parent = i + 1;
                    while(parent <= half){
                        int child = parent * 2;
                        if(child < near->num && near->sqrDistances->at(child -1) < near->sqrDistances->at(child + 1 - 1))
                            child++;
                        if(distanceSqr > near->sqrDistances->at(child - 1))
                            break;
                        near->sqrDistances->at(parent - 1) = near->sqrDistances->at(child - 1);
                        near->results->at(parent - 1) = near->results->at(child - 1);
                        parent = child;
                    }
                    near->sqrDistances->at(parent - 1) = distanceSqr;
                    near->results->at(parent - 1) = photon;
                }
                near->isHeap = true;
            }

            if(distSqr >= near->sqrDistances->at(0))
                return;

            int parent = 1;
            int child = 2;
            while(child <= near->num){
                if(child < near->num && near->sqrDistances->at(child - 1) < near->sqrDistances->at(child + 1 - 1))
                    child++;
                if(distSqr > near->sqrDistances->at(child - 1))
                    break;
                near->sqrDistances->at(parent - 1) = near->sqrDistances->at(child - 1);
                near->results->at(parent - 1) = near->results->at(child - 1);
                parent = child;
                child += child;
            }
            near->sqrDistances->at(parent - 1) = distSqr;
            near->results->at(parent - 1) = data;
            near->maxDistSqr = near->sqrDistances->at(0);
        }
    }
}
