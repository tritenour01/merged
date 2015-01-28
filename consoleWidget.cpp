#include <consoleWidget.h>
#include <UIlogger.h>


consoleWidget::consoleWidget(void)
{
    logs = new QPlainTextEdit();
    logs->setReadOnly(true);

    QVBoxLayout* l = new QVBoxLayout;
    l->addWidget(logs);
    setLayout(l);

    logger = new UILogger(this);
    Log::setLogger(logger);
}

void consoleWidget::write(QString message)
{
    logs->appendPlainText(message);
}

void consoleWidget::writeLine(QString message)
{
    logs->appendPlainText(message);
}
