#ifndef RENDERJOB_H_INCLUDED
#define RENDERJOB_H_INCLUDED

#include <QString>
#include <QImage>
#include <previewWindow.h>
#include <UIemitter.h>

class RenderJob : public QObject
{
    Q_OBJECT

    public:

        enum STATUS {Waiting, Rendering, Complete, Invalid, Aborted};

        RenderJob(QString, std::string, int, int);

        int getID(void);
        QString getName(void);
        int getNum(void);
        RenderJob::STATUS getStatus(void);
        int getProgress(void);
        UIemitter* getEmitter(void);
        std::string getData(void);

        void Render(void);
        void Error(void);
        void Done(void);

        void setImage(UIimage*);

        void showViewer(void);

        static QString statusToString(RenderJob::STATUS);

    signals:

        void statusChanged(RenderJob::STATUS);
        void progressChanged(int);

    private slots:

        void updateProgress(int);

    private:

        int jobID;
        QString jobName;
        int nameNum;
        STATUS status;
        UIemitter* emitter;
        int progress;
        //render time
        UIimage* image;
        PreviewWindow* preview;
        std::string jobData;
};

#endif // RENDERJOB_H_INCLUDED
