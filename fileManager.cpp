#include <fileManager.h>
#include <dataModel.h>
#include <mainwindow.h>
#include <QDebug>

fileManager::fileManager(void)
{
    files = new std::vector<fileObject*>();
    currentFile = NULL;
}

void fileManager::edited(void)
{
    if(currentFile)
        currentFile->info.edited = true;
}

int fileManager::createFile(void)
{
    fileObject* newFile = new fileObject();

    QString newFileName = "new";
    int num = 1;
    for(int i = 0; i < files->size(); i++){
        QString fileName = files->at(i)->info.fileName;
        if(fileName.mid(0, 3) != "new")
            continue;
        QString end = fileName.mid(3, fileName.length());
        if(end == ""){
            num++;
            continue;
        }
        bool result = false;
        end.toInt(&result, 10);
        if(result)
            num++;
    }

    if(num > 1)
        newFileName.append(QString::number(num));

    newFile->fileID = files->size();
    newFile->info.fileName = newFileName;
    newFile->info.filePath = "";
    newFile->info.edited = false;
    newFile->info.saved = false;
    files->push_back(newFile);

    return newFile->fileID;
}

int fileManager::openFile(QMainWindow* window)
{
    QString filePath = QFileDialog::getOpenFileName(window, QObject::tr("Open Scene"), "", QObject::tr("Scene Files (*.scene)"));
    if(filePath == "")
        return -1;

    QFileInfo info(filePath);
    fileObject* openedFile = new fileObject();
    openedFile->fileID = files->size();
    openedFile->info.fileName = info.baseName() + ".scene";
    openedFile->info.filePath = info.path();
    openedFile->info.edited = false;
    openedFile->info.saved = true;

    std::string path = filePath.toStdString();
    Loader loader;
    bool result = loader.parseScene(path, openedFile);
    if(result == false){
        QMessageBox mess;
        mess.setText("File at " + filePath + " is not a valid scene file");
        mess.exec();
        delete openedFile;
        return -1;
    }

    files->push_back(openedFile);
    return openedFile->fileID;
}

bool fileManager::saveFile(void)
{
    if(!currentFile)
        return false;
    if(currentFile->info.edited){
        if(!currentFile->info.saved){
            QString filePath = QFileDialog::getSaveFileName(window::getInstance(), QObject::tr("Save Scene"), "", QObject::tr("Scene Files (*.scene)"));
            if(filePath == "")
                return false;
            QFileInfo info(filePath);
            currentFile->info.fileName = info.baseName() + ".scene";
            currentFile->info.filePath = info.path();
        }
        QString path = currentFile->info.filePath + "/" + currentFile->info.fileName;
        ofstream file(path.toStdString().c_str());
        string data = currentFile->toString().toLocal8Bit().constData();
        file<<data;
        file.close();
        currentFile->info.saved = true;
        currentFile->info.edited = false;
        return true;
    }
    return false;
}

void fileManager::closeFile(void)
{

}

string fileManager::getData(void)
{
    if(!currentFile)
        return "";
    return currentFile->toString().toLocal8Bit().constData();
}

QString fileManager::getFileName(int ID)
{
    for(int i = 0; i < files->size(); i++)
    {
        if(files->at(i)->fileID == ID)
            return files->at(i)->info.fileName;
    }
    return "";
}

void fileManager::setCurrentFile(int ID)
{
    for(int i = 0; i < files->size(); i++){
        QMessageBox mess;
        mess.setText(QString::number(files->at(i)->fileID));
        //mess.exec();
        if(files->at(i)->fileID == ID)
            currentFile = files->at(i);
    }
    QMessageBox mess;
    mess.setText("BAD");
    //mess.exec();
}

void fileManager::read(FIELDS field, void* returnData)
{
    if(!currentFile)
        return;
    switch(field)
    {
        case SCENE:
        {
            sceneData* scene = (sceneData*)returnData;
            *scene = currentFile->scene;
            break;
        }
        case CAMERA:
        {
            cameraData* camera = (cameraData*)returnData;
            *camera = currentFile->camera;
            break;
        }
        case TEXT:
        {
            QString* text = (QString*)returnData;
            *text = currentFile->text;
            break;
        }
    }
}

void fileManager::write(FIELDS field, void* data)
{
    if(!currentFile)
        return;
    switch(field)
    {
        case SCENE:
        {
            sceneData* scene = (sceneData*)data;
            currentFile->scene = *scene;
            break;
        }
        case CAMERA:
        {
            cameraData* camera = (cameraData*)data;
            currentFile->camera = *camera;
            break;
        }
        case TEXT:
        {
            QString* text = (QString*)data;
            currentFile->text = *text;
            break;
        }
    }
}
