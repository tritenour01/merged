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

                //toon shading
                //r = min((int)((truncf(((float)r / 255.0f) * 10.0f) / 9.0f) * 255.0f), 255);
                //g = min((int)((truncf(((float)g / 255.0f) * 10.0f) / 9.0f) * 255.0f), 255);
                //b = min((int)((truncf(((float)b / 255.0f) * 10.0f) / 9.0f) * 255.0f), 255);

                //gray scale
                //float y = 0.0;
                //y += 0.2126 * ((float)r / 255.0f);
                //y += 0.7152 * ((float)g / 255.0f);
                //y += 0.0722 * ((float)b / 255.0f);

                //r = min((int)(y * 255.0f), 255);
                //g = min((int)(y * 255.0f), 255);
                //b = min((int)(y * 255.0f), 255);

                a = 255;
            }
        };

        string name;
        int width;
        int height;

        pixel* data;
};

#endif // UIIMAGE_H
