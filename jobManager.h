#ifndef JOBMANAGER_H_INCLUDED
#define JOBMANAGER_H_INCLUDED

#include <runner.h>
#include <renderJob.h>
#include <vector>

class JobWidget;

class JobManager : public QObject
{
    Q_OBJECT

    public:

        JobManager(Runner*);

        void addJob(QString, string);
        void showViewer(int);

        void setWidget(JobWidget*);

    private slots:

        void jobDone(void);
        void jobInvalid(void);

        void setImage(UIimage*);

    private:

        vector<RenderJob*> jobs;

        Runner* runner;
        JobWidget* widget;

        RenderJob* currentJob;

        bool busy;

        void runJob(void);
};

#endif // JOBMANAGER_H_INCLUDED
