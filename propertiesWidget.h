#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QMainWindow>
#include <QDockWidget>
#include <QScrollArea>
#include <QComboBox>

#include <fileManager.h>

class valueContainer;
class antiContainer;

class Header : public QWidget
{
    public:
        Header(QString);
};

class PropertiesWidget : public QDockWidget
{
    public:
        PropertiesWidget(fileManager*, QString, QMainWindow*);

        enum PROPERTIES {IMAGESIZE, AMBIENT, BACKGROUND, RECURSIVE,
                         ANTIALIASING, POSITION, LOOKAT, UP};

        void readOnly(bool);

        void write(void);
        void read(void);

    private:

        fileManager* manager;

        QScrollArea* area;

        valueContainer* imageSize;
        valueContainer* ambient;
        valueContainer* background;
        valueContainer* recursion;
        antiContainer* antialiasing;
        valueContainer* pos;
        valueContainer* look;
        valueContainer* up;
};

#endif // PROPERTIESWIDGET_H
