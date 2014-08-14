#include <iostream>
#include <fstream>
#include <SFML/Graphics/Image.hpp>

#include "raytracer.h"
#include "debug.h"
#include <inttypes.h>
#include "image.h"
#include "manager.h"

using namespace std;

#ifdef DEBUG
int currentCount;
pixel* debug = NULL;
#endif

bool readConfig(string, string&, int&, int&);

int main(int argc, char** argv)
{
    if(argc != 3){
        cout<<"error: expected an input scene file\n";
        return 0;
    }

    string fileName(argv[1]);
    string configName(argv[2]);

    string outputFile;
    int numThreads;
    int blockSetup;
    if(!readConfig(configName, outputFile, numThreads, blockSetup))
        return 0;

    //trace the image
    Raytracer R;
    if(!R.loadScene(fileName))
        return 0;

    #ifdef DEBUG
    debug = new pixel[R.getWidth() * R.getHeight()];
    #endif

    Image img(outputFile, R.getWidth(), R.getHeight());

    Manager manager(numThreads, blockSetup, &img, &R);
    manager.Render();

    //save the image
    sf::Image i;
    i.create(R.getWidth(), R.getHeight(), img.getPtr());
    i.saveToFile(outputFile);

    #ifdef DEBUG
    sf::Image d;
    d.create(R.getWidth(), R.getHeight(), (uint8_t*)debug);
    d.saveToFile("debug.png");
    #endif

    return 0;
}

bool readConfig(string fileName, string& outputFile, int& threads, int& blocks)
{
    ifstream file;
    file.open(fileName);
    if(!file.is_open()){
        cout<<"Error: Unable to open config file\n";
        return false;
    }

    if(!file.eof()){
        file>>outputFile;
        ifstream test;
        test.open(outputFile, std::ofstream::binary);
        if(!test.is_open()){
            cout<<"ERROR: unable to open destination file: "<<outputFile<<"\n";
            return false;
        }
        test.close();

        int loc = outputFile.find(".");
        string type = outputFile.substr(loc + 1, outputFile.length() - loc - 1);
        if(type != "png" && type != "jpg"){
            cout<<"ERROR: invalid destination file type: "<<type<<endl;
            return false;
        }
    }
    else{
        cout<<"ERROR: config file expected a file name\n";
        return false;
    }

    if(!file.eof()){
        string val;
        file>>val;

        if(val.find('.') < val.length()){
            cout<<"ERROR: config file expected an integer for number of threads\n";
            return false;
        }

        threads = atoi(val.c_str());
    }
    else{
        cout<<"ERROR: config file expected number of threads\n";
        return false;
    }

    if(!file.eof()){
        string val;
        file>>val;

        if(val.find('.') < val.length()){
            cout<<"ERROR: config file expected an integer for block config\n";
            return false;
        }

        blocks = atoi(val.c_str());
    }
    else{
        cout<<"ERROR: config file expected block config\n";
        return false;
    }

    string e;
    file>>e;
    if(!file.eof()){
        cout<<"ERROR: unexpected \""<<e<<"\", expected end of config file\n";
        return false;
    }

    file.close();
    return true;
}
