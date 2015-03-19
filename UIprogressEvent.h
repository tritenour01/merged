#ifndef UIPROGRESS_EVENT_H_INCLUDED
#define UIPROGRESS_EVENT_H_INCLUDED

#include <QObject>
#include <progressEvent.h>

class UIprogressEvent : public QObject, public ProgressEvent
{
    Q_OBJECT

    public:

        void lineComplete(int, int);

        void blockLineComplete(int);

        void blockComplete(int, int);

    signals:

        void updateProgress(int);
};

#endif // UIPROGRESS_EVENT_H_INCLUDED
