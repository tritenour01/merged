#ifndef UIEMITTER_H_INCLUDED
#define UIEMITTER_H_INCLUDED

#include <QObject>
#include <emitter.h>

class UIemitter : public QObject, public Emitter
{
    Q_OBJECT

    public:

        UIemitter();

        void lineComplete(int, int);

        void blockLineComplete(int);

        void blockComplete(int, int);

    signals:

        void updateProgress(int);
};

#endif // UIEMITTER_H_INCLUDED
