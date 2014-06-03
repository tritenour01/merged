#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "vector.h"
#include "ray.h"

//change the camera so the constructor sets a default coord-frame
//add have set frame and set FOV functions for the parser's use

class Camera
{
    public:

        Camera(Vector3, Vector3, Vector3, int, int);

        void computeRay(int, float, int, float, Ray&);

        Vector3 getPosition(void);

    private:

        Vector3 position;

        Vector3 direction;
        Vector3 up;
        Vector3 right;

        int width;
        int height;

        float fovX;
        float fovY;
};

#endif // CAMERA_H_INCLUDED
