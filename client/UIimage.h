#ifndef UIIMAGE_H
#define UIIMAGE_H

#include <QObject>

#include <image.h>

class UIimage : public QObject, public Image
{
    Q_OBJECT

    public:

        UIimage(string, int, int, unsigned char*);
        void setPixel(int, int, Vector3&);
        Vector3 getPixel(int, int);
        unsigned char* getPtr(void);

        int getWidth(void);
        int getHeight(void);

    signals:

        void changed(void);

    private:

        //used to store an individual pixel
        struct pixel
        {
            unsigned char b, g, r, a;

            pixel(){}

            //creates a colored pixel and clamps values with rounding
            pixel(Vector3 color)
            {
                color *= 255.0f;
                r = min((int)(color.x + 0.5f), 255);
                g = min((int)(color.y + 0.5f), 255);
                b = min((int)(color.z + 0.5f), 255);
                a = 255;
            }
        };

        string name;
        int width;
        int height;

        pixel* data;
};

#endif // UIIMAGE_H
