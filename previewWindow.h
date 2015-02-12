#ifndef PREVIEWWINDOW_H_INCLUDED
#define PREVIEWWINDOW_H_INCLUDED

#include <QMainWindow>
#include <QCloseEvent>
#include <QProgressBar>
#include <QStatusBar>
#include <previewWidget.h>
#include <UIImage.h>
#include <QToolButton>

class PreviewWindow : public QMainWindow
{
    Q_OBJECT

    public:

        PreviewWindow(UIimage*);

        void renderComplete(void);

        void renderProgress(int);

        void closeEvent(QCloseEvent*);

    private slots:

        void imageChanged(void);
        void save(void);

    private:

        previewWidget* preview;
        QProgressBar* progress;
        QToolButton* saveButton;
};

#endif // PREVIEWWINDOW_H_INCLUDED
