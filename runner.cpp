#include <runner.h>
#include <mainwindow.h>
#include <QDebug>

Worker::Worker(string path, int t, int b, UIemitter* e)
{
    image = NULL;
    emitter = e;

    filePath = path;
    threads = t;
    blocks = b;
}

Worker::~Worker(void)
{

}

void Worker::Render(void)
{
    Raytracer R;
    if(!R.loadScene(filePath))
        return;

    image = new QImage(R.getWidth(), R.getHeight(), QImage::Format_ARGB32);

    img = new UIimage("image", R.getWidth(), R.getHeight(), image->bits());
    emit imageReady(img);

    Manager manager(threads, blocks, img, &R);
    manager.setEmitter(emitter);
    manager.Render();

    image->save("image.png", "PNG");

    emit renderComplete();
}

Runner::Runner(void)
{
    emitter = new UIemitter();

    threads = 1;
    blocks = 1;
}

void Runner::runRenderer(string sceneData)
{
    QString cd = QDir::currentPath();
    string path = cd.toLocal8Bit().constData();
    path += "/.temp.scene";
    ofstream file(path.c_str());
    file<<sceneData;

    emitter->lineComplete(0, 1);

    Worker* work = new Worker(path, threads, blocks, emitter);
    QThread* thread = new QThread();
    work->moveToThread(thread);

    connect(thread, SIGNAL(started()), work, SLOT(Render()));
    connect(work, SIGNAL(imageReady(UIimage*)), this, SLOT(setImage(UIimage*)));
    connect(work, SIGNAL(renderComplete()), this, SLOT(done()));

    thread->start();
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
