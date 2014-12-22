#include <consoleWidget.h>
#include <QMessageBox>

consoleWidget::consoleWidget(QString name, QMainWindow* w) :
    QDockWidget(name, w)
{
    setVisible(false);
    logs = new QTextEdit();
    logs->setLineWrapMode(QTextEdit::NoWrap);

    setWidget(logs);
    setAllowedAreas(Qt::BottomDockWidgetArea);
    w->addDockWidget(Qt::BottomDockWidgetArea, this);
}

void consoleWidget::write(std::string message)
{
    int pos = message.find('\r');
    if(pos >= 0)
    {
        logs->append("TESTSEGSDGKSNDGKSN");
    }
    else
    {
        logs->moveCursor(QTextCursor::End);
        logs->textCursor().insertText(QString::fromStdString(message));
    }
}

void consoleWidget::writeLine(std::string message)
{
    logs->append(QString::fromStdString(message));
}
