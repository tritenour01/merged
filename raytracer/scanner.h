#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <iostream>
#include <fstream>

using namespace std;

class Scanner
{
    public:

        enum tokenType {Id, Int, Float, String, Comma, LeftAngle,
                        RightAngle, LeftCurly, RightCurly, StreamDone,
                        Separator, ERROR};

        bool setupScanner(string);
        tokenType nextToken(void);
        string tokenText(void);
        int lineNum(void);

    private:

        void nextChar(void);
        bool hasNext(void);
        char peek(void);

        bool isNumber(char);
        bool isLetter(char);
        bool isWhitespace(char);

        void skipWhitespace(void);
        bool singleLineComment(void);
        bool multiLineComment(void);

        void error(string);
        void debug(tokenType);

        ifstream file;
        char currentChar;
        string token;

        int lineNumber;

        bool errorFlag;
        bool debugFlag;
};

#endif // SCANNER_H_INCLUDED
