#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include "logger.h"

class Log
{
    public:

        static void write(std::string);
        static void writeLine(std::string);

        static void setLogger(Logger*);

        static std::string intToString(int);
        static std::string floatToString(float);

    private:

        Log(void){}

        static Logger* logger;
};

#endif // LOG_H_INCLUDED
