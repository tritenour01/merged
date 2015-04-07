#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

#include "image.h"
#include "raytracer.h"
#include "log.h"
#include "progress.h"

#include <thread>
#include <mutex>
#include <condition_variable>

struct Block{
    int initX;
    int initY;
    int width;
    int height;
};

class Manager
{
    public:

        Manager(int, int, Image*, Raytracer*);
        ~Manager(void);

        void Render(void);

        void interrupt(void);

        void setEventHandler(ProgressEvent*);

    private:

        int numThreads;
        vector<thread> threads;

        int threadsActive;
        mutex active;

        bool interruptFlag;

        Image* img;
        Raytracer* raytracer;

        int currentBlock;
        int numBlocks;
        Block* blocks;
        mutex blockMutex;
        mutex condMutex;
        condition_variable renderDone;
        int nextBlock(void);

        void basicRender(void);
        void threadedRender(int);

        Progress* progress;
};

#endif // MANAGER_H_INCLUDED
