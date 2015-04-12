#include <UIprogressEvent.h>
#include <mainwindow.h>

void UIprogressEvent::lineComplete(int complete, int total)
{
    int progress = (int)(((float)complete / (float)total) * 100.0f);
    emit updateProgress(progress);
}

void UIprogressEvent::blockComplete(int complete, int total)
{
    int progress = (int)(((float)complete / (float)total) * 100.0f);
    emit updateProgress(progress);
}
