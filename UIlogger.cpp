#include <UIlogger.h>

UILogger::UILogger(consoleWidget* c)
{
    console = c;
}

void UILogger::write(std::string message)
{
    console->write(message);
}

void UILogger::writeLine(std::string message)
{
    console->writeLine(message);
}
