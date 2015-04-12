#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <string>

class Logger
{
    public:

        virtual void write(std::string) =0;
        virtual void writeLine(std::string) =0;
};

#endif // LOGGER_H_INCLUDED
