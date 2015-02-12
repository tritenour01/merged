#ifndef EMITTER_H_INCLUDED
#define EMITTER_H_INCLUDED

class Emitter
{
    public:

        virtual void lineComplete(int, int) =0;
        virtual void blockComplete(int, int) =0;
};

#endif // EMITTER_H_INCLUDED
