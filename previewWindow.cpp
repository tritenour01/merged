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
