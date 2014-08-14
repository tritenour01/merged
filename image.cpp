#include "image.h"

Image::Image(string n, int w, int h)
{
    name = n;
    width = w;
    height = h;
    data = new pixel[width * height];
}

void Image::setPixel(int x, int y, Vector3& color)
{
    data[y * width + x] = pixel(color);
}

uint8_t* Image::getPtr(void)
{
    return (uint8_t*)data;
}
