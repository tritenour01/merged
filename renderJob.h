#ifndef RENDERJOB_H_INCLUDED
#define RENDERJOB_H_INCLUDED

#include <QString>
#include <QImage>
#include <previewWindow.h>

class RenderJob
{
    public:

        enum STATUS {Waiting, Rendering, Complete, Cancelled};

        RenderJob(QString, std::string, int, int);

        int getID(void);
        QString getName(void);
        int getNum(void);
        RenderJob::STATUS getStatus(void);
        int getProgress(void);
        std::string getData(void);

        void setStatus(RenderJob::STATUS);
        void setImage(UIimage*);

        void showViewer(void);

        static QString statusToString(RenderJob::STATUS);

    private:

        int jobID;
        QString jobName;
        int nameNum;
        STATUS status;
        int progress;
        //render time
        UIimage* image;
        PreviewWindow* preview;
        std::string jobData;
};

#endif // RENDERJOB_H_INCLUDED
