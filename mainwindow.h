#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QProgressBar>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <QIcon>
#include <QMessageBox>

#include <previewWidget.h>
#include <propertiesWidget.h>
#include <editWidget.h>
#include <consoleWidget.h>
#include <fileManager.h>
#include <runner.h>
#include <networkingDialog.h>
#include <UIlogger.h>

class window : public QMainWindow
{
    Q_OBJECT

    public:
        static window* getInstance(void);

    public slots:
        void fileEdited();
        void setImage(QImage*);
        void imageChanged();
        void renderDone();

    private slots:
        void newScene();
        void openScene();
        void saveScene();
        void quit();
        void copy();
        void paste();
        void thread(QAction*);
        void block(QAction*);
        void networking();
        void renderScene();
        void about();
        void sceneDescription();

        void changeFile(QString);
        void changeView(int);

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
        QAction* quitAction;
        QAction* copyAction;
        QAction* pasteAction;
        QAction* threadAction[6];
        QAction* blockAction[7];
        QAction* networkAction;
        QAction* renderAction;
        QAction* aboutAction;
        QAction* descriptionAction;

        QComboBox* view;
        QProgressBar* progress;
        QComboBox* currentFile;

        QToolBar* toolBar;

        PropertiesWidget* properties;
        editWidget* edit;
        consoleWidget* console;
        previewWidget* preview;

        fileManager* manager;

        Runner* runner;

        NetworkingDialog* networkDialog;

        UILogger* logger;

        int numThreads;
        int numBlocks;

        void createActions(void);
        void createMenu(void);
        void createToolbar(void);
        void createStatusbar(void);

        void setView(int);
};

#endif // MAINWINDOW_H
