#include "camera.h"

Camera::Camera(Vector3 o, Vector3 p, Vector3 u, int w, int h)
{
    //image size in pixels
    width = w;
    height = h;

    //camera postion
    position = o;

    //camera direction
    direction = p - o;
    direction.normalize();

    //camera right axis
    right = Vector3::CrossProduct(u, direction);
    right.normalize();

    //camera up axis
    up = Vector3::CrossProduct(direction, right);
    up.normalize();

    //y field of view
    fovY = 60.0f * 180.0f / 3.14159f;
}

void Camera::computeRay(int x, float offsetX, int y, float offsetY, Ray& result)
{
    result.origin = position;

    //ray direction based on many factors
    float xFactor = tanf(fovY / 2.0f) / ((float)height / (float)width) * (x + offsetX - (width / 2)) / (width / 2);
    float yFactor = tanf(fovY / 2.0f) * ((height / 2) - y - offsetY) / (height / 2);
    result.dir = direction + xFactor * right + yFactor * up;
}

Vector3 Camera::getPosition(void)
{
    return position;
}
