#ifndef PROGRESS_H_INCLUDED
#define PROGRESS_H_INCLUDED

#include <vector>
#include <mutex>
#include "emitter.h"

struct Block;

class Progress
{
    public:

        Progress(int, int, Block*);

        void setEmitter(Emitter*);

        void lineComplete(void);
        void blockComplete(void);

    private:

        Emitter* emitter;

        int linesComplete;
        int linesTotal;

        int blocksComplete;
        int blocksTotal;

        std::mutex completeMutex;
};

#endif // PROGRESS_H_INCLUDED
