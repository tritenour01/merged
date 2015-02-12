#include <loader.h>
#include <fileManager.h>
#include <dataModel.h>

bool Loader::parseScene(string fileName, fileObject* f)
{
    data = f;
    errorFlag = false;
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

void Loader::parse(void)
{
    parseConfig();
    acceptToken(Scanner::Separator);
    data->text = QString(scanner.getRemainingText().c_str());
    parseToken(Scanner::StreamDone);
}

void Loader::parseConfig(void)
{
    if(currentToken == Scanner::StreamDone)
        return;
    else{
        while(errorFlag == false && currentToken != Scanner::StreamDone)
        {
            if(currentToken == Scanner::Separator)
                return;
            acceptToken(Scanner::Id);
            string tokenText = scanner.tokenText();

            if(tokenText == "width"){
                parseNumber(data->scene.imageSize[0]);
            }
            else if(tokenText == "height"){
                parseNumber(data->scene.imageSize[1]);
            }
            else if(tokenText == "ambient"){
                parseNumber(data->scene.ambient);
            }
            else if(tokenText == "background"){
                parseVector(data->scene.backgroundColor);
            }
            else if(tokenText == "depth"){
                float depth;
                parseNumber(depth);

                data->scene.recursive[0] = depth;
            }
            else if(tokenText == "glossyReflectSampling"){
                int sampling;
                parseNumber(sampling);

                data->scene.recursive[1] = sampling;
            }
            else if(tokenText == "glossyRefractSampling"){
                int sampling;
                parseNumber(sampling);

                data->scene.recursive[2] = sampling;
            }
            else if(tokenText == "uniform"){
                int level;
                parseNumber(level);

                data->scene.antialiasing.type = "uniform";
                data->scene.antialiasing.sampling = level;
            }
            else if(tokenText == "jitter"){
                int level;
                parseNumber(level);

                data->scene.antialiasing.type = "jitter";
                data->scene.antialiasing.sampling = level;
            }
            else if(tokenText == "adaptive"){
                int level;
                parseNumber(level);

                float threshold[3];
                parseVector(threshold);

                data->scene.antialiasing.type = "adaptive";
                data->scene.antialiasing.sampling = level;
                data->scene.antialiasing.threshold = threshold[0];
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

void Loader::parseVector(float* retVal)
{
    if(errorFlag)
        return;

    parseToken(Scanner::LeftAngle);
    parseNumber(retVal[0]);
    parseToken(Scanner::Comma);
    parseNumber(retVal[1]);
    parseToken(Scanner::Comma);
    parseNumber(retVal[2]);
    parseToken(Scanner::RightAngle);

    if(errorFlag)
        error("invalid vector declaration");
}

void Loader::parseVector(int* retVal)
{
    if(errorFlag)
        return;

    parseToken(Scanner::LeftAngle);
    parseNumber(retVal[0]);
    parseToken(Scanner::Comma);
    parseNumber(retVal[1]);
    parseToken(Scanner::Comma);
    parseNumber(retVal[2]);
    parseToken(Scanner::RightAngle);

    if(errorFlag)
        error("invalid vector declaration");
}

void Loader::parseNumber(float& val)
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

void Loader::parseNumber(int& val)
{
    parseToken(Scanner::Int);
    if(errorFlag == false){
        string strVal = scanner.tokenText();
        val = atoi(strVal.c_str());
    }
}

void Loader::parseCamera(void)
{
    parseToken(Scanner::LeftCurly);

    parseVector(data->camera.position);

    parseVector(data->camera.lookat);

    parseVector(data->camera.up);

    parseToken(Scanner::RightCurly);
}

void Loader::advance(void)
{
    currentToken = scanner.nextToken();
}

void Loader::acceptToken(Scanner::tokenType t)
{
    if(errorFlag == true)
        return;
    if(currentToken != t)
        error("unexpected " + tokenName(currentToken, false) + ", a " + tokenName(t, true) + " was expected");
}

void Loader::parseToken(Scanner::tokenType t)
{
    if(errorFlag == true)
        return;
    advance();
    if(currentToken != t)
        error("unexpected " + tokenName(currentToken, false) + ", a " + tokenName(t, true) + " was expected");
}

void Loader::error(string message)
{
    Log::writeLine(Log::intToString(scanner.lineNum()) + ": ERROR: " + message);
    errorFlag = true;
}

string Loader::tokenName(Scanner::tokenType t, bool side)
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
        case Scanner::Separator:
            return "Separator";
        case Scanner::ERROR:
            return "invalid token";
        case Scanner::StreamDone:
            return "file end";
    }
}
