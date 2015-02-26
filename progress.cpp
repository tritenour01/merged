#include "progress.h"
#include "manager.h"

Progress::Progress(int threads, int blockSetup, Block* blocks)
{
    linesComplete = 0;
    if(blockSetup == 1)
        linesTotal = blocks[0].height;
    else{
        linesTotal = 0;
        for(int i = 0; i < blockSetup * blockSetup; i++){
            if(i % blockSetup == 0)
                linesTotal += blocks[i].height;
        }
    }

    blocksComplete = 0;
    blocksTotal = blockSetup * blockSetup;

    handler = NULL;
}

void Progress::setEventHandler(ProgressEvent* e)
{
    handler = e;
}

void Progress::lineComplete(void)
{
    linesComplete++;
    if(handler)
        handler->lineComplete(linesComplete, linesTotal);
}

void Progress::blockComplete(void)
{
    completeMutex.lock();
    blocksComplete++;
    if(handler)
        handler->blockComplete(blocksComplete, blocksTotal);
    completeMutex.unlock();
}
