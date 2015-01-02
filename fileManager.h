#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include <string>

#include <QString>
#include <QFileDialog>
#include <QObject>
#include <QMainWindow>
#include <QMessageBox>

#include <loader.h>

using namespace std;

struct fileObject;

class fileManager
{
    public:

        enum FIELDS {SCENE, CAMERA, TEXT};

        fileManager(void);
        int createFile(void);
        int openFile(QMainWindow*);
        bool saveFile(void);

        void closeFile(void);

        string getData(void);
        QString getFileName(int);

        void write(FIELDS, void*);
        void read(FIELDS, void*);

        void edited(void);

        void setCurrentFile(int);

    private:

        std::vector<fileObject*>* files;
        fileObject* currentFile;
};

#endif // FILEMANAGER_H
