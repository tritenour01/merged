#include "hemisphere.h"
#include "ray.h"

Hemisphere::Hemisphere(Vector3& normal, float maxAngle)
{
    Vector3 tangent;
    if(fabs(normal.x) >= Ray::SMALL || fabs(normal.z) >= Ray::SMALL)
        tangent = Vector3::CrossProduct(normal, Vector3(0, 1, 0));
    else
        tangent = Vector3(1, 0, 0);

    Vector3 bitangent = Vector3::CrossProduct(tangent, normal);
    tangent.normalize();
    bitangent.normalize();

    tangentSpace[0] = tangent.x;
    tangentSpace[1] = tangent.y;
    tangentSpace[2] = tangent.z;
    tangentSpace[4] = bitangent.x;
    tangentSpace[5] = bitangent.y;
    tangentSpace[6] = bitangent.z;
    tangentSpace[8] = normal.x;
    tangentSpace[9] = normal.y;
    tangentSpace[10] = normal.z;

    cosMaxAngle = cosf(maxAngle * 3.1415938f / 180.0f);
}

Vector3 Hemisphere::sample(void)
{
    float u = (float)(rand() % 1025) / 1024.0f;
    float v = (float)(rand() % 1025) / 1024.0f;
    Vector3 direction(0, 0, 0);
    computeDirection(u, v, direction);
    return direction;
}

void Hemisphere::multiSample(std::vector<Vector3>& samples, int samplesX, int samplesY)
{

}

void Hemisphere::computeDirection(float uCoord, float vCoord, Vector3& result)
{
    float theta = 2.0f * 3.1415938f * vCoord;
    float factor = sqrtf(1.0f - powf(1.0f - uCoord * (1.0f - cosMaxAngle), 2.0f));
    Vector3 dir(factor * cosf(theta), factor * sinf(theta), 1 - uCoord * (1.0f - cosMaxAngle));

    Matrix4x4::transformDirection(tangentSpace, result, dir);
}
