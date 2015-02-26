#ifndef PROGRESS_H_INCLUDED
#define PROGRESS_H_INCLUDED

#include <vector>
#include <mutex>
#include "progressEvent.h"

struct Block;

class Progress
{
    public:

        Progress(int, int, Block*);

        void setEventHandler(ProgressEvent*);

        void lineComplete(void);
        void blockComplete(void);

    private:

        ProgressEvent* handler;

        int linesComplete;
        int linesTotal;

        int blocksComplete;
        int blocksTotal;

        std::mutex completeMutex;
};

#endif // PROGRESS_H_INCLUDED
