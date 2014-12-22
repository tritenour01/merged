#include <previewWidget.h>
#include <qdebug.h>

previewWidget::previewWidget(QMainWindow* mw)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    image = NULL;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerFinished()));

    minTimerInterval = 100;
    maxTimerInterval = 5000;

    currentTimerInterval = minTimerInterval;
}

void previewWidget::timerFinished(void)
{
    emit update();

    if(updateNeeded)
        currentTimerInterval /= 2;
    else
        currentTimerInterval *= 2;

    updateNeeded = false;

    if(currentTimerInterval < minTimerInterval)
        currentTimerInterval = minTimerInterval;
    else if(currentTimerInterval > maxTimerInterval)
        currentTimerInterval = maxTimerInterval;

    qDebug()<<currentTimerInterval;

    timer->start(currentTimerInterval);
}

void previewWidget::paintEvent(QPaintEvent* event)
{
    if(image == NULL)
        return;

    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing, false);

    QImage& i = *image;

    QRect imageRect = image->rect();
    QRect screenRect = rect();

    float scale;
    int xOffset = 0;
    int yOffset = 0;

    if(imageRect.width() / imageRect.height() > screenRect.width() / screenRect.height()){
        scale = (float)screenRect.width() / (float)imageRect.width();
        yOffset = (float)(screenRect.height() - (float)imageRect.height() * scale) * 0.5;
    }
    else{
        scale = (float)screenRect.height() / (float)imageRect.height();
        xOffset = (float)(screenRect.width() - (float)imageRect.width() * scale) * 0.5;
    }

    QRect result(xOffset, yOffset, (float)imageRect.width() * scale, (float)imageRect.height() * scale);

    paint.drawImage(result, i, imageRect);
}

void previewWidget::setImage(QImage* i)
{
    image = i;
    timer->start(currentTimerInterval);
}

void previewWidget::imageChanged(void)
{
    updateNeeded = true;
}

void previewWidget::renderComplete(void)
{
    timer->stop();
    emit update();
}
