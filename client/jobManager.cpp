#include <jobManager.h>
#include <jobWidget.h>

JobManager::JobManager(Runner* r)
{
    runner = r;
    widget = NULL;
    currentJob = NULL;
    busy = false;

    connect(runner, SIGNAL(imageReady(UIimage*)), this, SLOT(setImage(UIimage*)));
    connect(runner, SIGNAL(renderComplete()), this, SLOT(jobDone()));
    connect(runner, SIGNAL(renderInvalid()), this, SLOT(jobInvalid()));
    connect(runner, SIGNAL(renderInterrupted()), this, SLOT(jobInterrupted()));
}

void JobManager::addJob(QString sceneName, string data)
{
    int newID = 0;
    int newNum = 0;
    for(int i = 0; i < jobs.size(); i++){
        RenderJob* current = jobs[i];
        if(current->getID() >= newID)
            newID = current->getID() + 1;
        if(sceneName == current->getName()){
            if(current->getNum() >= newNum)
                newNum = current->getNum() + 1;
        }
    }
    RenderJob* newJob = new RenderJob(sceneName, data, newNum, newID);
    jobs.push_back(newJob);

    widget->addEntry(newJob);

    runJob();
}

void JobManager::runJob(void)
{
    if(busy)
        return;

    RenderJob* job = NULL;
    for(int i = 0; i < jobs.size(); i++){
        if(jobs[i]->getStatus() == RenderJob::Waiting){
            job = jobs[i];
            break;
        }
    }

    if(job == NULL)
        return;

    busy = true;

    currentJob = job;
    currentJob->Render();
    runner->runRenderer(currentJob->getData(), currentJob->getHandler());
}

void JobManager::jobDone(void)
{
    currentJob->Done();
    currentJob = NULL;
    busy = false;

    runJob();
}

void JobManager::jobInvalid(void)
{
    currentJob->Error();
    currentJob = NULL;
    busy = false;

    runJob();
}

void JobManager::jobInterrupted(void)
{
    currentJob->Interrupted();
    currentJob = NULL;
    busy = false;

    runJob();
}

void JobManager::showViewer(int id)
{
    RenderJob* job = NULL;
    for(int i = 0; i < jobs.size(); i++){
        if(jobs[i]->getID() == id){
            job = jobs[i];
            break;
        }
    }

    job->showViewer();
}

void JobManager::killJob(int id)
{
    runner->killRender();
}

void JobManager::setImage(UIimage* i)
{
    currentJob->setImage(i);
    widget->imageViewable(currentJob->getID());
}

void JobManager::setWidget(JobWidget* w)
{
    widget = w;
}
