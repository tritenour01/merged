#include <UIimage.h>
#include <mainwindow.h>

UIimage::UIimage(std::string n, int w, int h, unsigned char* d)
{
    name = n;
    width = w;
    height = h;
    data = (pixel*)d;

    connect(this, SIGNAL(changedPixel()), window::getInstance(), SLOT(imageChanged()));
}

void UIimage::setPixel(int x, int y, Vector3& color)
{
    data[y * width + x] = pixel(color);
    emit changedPixel();
}

unsigned char* UIimage::getPtr(void)
{
    return (unsigned char*)data;
}
