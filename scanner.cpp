#include "scanner.h"

bool Scanner::setupScanner(string fileName)
{
    file.open(fileName.c_str());
    if(!file.is_open()){
        cout<<"unable to open file: "<<fileName<<endl;
        return false;
    }
    file.get(currentChar);
    lineNumber = 1;
    token = "";
    errorFlag = false;
    debugFlag = false;
    return true;
}

Scanner::tokenType Scanner::nextToken()
{
    token = "";

    skipWhitespace();

    if(errorFlag)
        return ERROR;
    if(!hasNext())
        return StreamDone;

    tokenType returnVal = ERROR;
    if(isLetter(currentChar)){
        token += currentChar;
        returnVal = Id;
        nextChar();
        while(hasNext() && isLetter(currentChar))
        {
            token += currentChar;
            nextChar();
        }
        if(isNumber(currentChar))
            error("invalid ident : " + token + currentChar);
    }
    else if(currentChar == '-' || currentChar == '.' || isNumber(currentChar)){
        bool decimal = false;
        if(currentChar == '-'){
            token += currentChar;
            nextChar();
        }
        if(currentChar == '.'){
            token += currentChar;
            nextChar();
            decimal = true;
        }
        else{
            token += currentChar;
            returnVal = Int;
            nextChar();
            while(hasNext() && isNumber(currentChar))
            {
                token += currentChar;
                nextChar();
            }
        }
        if(decimal || currentChar == '.'){
            if(!decimal){
                token += currentChar;
                nextChar();
            }
            if(!isNumber(currentChar)){
                string message = "invalid number " + token + currentChar;
                error(message);
            }
            returnVal = Float;
            while(hasNext() && isNumber(currentChar))
            {
                token += currentChar;
                nextChar();
            }
            if(isLetter(currentChar))
                error("invalid number " + token + currentChar);
        }
        else if(isLetter(currentChar))
            error("invalid number " + token + currentChar);
    }
    else if(currentChar == '"'){
        returnVal = String;
        nextChar();
        while(currentChar != '\n' && currentChar != '\r' && currentChar != '"'){
            token += currentChar;
            nextChar();
        }
        if(currentChar == '"')
            nextChar();
        else if(currentChar == '\n' || currentChar == '\r')
            error("unterminated string constant \"" + token + "\"");
    }
    else if(currentChar == ','){
        token = currentChar;
        returnVal = Comma;
        nextChar();
    }
    else if(currentChar == '<'){
        token = currentChar;
        returnVal = LeftAngle;
        nextChar();
    }
    else if(currentChar == '>'){
        token = currentChar;
        returnVal = RightAngle;
        nextChar();
    }
    else if(currentChar == '{'){
        token = currentChar;
        returnVal = LeftCurly;
        nextChar();
    }
    else if(currentChar == '}'){
        token = currentChar;
        returnVal = RightCurly;
        nextChar();
    }
    else{
        string c = "";
        c += currentChar;
        error("invalid character \"" + c + "\"");
    }

    if(errorFlag)
        returnVal =  ERROR;

    if(debugFlag)
        debug(returnVal);

    return returnVal;
}

string Scanner::tokenText(void)
{
    return token;
}

int Scanner::lineNum(void)
{
    return lineNumber;
}

void Scanner::skipWhitespace(void)
{
    bool Continue = true;
    while(Continue){
        Continue = false;
        while(hasNext() && isWhitespace(currentChar)){
            nextChar();
            Continue = true;
        }
        Continue |= singleLineComment();
        Continue |= multiLineComment();
    }
}

bool Scanner::singleLineComment(void)
{
    if(currentChar == '/' && peek() == '/'){
        while(currentChar != '\n' && currentChar != '\r')
            nextChar();
        return true;
    }
    return false;
}

bool Scanner::multiLineComment(void)
{
    if(currentChar == '/' && peek() == '*'){
        int lineChange = 0;
        nextChar();
        nextChar();
        while(currentChar != '*' || peek() != '/'){
            if(!hasNext()){
                error("unterminated comment");
                return false;
            }
            if(currentChar == '\n' || currentChar == 'r')
                lineChange++;
            nextChar();
        }
        nextChar();
        nextChar();
        lineNumber += lineChange;
        return true;
    }

    return false;
}

void Scanner::nextChar(void)
{
    file.get(currentChar);
}

bool Scanner::hasNext(void)
{
    return !file.eof();
}

char Scanner::peek(void)
{
    return (char)file.peek();
}

bool Scanner::isNumber(char c)
{
    int val = (int)c;
    return (val >= 48) && (val <= 57);
}

bool Scanner::isLetter(char c)
{
    int val = (int)c;
    return ((val >= 65) && (val <= 90)) || ((val >= 97) && (val <= 122));
}

bool Scanner::isWhitespace(char c)
{
    switch(c)
    {
        case '\n':
            lineNumber++;
        case ' ':
        case '\r':
        case '\t':
            return true;
    }
    return false;
}

void Scanner::error(string errorMessage)
{
    if(errorFlag)
        return;
    cout<<lineNumber<<": ERROR: "<<errorMessage<<endl;
    errorFlag = true;
}

void Scanner::debug(tokenType t)
{
    switch(t)
    {
        case Scanner::Id:
            cout<<"TOKEN ID: "<<token<<" @ line "<<lineNumber<<endl;
            break;
        case Scanner::Int:
            cout<<"TOKEN INT: "<<token<<" @ line "<<lineNumber<<endl;
            break;
        case Scanner::Float:
            cout<<"TOKEN FLOAT: "<<token<<" @ line "<<lineNumber<<endl;
            break;
        case Scanner::String:
            cout<<"TOKEN STRING: "<<token<<" @ line "<<lineNumber<<endl;
            break;
        case Scanner::Comma:
            cout<<"TOKEN COMMA: "<<token<<" @ line "<<lineNumber<<endl;
            break;
        case Scanner::LeftAngle:
            cout<<"TOKEN LEFTANGLE: "<<token<<" @ line "<<lineNumber<<endl;
            break;
        case Scanner::RightAngle:
            cout<<"TOKEN RIGHTANGLE: "<<token<<" @ line "<<lineNumber<<endl;
            break;
        case Scanner::LeftCurly:
            cout<<"TOKEN LEFTCURLY: "<<token<<" @ line "<<lineNumber<<endl;
            break;
        case Scanner::RightCurly:
            cout<<"TOKEN RIGHTCURLY: "<<token<<" @ line "<<lineNumber<<endl;
            break;
        case Scanner::StreamDone:
            cout<<"TOKEN STREAMDONE: "<<token<<" @ line "<<lineNumber<<endl;
            break;
        case Scanner::ERROR:
            cout<<"TOKEN ERROR: "<<token<<endl;
            break;
    }
}
