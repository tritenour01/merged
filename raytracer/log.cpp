#include "log.h"
#include <sstream>

Logger* Log::logger = NULL;

void Log::write(std::string message)
{
    if(Log::logger)
        Log::logger->write(message);
}

void Log::writeLine(std::string message)
{
    if(Log::logger)
        Log::logger->writeLine(message);
}

void Log::setLogger(Logger* logger)
{
    Log::logger = logger;
}

std::string Log::intToString(int value)
{
    std::stringstream ss;
    ss<<value;
    return ss.str();
}

std::string Log::floatToString(float value)
{
    std::stringstream ss;
    ss<<value;
    return ss.str();
}
