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
