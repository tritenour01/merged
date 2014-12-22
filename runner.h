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

#include <raytracer.h>

using namespace std;

class Worker : public QObject
{
    Q_OBJECT

    public:

        Worker(string, int, int);
        ~Worker();

    public slots:

        void Render(void);

    signals:

        void imageReady(QImage*);
        void renderComplete(void);

    private:

        QImage* image;

        string filePath;
        int threads;
        int blocks;

};

class Runner
{
    public:

        void runRenderer(string, int, int);

    private:


};

#endif // RUNNER_H
