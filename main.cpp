#include <iostream>
#include <SFML/Graphics/Image.hpp>

#include "raytracer.h"
#include "debug.h"

using namespace std;

#ifdef DEBUG
int currentCount;
pixel* debug = NULL;
#endif

int main(int argc, char** argv)
{
    if(argc != 2){
        cout<<"error: expected an input scene file\n";
        return 0;
    }

    string fileName(argv[1]);

    //trace the image
    Raytracer R(fileName);

    #ifdef DEBUG
    debug = new pixel[R.getWidth() * R.getHeight()];
    #endif

    pixel* traced = R.traceImage();

    if(traced){
        //save the image
        sf::Image i;
        i.create(R.getWidth(), R.getHeight(), (uint8_t*)traced);
        i.saveToFile("image.png");

        #ifdef DEBUG
        sf::Image d;
        d.create(R.getWidth(), R.getHeight(), (uint8_t*)debug);
        d.saveToFile("debug.png");
        #endif
    }

    return 0;
}
