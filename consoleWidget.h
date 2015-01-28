#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <log.h>

class UILogger;

class consoleWidget : public QWidget
{
    Q_OBJECT

    public:

        consoleWidget(void);

    public slots:

        void write(QString);
        void writeLine(QString);

    private:

        QPlainTextEdit* logs;
        UILogger* logger;
};

#endif // CONSOLEWIDGET_H
