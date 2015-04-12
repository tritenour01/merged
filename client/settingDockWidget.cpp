#include <settingDockWidget.h>

SettingDockWidget::SettingDockWidget(QMainWindow* parent, fileManager* m, JobManager* j) :
    QDockWidget("Settings", parent)
{
    tabs = new QTabWidget(this);
    setWidget(tabs);

    properties = new PropertiesWidget(m);
    properties->layout()->setContentsMargins(0, 0, 0, 0);
    tabs->addTab(properties, "Settings");

    job = new JobWidget(j);
    tabs->addTab(job, "Jobs");

    console = new consoleWidget();
    console->layout()->setContentsMargins(0, 0, 0, 0);
    tabs->addTab(console, "Log");
}

void SettingDockWidget::readOnly(bool b)
{
    properties->readOnly(b);
}

void SettingDockWidget::read(void)
{
    properties->read();
}

void SettingDockWidget::write(void)
{
    properties->write();
}
