#include <UIimage.h>
#include <mainwindow.h>

UIimage::UIimage(std::string n, int w, int h, unsigned char* d)
{
    name = n;
    width = w;
    height = h;
    data = (pixel*)d;
}

void UIimage::setPixel(int x, int y, Vector3& color)
{
    data[y * width + x] = pixel(color);
    emit changed();
}

Vector3 UIimage::getPixel(int x, int y)
{
    pixel p = data[y * width + x];
    return Vector3(p.r / 255.0f, p.g / 255.0f, p.b / 255.0f);
}

unsigned char* UIimage::getPtr(void)
{
    return (unsigned char*)data;
}

int UIimage::getWidth(void)
{
    return width;
}

int UIimage::getHeight(void)
{
    return height;
}
