#include <UIemitter.h>
#include <mainwindow.h>

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
