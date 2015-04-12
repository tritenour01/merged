#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QScrollArea>

#include <fileManager.h>

class valueContainer;
class antiContainer;
class renderModeContainer;

class Header : public QWidget
{
    public:
        Header(QString);
};

class PropertiesWidget : public QWidget
{
    public:
        PropertiesWidget(fileManager*);

        enum PROPERTIES {IMAGESIZE, GAMMA, AMBIENT, BACKGROUND,
                         RECURSIVE, ANTIALIASING, POSITION, LOOKAT, UP};

        void readOnly(bool);

        void write(void);
        void read(void);

    private:

        fileManager* manager;

        QScrollArea* area;

        valueContainer* imageSize;
        valueContainer* gamma;
        valueContainer* background;
        valueContainer* recursion;
        antiContainer* antialiasing;
        renderModeContainer* mode;
        valueContainer* pos;
        valueContainer* look;
        valueContainer* up;
};

#endif // PROPERTIESWIDGET_H
