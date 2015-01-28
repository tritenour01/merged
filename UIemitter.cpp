#include <UIemitter.h>
#include <mainwindow.h>

UIemitter::UIemitter(void)
{
    QObject::connect(this, SIGNAL(updateProgress(int)), window::getInstance(), SLOT(renderProgress(int)));
}

void UIemitter::lineComplete(int complete, int total)
{
    int progress = (int)(((float)complete / (float)total) * 100.0f);
    emit updateProgress(progress);
}

void UIemitter::blockComplete(int complete, int total)
{
    int progress = (int)(((float)complete / (float)total) * 100.0f);
    emit updateProgress(progress);
}
