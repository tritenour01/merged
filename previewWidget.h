#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>

class previewWidget : public QWidget
{
    Q_OBJECT

    public:
        previewWidget(QMainWindow*);
        void paintEvent(QPaintEvent*);

        void setImage(QImage*);
        void imageChanged(void);
        void renderComplete(void);

    private slots:

        void timerFinished(void);

    private:

        QImage* image;

        QTimer* timer;
        bool updateNeeded;

        int minTimerInterval;
        int maxTimerInterval;
        int currentTimerInterval;
};

#endif // PREVIEWWIDGET_H
