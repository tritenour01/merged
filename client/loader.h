#ifndef SCENEPARSER_H_INCLUDED
#define SCENEPARSER_H_INCLUDED

#include <scanner.h>

class fileObject;

class Loader
{
    public:

        bool parseScene(string, fileObject*);

    private:

        void parse(void);
        void parseConfig(void);
        void parseObjects(void);

        void parseVector(float*);
        void parseVector(int*);
        void parseNumber(float&);
        void parseNumber(int&);

        void parseCamera(void);
        void parsePhoton(void);

        void advance(void);
        void acceptToken(Scanner::tokenType);
        void parseToken(Scanner::tokenType);

        void error(string message);
        bool errorFlag;

        string tokenName(Scanner::tokenType, bool);

        Scanner scanner;
        Scanner::tokenType currentToken;

        fileObject* data;
};

#endif // SCENEPARSER_H_INCLUDED
