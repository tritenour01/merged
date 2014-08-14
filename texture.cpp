#include "texture.h"

Texture* Texture::loadTexture(string file, SamplingType t, ClampingType c)
{
    bool success = true;
    Texture* newTexture = new Texture(file, t, c, success);
    if(success)
        return newTexture;
    delete newTexture;
    return NULL;
}

Texture::Texture(string file, SamplingType t, ClampingType c, bool& retVal)
{
    filePath = file;
    if(!tex.loadFromFile(file)){
        retVal = false;
        return;
    }
    sf::Vector2u s = tex.getSize();
    imageWidth = s.x;
    imageHeight = s.y;
    sampling = t;
    clamping = c;
}

Vector3 Texture::sampleTexture(float u, float v)
{
    if(clamping == REPEAT){
        if(u > 1.0f){
            float k;
            modf(u, &k);
            u -= k;
        }
        else if(u < 0.0f){
            float k;
            modf(-u, &k);
            u += k + 1;
        }

        if(v > 1.0f){
            float k;
            modf(v, &k);
            v -= k;
        }
        else if(v < 0.0f){
            float k;
            modf(-v, &k);
            v += k + 1;
        }
    }
    else if(clamping == CLAMP){
        if(u > 1.0f)
            u = 1.0f;
        else if(u < 0.0f)
            u = 0.0f;

        if(v > 1.0f)
            v = 1.0f;
        else if(v < 0.0f)
            v = 0.0f;
    }

    v = 1 - v;

    if(sampling == BILINEAR){
        float uCenter = u * (float)imageWidth - 0.5f;
        float vCenter = v * (float)imageHeight - 0.5f;

        int uLeft = floor(uCenter);
        int vLeft = floor(vCenter);
        int uRight = uLeft + 1;
        int vRight = vLeft + 1;

        float uDiff = uCenter - uLeft;
        float vDiff = vCenter - vLeft;

        if(uLeft < 0)
            uLeft = imageWidth - 1;
        if(vLeft < 0)
            vLeft = imageHeight - 1;
        if(uRight >= imageWidth)
            uRight = 0;
        if(vRight >= imageHeight)
            vRight = 0;

        sf::Color lbColor = tex.getPixel(uLeft, vLeft);
        sf::Color ltColor = tex.getPixel(uLeft, vRight);
        sf::Color rbColor = tex.getPixel(uRight, vLeft);
        sf::Color rtColor = tex.getPixel(uRight, vRight);

        Vector3 lb = Vector3(lbColor.r / 255.0f, lbColor.g / 255.0f, lbColor.b / 255.0f);
        Vector3 lt = Vector3(ltColor.r / 255.0f, ltColor.g / 255.0f, ltColor.b / 255.0f);
        Vector3 rb = Vector3(rbColor.r / 255.0f, rbColor.g / 255.0f, rbColor.b / 255.0f);
        Vector3 rt = Vector3(rtColor.r / 255.0f, rtColor.g / 255.0f, rtColor.b / 255.0f);

        return (lt * (1 - uDiff) + rt * uDiff) * vDiff + (lb * (1 - uDiff) + rb * uDiff) * (1 - vDiff);
    }
    else if(sampling == NEAREST){
        int uImage = u * (float)imageWidth;
        if(uImage == imageWidth)
            uImage--;
        int vImage = v * (float)imageHeight;
        if(vImage == imageHeight)
            vImage--;
        sf::Color c = tex.getPixel(uImage, vImage);
        return Vector3(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f);
    }
}

Vector3 Texture::sampleBump(float u, float v)
{
    float val00 = 2.0f * sampleTexture(u - 1.0f / (float)imageWidth, v - 1.0f / (float)imageHeight).x - 1.0f;
    float val01 = 2.0f * sampleTexture(u - 1.0f / (float)imageWidth, v).x - 1.0f;
    float val02 = 2.0f * sampleTexture(u - 1.0f / (float)imageWidth, v + 1.0f / (float)imageHeight).x - 1.0f;
    float val10 = 2.0f * sampleTexture(u, v - 1.0f / (float)imageHeight).x - 1.0f;
    float val11 = 2.0f * sampleTexture(u, v).x - 1.0f;
    float val12 = 2.0f * sampleTexture(u, v + 1.0f / (float)imageHeight).x - 1.0f;
    float val20 = 2.0f * sampleTexture(u + 1.0f / (float)imageWidth, v - 1.0f / (float)imageHeight).x - 1.0f;
    float val21 = 2.0f * sampleTexture(u + 1.0f / (float)imageWidth, v).x - 1.0f;
    float val22 = 2.0f * sampleTexture(u + 1.0f / (float)imageWidth, v + 1.0f / (float)imageHeight).x - 1.0f;

    float xDiff = (0.5f * val02) + (2.0f * val01) + (0.5f * val00) - (0.5f * val22) - (2.0f * val21) - (0.5f * val20);
    float yDiff = (0.5f * val00) + (2.0f * val10) + (0.5f * val20) - (0.5f * val02) - (2.0f * val12) - (0.5f * val22);
    return Vector3(xDiff, yDiff, 1.0f);
}

Vector3 Texture::sampleNormal(float u, float v)
{
    Vector3 C = sampleTexture(u, v);
    C *= 2.0f;
    C -= Vector3(1.0f, 1.0f, 1.0f);
    return C;
}
