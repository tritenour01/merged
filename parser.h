#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "sceneParser.h"

class Raytracer;
struct Config;

using namespace std;

class Parser
{
    public:

        Parser(Raytracer*);

        bool loadScene(string, Config&);
        bool loadObj(string, vector<Triangle*>&, vector<Vector3>&);

    private:

        Raytracer* raytracer;
};

#endif // PARSER_H_INCLUDED
