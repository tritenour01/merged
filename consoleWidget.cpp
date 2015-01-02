#include <consoleWidget.h>

consoleWidget::consoleWidget(QString name, QMainWindow* w) :
    QDockWidget(name, w)
{
    setFeatures(QDockWidget::NoDockWidgetFeatures);

    logs = new QTextEdit();
    logs->setLineWrapMode(QTextEdit::NoWrap);

    setWidget(logs);
    setAllowedAreas(Qt::BottomDockWidgetArea);
    w->addDockWidget(Qt::BottomDockWidgetArea, this);
}

void consoleWidget::write(std::string message)
{
    int pos = message.find('\r');

        //logs->moveCursor(QTextCursor::End);
        //logs->textCursor().insertText(QString::fromStdString(message));

}

void consoleWidget::writeLine(std::string message)
{
    //logs->append(QString::fromStdString(message));
}
