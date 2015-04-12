#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>

#include <fileManager.h>

class editWidget : public QWidget
{
    public:
        editWidget(fileManager*, QMainWindow*);

        void write(void);
        void read(void);

    private:

        fileManager* manager;

        QTextEdit* editArea;
};

#endif // EDITWIDGET_H
