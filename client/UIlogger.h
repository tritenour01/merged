#ifndef UILOGGER_H_INCLUDED
#define UILOGGER_H_INCLUDED

#include <logger.h>
#include <consoleWidget.h>

#include <QObject>

class UILogger : public QObject, public Logger
{
    Q_OBJECT

    public:

        UILogger(consoleWidget*);

        void write(std::string);
        void writeLine(std::string);

    signals:

        void consoleWrite(QString);
        void consoleWriteLine(QString);
};

#endif // UILOGGER_H_INCLUDED
