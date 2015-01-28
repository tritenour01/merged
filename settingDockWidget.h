#ifndef SETTINGDOCKWIDGET_H_INCLUDED
#define SETTINGDOCKWIDGET_H_INCLUDED

#include <QMainWindow>
#include <QDockWidget>
#include <QTabWidget>
#include <QLayout>

#include <fileManager.h>
#include <propertiesWidget.h>
#include <consoleWidget.h>
#include <jobWidget.h>

class SettingDockWidget : public QDockWidget
{
    public:

        SettingDockWidget(QMainWindow*, fileManager*, JobManager*);

        void read(void);
        void write(void);

    private:

        QTabWidget* tabs;

        PropertiesWidget* properties;
        consoleWidget* console;
        JobWidget* job;
};

#endif // SETTINGDOCKWIDGET_H_INCLUDED
