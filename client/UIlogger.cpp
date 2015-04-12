#include <UIlogger.h>

UILogger::UILogger(consoleWidget* c)
{
    connect(this, SIGNAL(consoleWrite(QString)), c, SLOT(write(QString)));
    connect(this, SIGNAL(consoleWriteLine(QString)), c, SLOT(writeLine(QString)));
}

void UILogger::write(std::string message)
{
    emit consoleWrite(QString::fromStdString(message));
}

void UILogger::writeLine(std::string message)
{
    emit consoleWriteLine(QString::fromStdString(message));
}
