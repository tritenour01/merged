#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QProgressBar>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <QIcon>
#include <QMessageBox>
#include <QComboBox>

#include <editWidget.h>
#include <fileManager.h>
#include <runner.h>
#include <settingDockWidget.h>

class window : public QMainWindow
{
    Q_OBJECT

    public:
        static window* getInstance(void);

    public slots:
        void fileEdited();

    private slots:
        void newScene();
        void openScene();
        bool saveScene();
        void closeScene();
        void quit();
        void copy();
        void paste();
        void thread(QAction*);
        void block(QAction*);
        void renderScene();
        void abortRender();
        void about();
        void sceneDescription();

        void changeFile(int);

    private:
        window(void);
        void setup(void);
        static window* instance;

        bool loading;

        QMenu* fileMenu;
        QMenu* editMenu;
        QMenu* renderMenu;
        QMenu* threadMenu;
        QMenu* blockMenu;
        QMenu* helpMenu;

        QAction* newAction;
        QAction* openAction;
        QAction* saveAction;
        QAction* closeAction;
        QAction* quitAction;
        QAction* copyAction;
        QAction* pasteAction;
        QAction* threadAction[6];
        QAction* blockAction[7];
        QAction* renderAction;
        QAction* aboutAction;
        QAction* descriptionAction;

        QComboBox* currentFile;

        QToolBar* toolBar;

        editWidget* edit;

        fileManager* manager;
        JobManager* jobManager;

        Runner* runner;

        SettingDockWidget* settings;

        void createActions(void);
        void createMenu(void);
        void createToolbar(void);
};

#endif // MAINWINDOW_H
