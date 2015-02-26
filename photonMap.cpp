#include "photonMap.h"

PhotonMap::PhotonMap(void)
{
    root = NULL;
    maxDepth = 10;
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
    else if(depth >= maxDepth || dataSet.size() == 1){
        KDnode* node = new KDnode;
        for(int i = 0; i < dataSet.size(); i++)
            node->data.push_back(dataSet[i]);
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

        float median = dataSet[dataSet.size() / 2]->pos.elements[axis];

        std::vector<Photon*> leftData;
        std::vector<Photon*> rightData;

        for(int i = 0; i < dataSet.size(); i++){
            Photon* photon = dataSet[i];
            if(photon->pos.elements[axis] < median)
                leftData.push_back(photon);
            else
                rightData.push_back(photon);
        }

        KDnode* node = new KDnode;
        node->median = median;
        node->axis = axis;
        node->LeftChild = constructTree(leftData, depth + 1);
        node->rightChild = constructTree(rightData, depth + 1);

        return node;
    }
}

void PhotonMap::store(Photon& p)
{
    photons.push_back(p);
}

float PhotonMap::distanceBetween(Vector3& v1, Vector3& v2)
{
    Vector3 distVector = v1 - v2;
    return distVector.getLength();
}

Photon& PhotonMap::nearest(Vector3& pos)
{
    Photon* near = nearestSearch(pos, root);
    return *near;
}

void PhotonMap::nearestN(Vector3& pos, std::vector<Photon*>& results, int num, float radius)
{
    if(root == NULL)
        return;
    std::vector<float> distances;
    nearestNSearch(pos, root, distances, results, num, radius);
}

Photon* PhotonMap::nearestSearch(Vector3& pos, KDnode* node)
{
    if(node->data.size() > 0){
        float closestDist = 1e8f;
        int closestIndex;
        for(int i = 0; i < node->data.size(); i++){
            Photon* current = node->data[i];
            float dist = distanceBetween(pos, current->pos);
            if(dist < closestDist){
                closestDist = dist;
                closestIndex = i;
            }
        }
        return node->data[closestIndex];
    }

    if(node->LeftChild && node->rightChild){
        if(pos.elements[node->axis] < node->median){
            Photon* leftPhoton = nearestSearch(pos, node->LeftChild);
            float dist = distanceBetween(pos, leftPhoton->pos);
            if(dist >= node->median - leftPhoton->pos.elements[node->axis]){
                Photon* rightPhoton = nearestSearch(pos, node->rightChild);
                if(dist < distanceBetween(pos, rightPhoton->pos))
                    return leftPhoton;
                else
                    return rightPhoton;
            }
            else
                return leftPhoton;
        }
        else{
            Photon* rightPhoton = nearestSearch(pos, node->rightChild);
            float dist = distanceBetween(pos, rightPhoton->pos);
            if(dist >= rightPhoton->pos.elements[node->axis] - node->median){
                Photon* leftPhoton = nearestSearch(pos, node->LeftChild);
                if(dist < distanceBetween(pos, leftPhoton->pos))
                    return rightPhoton;
                else
                    return leftPhoton;
            }
            else
                return rightPhoton;
        }
    }
    else if(node->LeftChild){
        return nearestSearch(pos, node->LeftChild);
    }
    else if(node->rightChild){
        return nearestSearch(pos, node->rightChild);
    }
    else
        Log::writeLine("Something went wrong");
}

void PhotonMap::nearestNSearch(Vector3& pos, KDnode* node, std::vector<float>& distances, std::vector<Photon*>& results, int num, float radius)
{
    if(node->data.size() > 0){
        int index = 0;
        while(distances.size() < num && index < node->data.size()){
            float dist = distanceBetween(pos, node->data[index]->pos);
            if(dist <= radius){
                distances.push_back(dist);
                results.push_back(node->data[index]);
            }
            index++;
        }
        if(index < node->data.size()){
            float maxDist = 0.0f;
            for(int i = 0; i < distances.size(); i++)
                maxDist = max(maxDist, distances[i]);
            for(int i = index; i < node->data.size(); i++){
                Photon* current = node->data[i];
                float dist = distanceBetween(pos, current->pos);
                if(dist < maxDist){
                    int index;
                    float d = 0.0f;
                    for(int i = 0; i < distances.size(); i++){
                        if(distances[i] > d){
                            index = i;
                            d = distances[i];
                        }
                    }
                    distances[index] = dist;
                    results[index] = current;
                    maxDist = 0.0f;
                    for(int i = 0; i < distances.size(); i++)
                        maxDist = max(maxDist, distances[i]);
                }
            }
        }
        return;
    }

    if(node->LeftChild && node->rightChild){
        if(pos.elements[node->axis] < node->median){
            nearestNSearch(pos, node->LeftChild, distances, results, num, radius);
            float dist = 0;
            if(distances.size() == 0)
                dist = 1e8f;
            for(int i = 0; i < distances.size(); i++)
                dist = max(dist, distances[i]);
            if(pos.elements[node->axis] + dist >= node->median &&
               pos.elements[node->axis] + radius >= node->median){
                nearestNSearch(pos, node->rightChild, distances, results, num, radius);
            }
        }
        else{
            nearestNSearch(pos, node->rightChild, distances, results, num, radius);
            float dist = 0;
            if(distances.size() == 0)
                dist = 1e8f;
            for(int i = 0; i < distances.size(); i++)
                dist = max(dist, distances[i]);
            if(pos.elements[node->axis] - dist < node->median &&
               pos.elements[node->axis] - radius < node->median){
                nearestNSearch(pos, node->LeftChild, distances, results, num, radius);
            }
        }
    }
    else if(node->LeftChild){
        nearestNSearch(pos, node->LeftChild, distances, results, num, radius);
    }
    else if(node->rightChild){
        nearestNSearch(pos, node->rightChild, distances, results, num, radius);
    }
    else
        Log::writeLine("Something went wrong");
}
