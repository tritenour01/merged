#ifndef PROGRESS_EVENT_H_INCLUDED
#define PROGRESS_EVENT_H_INCLUDED

class ProgressEvent
{
    public:

        virtual void lineComplete(int, int) =0;
        virtual void blockComplete(int, int) =0;
};

#endif // PROGRESS_EVENT_H_INCLUDED
