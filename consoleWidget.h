#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDockWidget>

class consoleWidget : public QDockWidget
{
    public:

        consoleWidget(QString, QMainWindow*);
        void write(std::string);
        void writeLine(std::string);

    private:

        QTextEdit* logs;
};

#endif // CONSOLEWIDGET_H
