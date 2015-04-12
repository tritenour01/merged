#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <iostream>
#include <SFML/Graphics/Image.hpp>
#include "vector.h"

using namespace std;

class Texture
{
    public:

        enum SamplingType {NEAREST, BILINEAR};
        enum ClampingType {CLAMP, REPEAT};

        static Texture* loadTexture(string, SamplingType, ClampingType);

        Vector3 sampleTexture(float, float);
        Vector3 sampleBump(float, float);
        Vector3 sampleNormal(float, float);

    private:

        Texture(string, SamplingType, ClampingType, bool&);

        string filePath;
        int imageWidth;
        int imageHeight;
        sf::Image tex;

        SamplingType sampling;
        ClampingType clamping;
};

#endif // TEXTURE_H_INCLUDED
