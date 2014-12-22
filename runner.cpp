#include <runner.h>
#include <mainwindow.h>
#include <QDebug>

Worker::Worker(string path, int t, int b)
{
    image = NULL;

    filePath = path;
    threads = t;
    blocks = b;

    connect(this, SIGNAL(imageReady(QImage*)), window::getInstance(), SLOT(setImage(QImage*)));
    connect(this, SIGNAL(renderComplete()), window::getInstance(), SLOT(renderDone()));
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
    emit imageReady(image);

    UIimage img("image", R.getWidth(), R.getHeight(), image->bits());

    Manager manager(threads, blocks, &img, &R);
    manager.Render();

    image->save("image.png", "PNG");

    emit renderComplete();
}

void Runner::runRenderer(string sceneData, int threads, int blocks)
{
    QString cd = QDir::currentPath();
    string path = cd.toLocal8Bit().constData();
    path += "/.temp.scene";
    ofstream file(path.c_str());
    file<<sceneData;

    Worker* work = new Worker(path, threads, blocks);
    QThread* thread = new QThread();
    work->moveToThread(thread);

    QObject::connect(thread, SIGNAL(started()), work, SLOT(Render()));

    thread->start();
}
