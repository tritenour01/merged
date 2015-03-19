#ifndef RUNNER_H
#define RUNNER_H

#include <iostream>
#include <fstream>

#include <QDir>
#include <QThread>
#include <QObject>
#include <QImage>
#include <UIimage.h>
#include <manager.h>
#include <UIprogressEvent.h>

#include <raytracer.h>
#include <UIprogressEvent.h>

class JobManager;

using namespace std;

class Worker : public QObject
{
    Q_OBJECT

    public:

        Worker(string, int, int, UIprogressEvent*);
        ~Worker();

    public slots:

        void Render(void);

    signals:

        void imageReady(UIimage*);
        void renderComplete(void);
        void renderInvalid(void);

    private:

        QImage* image;
        UIimage* img;
        UIprogressEvent* handler;

        string filePath;
        int threads;
        int blocks;
};

class Runner : public QObject
{
    Q_OBJECT

    public:

        Runner(void);
        void runRenderer(string, UIprogressEvent*);

        void setThreads(int);
        void setBlocks(int);

        void setManager(JobManager*);

    private slots:

        void setImage(UIimage*);
        void done(void);
        void invalid(void);

    signals:

        void imageReady(UIimage*);
        void renderComplete(void);
        void renderInvalid(void);

    private:

        UIprogressEvent* handler;
        JobManager* manager;

        int threads;
        int blocks;
};

#endif // RUNNER_H
