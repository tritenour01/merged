#ifndef SCENEPARSER_H_INCLUDED
#define SCENEPARSER_H_INCLUDED

#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "box.h"
#include "cylinder.h"
#include "cone.h"
#include "mesh.h"
#include "camera.h"
#include "light.h"

#include "scanner.h"

class Raytracer;
class Parser;
struct Config;

class SceneParser
{
    public:

        SceneParser(Raytracer*, Config&, Parser*);

        bool parseScene(string);

    private:

        void parse(void);
        void parseConfig(void);
        void parseObjects(void);

        void parseVector(Vector3&);
        void parseNumber(float&);
        void parseNumber(int&);

        void parseBox(void);
        void parseCone(void);
        void parseCylinder(void);
        void parseMesh(void);
        void parsePlane(void);
        void parseSphere(void);
        void parseTriangle(void);

        void parseCamera(void);
        void parsePhoton(void);

        void parsePointLight(void);
        void parseDirectionalLight(void);
        void parseSpotlight(void);
        void parseAreaLight(void);

        void parseMaterial(Shape*);
        void parseTransform(Shape*);

        void advance(void);
        void acceptToken(Scanner::tokenType);
        void parseToken(Scanner::tokenType);

        void error(string message);
        bool errorFlag;

        string tokenName(Scanner::tokenType, bool);

        Scanner scanner;
        Scanner::tokenType currentToken;

        Raytracer* raytracer;
        Config& config;
        Parser* parser;
};

#endif // SCENEPARSER_H_INCLUDED
