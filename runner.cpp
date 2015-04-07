#include <runner.h>
#include <mainwindow.h>
#include <QDebug>

Worker::Worker(string path, int t, int b, UIprogressEvent* e)
{
    manager = NULL;

    interrupted = false;

    image = NULL;
    handler = e;

    filePath = path;
    threads = t;
    blocks = b;
}

Worker::~Worker(void)
{
    delete manager;
}

void Worker::interrupt(void)
{
    interrupted = true;
    if(manager)
        manager->interrupt();
}

void Worker::Render(void)
{
    Raytracer R;
    if(!R.loadScene(filePath)){
        emit renderInvalid();
        return;
    }

    image = new QImage(R.getWidth(), R.getHeight(), QImage::Format_ARGB32);

    img = new UIimage("image", R.getWidth(), R.getHeight(), image->bits());
    emit imageReady(img);

    manager = new Manager(threads, blocks, img, &R);
    manager->setEventHandler(handler);

    if(!interrupted)
        manager->Render();

    image->save("image.png", "PNG");

    if(interrupted)
        emit renderInterrupted();
    else
        emit renderComplete();
}

Runner::Runner(void)
{
    handler = NULL;

    threads = 1;
    blocks = 1;
}

void Runner::runRenderer(string sceneData, UIprogressEvent* e)
{
    QString cd = QDir::currentPath();
    string path = cd.toLocal8Bit().constData();
    path += "/.temp.scene";
    ofstream file(path.c_str());
    file<<sceneData;

    handler = e;

    currentWorker = new Worker(path, threads, blocks, handler);
    QThread* thread = new QThread();
    currentWorker->moveToThread(thread);

    connect(thread, SIGNAL(started()), currentWorker, SLOT(Render()));
    connect(currentWorker, SIGNAL(imageReady(UIimage*)), this, SLOT(setImage(UIimage*)));
    connect(currentWorker, SIGNAL(renderComplete()), this, SLOT(done()));
    connect(currentWorker, SIGNAL(renderInvalid()), this, SLOT(invalid()));
    connect(currentWorker, SIGNAL(renderInterrupted()), this, SLOT(interrupted()));

    thread->start();
}

void Runner::killRender(void)
{
    currentWorker->interrupt();
}

void Runner::setManager(JobManager* m)
{
    manager = m;
}

void Runner::setThreads(int t)
{
    threads = t;
}

void Runner::setBlocks(int b)
{
    blocks = b;
}

void Runner::setImage(UIimage* i)
{
    emit imageReady(i);
}

void Runner::done(void)
{
    emit renderComplete();
}

void Runner::invalid(void)
{
    emit renderInvalid();
}

void Runner::interrupted(void)
{
    emit renderInterrupted();
}
