#include "sceneParser.h"
#include "raytracer.h"

SceneParser::SceneParser(Raytracer* r, Config& c, Parser* p) : config(c)
{
    raytracer = r;
    parser = p;
    errorFlag = false;
}

bool SceneParser::parseScene(string fileName)
{
    Log::writeLine("parsing scene file: " + fileName);
    if(!scanner.setupScanner(fileName))
        return false;
    advance();
    parse();
    if(errorFlag == false)
        Log::writeLine("parsing successful");
    else
        Log::writeLine("parsing failed");
    return !errorFlag;
}

void SceneParser::parse(void)
{
    parseConfig();
    parseObjects();
    parseToken(Scanner::StreamDone);
}

void SceneParser::parseConfig(void)
{
    if(currentToken == Scanner::StreamDone)
        return;
    else{
        while(errorFlag == false && currentToken != Scanner::StreamDone)
        {
            acceptToken(Scanner::Id);
            string tokenText = scanner.tokenText();

            if(tokenText == "width"){
                parseNumber(config.width);
            }
            else if(tokenText == "height"){
                parseNumber(config.height);
            }
            else if(tokenText == "ambient"){
                parseNumber(config.ambient);
            }
            else if(tokenText == "background"){
                parseVector(config.backColor);
            }
            else if(tokenText == "depth"){
                float depth;
                parseNumber(depth);

                config.reflectionDepth = depth;
            }
            else if(tokenText == "glossyReflectSampling"){
                int sampling;
                parseNumber(sampling);

                config.glossyReflectSampling = sampling;
            }
            else if(tokenText == "uniform"){
                int level;
                parseNumber(level);

                config.sampler = new uniformSampler(raytracer, config, level);
            }
            else if(tokenText == "jitter"){
                int level;
                parseNumber(level);

                config.sampler = new jitterSampler(raytracer, config, level);
            }
            else if(tokenText == "adaptive"){
                int level;
                parseNumber(level);

                Vector3 threshold;
                parseVector(threshold);

                config.sampler = new adaptiveSampler(raytracer, config, level, threshold);
            }
            else if(tokenText == "camera"){
                parseCamera();
            }
            else{
                return;
            }
            advance();
        }
    }
}

void SceneParser::parseObjects(void)
{
    if(currentToken == Scanner::StreamDone)
        return;
    else{
        while(errorFlag == false && currentToken != Scanner::StreamDone)
        {
            acceptToken(Scanner::Id);
            string tokenText = scanner.tokenText();

            if(tokenText == "sphere")
                parseSphere();
            else if(tokenText == "box")
                parseBox();
            else if(tokenText == "plane")
                parsePlane();
            else if(tokenText == "triangle")
                parseTriangle();
            else if(tokenText == "mesh")
                parseMesh();
            else if(tokenText == "cone")
                parseCone();
            else if(tokenText == "cylinder")
                parseCylinder();
            else if(tokenText == "pointlight")
                parsePointLight();
            else if(tokenText == "directionallight")
                parseDirectionalLight();
            else if(tokenText == "spotlight")
                parseSpotlight();
            else if(tokenText == "arealight")
                parseAreaLight();
            else{
                error("undefined command: " + tokenText);
            }

            advance();
        }
    }
}

void SceneParser::parseVector(Vector3& retVal)
{
    if(errorFlag)
        return;

    parseToken(Scanner::LeftAngle);
    parseNumber(retVal.x);
    parseToken(Scanner::Comma);
    parseNumber(retVal.y);
    parseToken(Scanner::Comma);
    parseNumber(retVal.z);
    parseToken(Scanner::RightAngle);

    if(errorFlag)
        error("invalid vector declaration");
}

void SceneParser::parseNumber(float& val)
{
    advance();
    if(currentToken == Scanner::Int || currentToken == Scanner::Float){
        if(errorFlag == false){
            string strVal = scanner.tokenText();
            val = atof(strVal.c_str());
        }
    }
    else
        acceptToken(Scanner::Float);
}

void SceneParser::parseNumber(int& val)
{
    parseToken(Scanner::Int);
    if(errorFlag == false){
        string strVal = scanner.tokenText();
        val = atoi(strVal.c_str());
    }
}

void SceneParser::parseBox(void)
{
    parseToken(Scanner::LeftCurly);

    Vector3 minCorner;
    parseVector(minCorner);

    Vector3 maxCorner;
    parseVector(maxCorner);

    Box* b = new Box(minCorner, maxCorner);
    raytracer->addObject(b);

    advance();

    parseMaterial(b);
    parseTransform(b);

    acceptToken(Scanner::RightCurly);
}

void SceneParser::parseCone(void)
{
    parseToken(Scanner::LeftCurly);

    Vector3 base;
    parseVector(base);

    float height;
    parseNumber(height);

    float radius;
    parseNumber(radius);

    Cone* c = new Cone(base, height, radius);
    raytracer->addObject(c);

    advance();
    parseMaterial(c);
    parseTransform(c);

    acceptToken(Scanner::RightCurly);
}

void SceneParser::parseCylinder(void)
{
    parseToken(Scanner::LeftCurly);

    Vector3 base;
    parseVector(base);

    float height;
    parseNumber(height);

    float radius;
    parseNumber(radius);

    Cylinder* c = new Cylinder(base, height, radius);
    raytracer->addObject(c);

    advance();
    parseMaterial(c);
    parseTransform(c);

    acceptToken(Scanner::RightCurly);
}

void SceneParser::parseMesh(void)
{
    parseToken(Scanner::LeftCurly);

    parseToken(Scanner::String);

    string fileName = scanner.tokenText();
    vector<Triangle*>* newMesh = new vector<Triangle*>;
    vector<Vector3>* newPoints = new vector<Vector3>;
    vector<Vector3>* newNormals = new vector<Vector3>;

    if(!parser->loadObj(fileName, *newMesh, *newPoints, *newNormals))
        errorFlag = true;

    Mesh* m = new Mesh(newMesh, newPoints, newNormals);
    raytracer->addObject(m);

    advance();
    parseMaterial(m);
    parseTransform(m);

    acceptToken(Scanner::RightCurly);
}

void SceneParser::parsePlane(void)
{
    parseToken(Scanner::LeftCurly);

    Vector3 center;
    parseVector(center);

    Vector3 right;
    parseVector(right);

    Vector3 up;
    parseVector(up);

    Plane* p = new Plane(center, right, up);
    raytracer->addObject(p);

    advance();
    parseMaterial(p);
    parseTransform(p);

    acceptToken(Scanner::RightCurly);
}

void SceneParser::parseSphere(void)
{
    parseToken(Scanner::LeftCurly);

    Vector3 center;
    parseVector(center);

    float radius;
    parseNumber(radius);

    Sphere* s = new Sphere(center, radius);
    raytracer->addObject(s);

    advance();

    parseMaterial(s);
    parseTransform(s);

    acceptToken(Scanner::RightCurly);
}

void SceneParser::parseTriangle(void)
{
    parseToken(Scanner::LeftCurly);

    Vector3* p1 = new Vector3();
    parseVector(*p1);

    Vector3* p2 = new Vector3();
    parseVector(*p2);

    Vector3* p3 = new Vector3();
    parseVector(*p3);

    Triangle* t = new Triangle(p1, p2, p3, true);
    raytracer->addObject(t);

    advance();

    parseMaterial(t);
    parseTransform(t);

    acceptToken(Scanner::RightCurly);
}

void SceneParser::parseCamera(void)
{
    parseToken(Scanner::LeftCurly);

    Vector3 o;
    parseVector(o);

    Vector3 p;
    parseVector(p);

    Vector3 u;
    parseVector(u);

    config.camera = new Camera(o, p, u, config.width, config.height);

    parseToken(Scanner::RightCurly);
}

void SceneParser::parsePointLight(void)
{
    parseToken(Scanner::LeftCurly);

    Vector3 pos;
    parseVector(pos);

    Vector3 color;
    parseVector(color);

    Vector3 falloff;
    parseVector(falloff);

    float intensity;
    parseNumber(intensity);

    Light* l = new PointLight(raytracer, pos, color, falloff, intensity);
    raytracer->addLight(l);

    parseToken(Scanner::RightCurly);
}

void SceneParser::parseDirectionalLight(void)
{
    parseToken(Scanner::LeftCurly);

    Vector3 dir;
    parseVector(dir);

    Vector3 color;
    parseVector(color);

    float intensity;
    parseNumber(intensity);

    Light* l = new DirectionalLight(raytracer, dir, color, intensity);
    raytracer->addLight(l);

    parseToken(Scanner::RightCurly);
}

void SceneParser::parseSpotlight(void)
{
    parseToken(Scanner::LeftCurly);

    Vector3 pos;
    parseVector(pos);

    Vector3 lookat;
    parseVector(lookat);

    Vector3 color;
    parseVector(color);

    Vector3 falloff;
    parseVector(falloff);

    float intensity;
    parseNumber(intensity);

    float innerAngle;
    parseNumber(innerAngle);

    float outerNumber;
    parseNumber(outerNumber);

    Light* l = new Spotlight(raytracer, pos, lookat, color, falloff, intensity, innerAngle, outerNumber);
    raytracer->addLight(l);

    parseToken(Scanner::RightCurly);
}

void SceneParser::parseAreaLight(void)
{
    parseToken(Scanner::LeftCurly);

    Vector3 pos;
    parseVector(pos);

    Vector3 right;
    parseVector(right);

    Vector3 up;
    parseVector(up);

    Vector3 color;
    parseVector(color);

    Vector3 falloff;
    parseVector(falloff);

    float intensity;
    parseNumber(intensity);

    int samplesx;
    parseNumber(samplesx);

    int samplesy;
    parseNumber(samplesy);

    Light* l = new AreaLight(raytracer, pos, right, up, color, falloff, intensity, samplesx, samplesy);
    raytracer->addLight(l);

    Plane* p = new Plane(pos + up * 0.5f + right * 0.5f, right, right.getLength() / 2.0f, up, up.getLength() / 2.0f);
    p->getMaterial().setEmissive(color);
    raytracer->addObject(p);

    parseToken(Scanner::RightCurly);
}

void SceneParser::parseMaterial(Shape* s)
{
    if(currentToken != Scanner::Id || scanner.tokenText() != "material")
        return;
    parseToken(Scanner::LeftCurly);
    advance();

    while(errorFlag == false && currentToken == Scanner::Id)
    {
        string tokenText = scanner.tokenText();
        if(tokenText == "diffuse"){
            Vector3 diffuseColor;
            parseVector(diffuseColor);

            float coefficent;
            parseNumber(coefficent);

            s->getMaterial().setDiffuse(diffuseColor);
            s->getMaterial().setDiffuseFactor(coefficent);
        }
        else if(tokenText == "specular"){
            Vector3 color;
            parseVector(color);

            float coefficent;
            parseNumber(coefficent);

            s->getMaterial().setSpecularFactor(coefficent);
            s->getMaterial().setSpecular(color);
        }
        else if(tokenText == "shine"){
            float shineness;
            parseNumber(shineness);

            s->getMaterial().setShineness(shineness);
        }
        else if(tokenText == "reflect"){
            Vector3 color;
            parseVector(color);

            float reflect;
            parseNumber(reflect);

            s->getMaterial().setReflectColor(color);
            s->getMaterial().setReflective(reflect);
        }
        else if(tokenText == "glossyReflect"){
            Vector3 color;
            parseVector(color);

            float reflect;
            parseNumber(reflect);

            float glossy;
            parseNumber(glossy);

            s->getMaterial().setReflectColor(color);
            s->getMaterial().setReflective(reflect);
            s->getMaterial().setGlossiness(glossy);
        }
        else if(tokenText == "refract"){
            float refract;
            parseNumber(refract);

            float IOR;
            parseNumber(IOR);

            s->getMaterial().setRefraction(refract);
            s->getMaterial().setIOR(IOR);
        }
        else if(tokenText == "texture"){
            parseToken(Scanner::String);
            string fileName = scanner.tokenText();

            parseToken(Scanner::Id);
            string sampling = scanner.tokenText();
            Texture::SamplingType samp;

            if(sampling == "NEAREST")
                samp = Texture::NEAREST;
            else if(sampling == "BILINEAR")
                samp = Texture::BILINEAR;
            else
                error("invalid texture filtering option: " + sampling);

            parseToken(Scanner::Id);
            string clamping = scanner.tokenText();
            Texture::ClampingType clamp;

            if(clamping == "CLAMP")
                clamp = Texture::CLAMP;
            else if(clamping == "REPEAT")
                clamp = Texture::REPEAT;
            else
                error("invalid texture clamping option: " + clamping);

            Texture* t = Texture::loadTexture(fileName, samp, clamp);

            if(t == NULL)
                error("Invalid file path \"" + fileName + "\"");

            s->getMaterial().setTexture(t);
        }
        else if(tokenText == "bumpmap"){
            parseToken(Scanner::String);
            string fileName = scanner.tokenText();

            Texture* t = Texture::loadTexture(fileName, Texture::NEAREST, Texture::REPEAT);

            if(t == NULL)
                error("Invalid file path \"" + fileName + "\"");

            s->getMaterial().setBumpMap(t);
        }
        else if(tokenText == "normalmap"){
            parseToken(Scanner::String);
            string fileName = scanner.tokenText();

            Texture* t = Texture::loadTexture(fileName, Texture::NEAREST, Texture::REPEAT);

            if(t == NULL)
                error("Invalid file path \"" + fileName + "\"");

            s->getMaterial().setNormalMap(t);
        }
        else{
            error("undefined material attribute \"" + tokenText + "\"");
        }
        advance();
    }

    acceptToken(Scanner::RightCurly);
    advance();
}

void SceneParser::parseTransform(Shape* s)
{
    if(currentToken != Scanner::Id || scanner.tokenText() != "transform")
        return;
    parseToken(Scanner::LeftCurly);
    advance();

    while(errorFlag == false && currentToken == Scanner::Id)
    {
        string tokenText = scanner.tokenText();

        if(tokenText == "translate"){
            Vector3 factor;
            parseVector(factor);
            s->Translate(factor);
        }
        else if(tokenText == "rotate"){
            Vector3 factor;
            parseVector(factor);
            s->Rotate(factor);
        }
        else if(tokenText == "scale"){
            Vector3 factor;
            parseVector(factor);
            s->Scale(factor);
        }
        else
            error("undefined transform attribute \"" + tokenText + "\"");

         advance();
    }

    acceptToken(Scanner::RightCurly);
    advance();
}

void SceneParser::advance(void)
{
    currentToken = scanner.nextToken();
}

void SceneParser::acceptToken(Scanner::tokenType t)
{
    if(errorFlag == true)
        return;
    if(currentToken != t)
        error("unexpected " + tokenName(currentToken, false) + ", a " + tokenName(t, true) + " was expected");
}

void SceneParser::parseToken(Scanner::tokenType t)
{
    if(errorFlag == true)
        return;
    advance();
    if(currentToken != t)
        error("unexpected " + tokenName(currentToken, false) + ", a " + tokenName(t, true) + " was expected");
}

void SceneParser::error(string message)
{
    Log::writeLine(Log::intToString(scanner.lineNum()) + ": ERROR: " + message);
    errorFlag = true;
}

string SceneParser::tokenName(Scanner::tokenType t, bool side)
{
    switch(t)
    {
        case Scanner::Id:
            if(!side)
                return "id : \"" + scanner.tokenText() + "\"";
            else
                return "id";
        case Scanner::Int:
            if(!side)
                return "interger : " + scanner.tokenText();
            else
                return "integer";
        case Scanner::Float:
            if(!side)
                return "decimal value : " + scanner.tokenText();
            else
                return "decimal value";
        case Scanner::String:
            return "string const";
        case Scanner::Comma:
            return "comma";
        case Scanner::LeftAngle:
            return "left angle bracket";
        case Scanner::RightAngle:
            return "right angle bracket";
        case Scanner::LeftCurly:
            return "left curly bracket";
        case Scanner::RightCurly:
            return "right curly bracket";
        case Scanner::ERROR:
            return "invalid token";
        case Scanner::StreamDone:
            return "file end";
    }
}
