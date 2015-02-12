#include <previewWindow.h>
#include <mainwindow.h>

PreviewWindow::PreviewWindow(UIimage* i) :
    QMainWindow(window::getInstance())
{
    setWindowTitle("Render Viewer");
    setMinimumSize(500, 500);

    preview = new previewWidget(this);
    setCentralWidget(preview);

    progress = new QProgressBar(this);
    progress->setTextVisible(false);

    statusBar()->addWidget(progress, 1);

    saveButton = new QToolButton(this);
    QIcon* saveIcon = new QIcon("icons/save.png");
    saveButton->setIcon(*saveIcon);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

    statusBar()->addWidget(saveButton);

    preview->setImage(i);
    connect(i, SIGNAL(changed()), this, SLOT(imageChanged()));
}

void PreviewWindow::imageChanged(void)
{
    preview->imageChanged();
}

void PreviewWindow::renderComplete(void)
{
    preview->renderComplete();
}

void PreviewWindow::renderProgress(int val)
{
    progress->setValue(val);
}

void PreviewWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();
    hide();
}

void PreviewWindow::save(void)
{
    QString filePath = QFileDialog::getSaveFileName(window::getInstance(), QObject::tr("Save Image"), "", QObject::tr("PNG Files (*.png)"));
    if(filePath == "")
        return;
    preview->save(filePath);
}
