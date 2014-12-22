#ifndef UILOGGER_H_INCLUDED
#define UILOGGER_H_INCLUDED

#include <logger.h>
#include <consoleWidget.h>

class UILogger : public Logger
{
    public:

        UILogger(consoleWidget*);

        void write(std::string);
        void writeLine(std::string);

    private:

        consoleWidget* console;
};

#endif // UILOGGER_H_INCLUDED
