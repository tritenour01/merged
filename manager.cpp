#include "manager.h"

Manager::Manager(int num, int blockSetup, Image* i, Raytracer* r)
{
    threads.clear();
    numThreads = num;
    img = i;
    raytracer = r;

    numBlocks = blockSetup * blockSetup;
    currentBlock = 0;
    blocksComplete = 0;
    blocks = new Block[numBlocks];
    int x = 0;
    int y = 0;
    int xOffset = (int)((float)raytracer->getWidth() / (float)blockSetup);
    int yOffset = (int)((float)raytracer->getHeight() / (float)blockSetup);
    int index = 0;
    for(int i = 0; i < blockSetup; i++){
        x = 0;
        for(int j = 0; j < blockSetup; j++){
            blocks[index].width = xOffset;
            blocks[index].height = yOffset;
            blocks[index].initX = x;
            blocks[index].initY = y;
            if(i == blockSetup - 1)
                blocks[index].height = raytracer->getHeight() - y;
            if(j == blockSetup - 1)
                blocks[index].width = raytracer->getWidth() - x;
            index++;
            x += xOffset;
        }
        y += yOffset;
    }
}

Manager::~Manager(void)
{
    if(numThreads > 1){
        for(int i = 0; i < numThreads; i++)
            threads[i].join();
    }
}

void Manager::Render(void)
{
    if(numThreads == 1)
        basicRender();
    else{
        Log::write("0/" + Log::intToString(numBlocks) + " blocks            \r");
        for(int i = 0; i < numThreads; i++)
            threads.push_back(thread(threadedRender, this, i));
        threadsActive = numThreads;
        unique_lock<mutex> l(condMutex);
        renderDone.wait(l);
    }
}

void Manager::basicRender(void)
{
    for(int i = 0; i < raytracer->getHeight(); i++){
        Log::write("\r" + Log::intToString(i+1) + "/" + Log::intToString(raytracer->getHeight()) + " rows         ");
        for(int j = 0; j < raytracer->getWidth(); j++){
            Vector3 color = raytracer->tracePixel(j, i);
            img->setPixel(j, i, color);
        }
    }
}

void Manager::threadedRender(int id)
{
    Block* current = &blocks[0];
    while(current){
        int newBlock = nextBlock();
        if(newBlock == -1){
            current = NULL;
            continue;
        }
        current = &blocks[newBlock];
        for(int i = 0; i < current->height; i++){
            for(int j = 0; j < current->width; j++){
                Vector3 color = raytracer->tracePixel(current->initX + j, current->initY + i);
                img->setPixel(current->initX + j, current->initY + i, color);
            }
        }
        completeMutex.lock();
        blocksComplete++;
        Log::write(Log::intToString(blocksComplete) + "/" + Log::intToString(numBlocks) + " blocks            \r");
        completeMutex.unlock();
    }

    active.lock();
    threadsActive--;
    if(threadsActive == 0)
        renderDone.notify_one();
    active.unlock();
}

int Manager::nextBlock(void)
{
    blockMutex.lock();
    int value = currentBlock;
    currentBlock++;
    if(value >= numBlocks)
        value = -1;
    blockMutex.unlock();
    return value;
}
