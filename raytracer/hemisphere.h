#ifndef HEMISPHERE_H_INCLUDED
#define HEMISPHERE_H_INCLUDED

#include "vector.h"
#include "matrix.h"

class Hemisphere
{
    public:

        Hemisphere(Vector3&, float);

        Vector3 sample(void);
        void multiSample(std::vector<Vector3>&, int, int);

    private:

        Matrix4x4 tangentSpace;
        float cosMaxAngle;

        void computeDirection(float, float, Vector3&);
};

#endif // HEMISPHERE_H_INCLUDED
