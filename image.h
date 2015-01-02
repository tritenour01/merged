#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <inttypes.h>

#include "vector.h"

class Image
{
    public:

        virtual void setPixel(int, int, Vector3&) =0;
};

#endif // IMAGE_H_INCLUDED
