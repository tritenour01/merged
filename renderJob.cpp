#include <renderJob.h>

RenderJob::RenderJob(QString name, std::string data, int num, int id)
{
    jobID = id;
    jobName = name;
    nameNum = num;
    status = Waiting;
    progress = 0;
    emitter = new UIemitter();
    image = NULL;
    preview = NULL;
    jobData = data;

    connect(emitter, SIGNAL(updateProgress(int)), this, SLOT(updateProgress(int)));
}

int RenderJob::getID(void)
{
    return jobID;
}

QString RenderJob::getName(void)
{
    return jobName;
}

int RenderJob::getNum(void)
{
    return nameNum;
}

RenderJob::STATUS RenderJob::getStatus(void)
{
    return status;
}

int RenderJob::getProgress(void)
{
    return progress;
}

UIemitter* RenderJob::getEmitter(void)
{
    return emitter;
}

std::string RenderJob::getData(void)
{
    return jobData;
}

void RenderJob::Render(void)
{
    status = Rendering;
    emit statusChanged(status);
}

void RenderJob::Error(void)
{
    status = Invalid;
    emit statusChanged(status);
}

void RenderJob::Done(void)
{
    status = Complete;
    emit statusChanged(status);
}

void RenderJob::setImage(UIimage* i)
{
    image = i;
}

void RenderJob::showViewer(void)
{
    if(preview == NULL)
        preview = new PreviewWindow(image);
    preview->show();
}

void RenderJob::updateProgress(int p)
{
    progress = p;
    if(preview)
        preview->renderProgress(p);
    emit progressChanged(p);
}

QString RenderJob::statusToString(RenderJob::STATUS s)
{
    switch(s)
    {
        case RenderJob::Waiting:
            return "Waiting";
        case RenderJob::Rendering:
            return "Rendering";
        case RenderJob::Complete:
            return "Complete";
        case RenderJob::Invalid:
            return "Invalid";
        case RenderJob::Aborted:
            return "Cancelled";
    }
}
