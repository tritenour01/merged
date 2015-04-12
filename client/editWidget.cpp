#include <editWidget.h>
#include <mainwindow.h>

editWidget::editWidget(fileManager* m, QMainWindow* mw)
{
    manager = m;

    QVBoxLayout* layout = new QVBoxLayout;
    editArea = new QTextEdit();
    connect(editArea, SIGNAL(textChanged()), window::getInstance(), SLOT(fileEdited()));
    layout->addWidget(editArea);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

void editWidget::read(void)
{
    QString text;
    manager->read(fileManager::TEXT, (void*)&text);
    editArea->setText(text);
}

void editWidget::write(void)
{
    QString text = editArea->toPlainText();
    manager->write(fileManager::TEXT, (void*)&text);
}
