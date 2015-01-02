#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

#include "image.h"
#include "raytracer.h"
#include "log.h"

#include <thread>
#include <mutex>
#include <condition_variable>

class Manager
{
    public:

        Manager(int, int, Image*, Raytracer*);
        ~Manager(void);
        void Render(void);

    private:

        int numThreads;
        vector<thread> threads;

        int threadsActive;
        mutex active;

        Image* img;
        Raytracer* raytracer;

        struct Block{
            int initX;
            int initY;
            int width;
            int height;
        };
        int currentBlock;
        int numBlocks;
        Block* blocks;
        mutex blockMutex;
        mutex condMutex;
        condition_variable renderDone;
        int nextBlock(void);

        void basicRender(void);
        void threadedRender(int);

        int blocksComplete;
        mutex completeMutex;
};

#endif // MANAGER_H_INCLUDED
