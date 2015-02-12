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
#include <UIemitter.h>

#include <raytracer.h>
#include <UIemitter.h>

class JobManager;

using namespace std;

class Worker : public QObject
{
    Q_OBJECT

    public:

        Worker(string, int, int, UIemitter*);
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
        UIemitter* emitter;

        string filePath;
        int threads;
        int blocks;
};

class Runner : public QObject
{
    Q_OBJECT

    public:

        Runner(void);
        void runRenderer(string, UIemitter*);

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

        UIemitter* emitter;
        JobManager* manager;

        int threads;
        int blocks;
};

#endif // RUNNER_H
