#include <mainwindow.h>
#include <qdebug.h>

window* window::instance = NULL;

window* window::getInstance(void)
{
    if(instance == NULL){
        instance = new window();
        instance->setup();
    }
    return instance;
}

window::window(void)
{
    resize(800, 500);

    createActions();
    createMenu();

    createToolbar();

    loading = false;
}

void window::setup(void)
{
    manager = new fileManager();
    runner = new Runner();
    jobManager = new JobManager(runner);

    edit = new editWidget(manager, this);

    settings = new SettingDockWidget(this, manager, jobManager);
    addDockWidget(Qt::LeftDockWidgetArea, settings);
    settings->readOnly(true);

    threadAction[0]->trigger();
    blockAction[0]->trigger();

    setCentralWidget(edit);
}

void showMsg(QString msg)
{
    QMessageBox mess;
    mess.setText(msg);
    mess.exec();
}

void window::fileEdited(void)
{
    if(!loading){
        manager->edited();
        QString text = currentFile->currentText();
        if(text.mid(text.length() - 1, 1) != "*")
            currentFile->setItemText(currentFile->currentIndex(), currentFile->currentText() + "*");
    }
}

void window::newScene(void)
{
    int newFileID = manager->createFile();
    QString fileName = manager->getFileName(newFileID);
    currentFile->addItem(fileName, newFileID);

    int index = currentFile->findData(newFileID);
    if(index != -1)
        currentFile->setCurrentIndex(index);

    currentFile->setItemText(index, fileName + "*");
    manager->edited();

    settings->readOnly(false);
}

void window::openScene(void)
{
    int fileID = manager->openFile(this);
    if(fileID == -1)
        return;

    QString fileName = manager->getFileName(fileID);
    currentFile->addItem(fileName, fileID);
    int index = currentFile->findData(fileID);
    currentFile->setCurrentIndex(index);

    settings->readOnly(false);
}

bool window::saveScene(void)
{
    settings->write();
    edit->write();
    bool saved = manager->saveFile();
    if(saved){
        int currentID = currentFile->itemData(currentFile->currentIndex()).toInt();
        QString newFilename = manager->getFileName(currentID);

        QString currentText = currentFile->currentText();
        QString newText = currentText.mid(0, currentText.length() - 1);
        currentFile->setItemText(currentFile->currentIndex(), newText);
        if(currentFile->currentText() != newFilename){
            currentFile->setItemText(currentFile->currentIndex(), newFilename);
        }
    }
    return saved;
}

void window::closeScene(void)
{
    if(manager->isEdited()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Save file", "QUIT",
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(reply == QMessageBox::Cancel)
            return;
        if(reply == QMessageBox::Yes){
            if(!saveScene())
                return;
        }
    }
    int nextFileID = manager->closeFile();
    int index = currentFile->currentIndex();

    if(nextFileID != -1)
        currentFile->setCurrentIndex(currentFile->findData(nextFileID));
    else
        settings->readOnly(true);
    currentFile->removeItem(index);
}

void window::quit(void)
{
    showMsg("quit");
}

void window::copy(void)
{
    showMsg("copy");
}

void window::paste(void)
{
    showMsg("paste");
}

void window::thread(QAction* action)
{
    for(int i = 0; i < 6; i++)
        threadAction[i]->setChecked(false);
    action->setChecked(true);

    QString threads = action->text();
    if(threads == "auto")
        runner->setThreads(4);
    else
        runner->setThreads(threads.toInt());
}

void window::block(QAction* action)
{
    for(int i = 0; i < 7; i++)
        blockAction[i]->setChecked(false);
    action->setChecked(true);

    QString blocks = action->text();
    runner->setBlocks(blocks.toInt());
}

void window::renderScene(void)
{
    string data = manager->getData();
    if(data == "")
        return;
    int id = currentFile->itemData(currentFile->currentIndex()).toInt();
    jobManager->addJob(manager->getName(id), data);
}

void window::abortRender(void)
{
    showMsg("abort");
}

void window::about(void)
{
    showMsg("about");
}

void window::sceneDescription(void)
{
    showMsg("description");
}

void window::changeFile(int val)
{
    loading = true;
    settings->write();
    edit->write();
    manager->setCurrentFile(currentFile->itemData(val).toInt());
    settings->read();
    edit->read();
    loading = false;
}

void window::createActions(void)
{
    newAction = new QAction(QIcon("icons/new.png"), "New", this);
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newScene()));

    openAction = new QAction(QIcon("icons/open.png"), "Open", this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), this, SLOT(openScene()));

    saveAction = new QAction(QIcon("icons/save.png"), "Save", this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveScene()));

    closeAction = new QAction("Close File", this);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(closeScene()));

    quitAction = new QAction("Quit", this);
    quitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

    copyAction = new QAction(QIcon("icons/copy.png"), "Copy", this);
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(QIcon("icons/paste.png"), "Paste", this);
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    QString threadTitles[] = {"1", "2", "3", "4", "8", "auto"};
    for(int i = 0; i < 6; i++){
        threadAction[i] = new QAction(threadTitles[i], this);
        threadAction[i]->setCheckable(true);
    }

    QString blockTitles[] = {"1", "4", "8", "16", "32", "64", "128"};
    for(int i = 0; i < 7; i++){
        blockAction[i] = new QAction(blockTitles[i], this);
        blockAction[i]->setCheckable(true);
    }

    renderAction = new QAction(QIcon("icons/play.png"), "Render", this);
    connect(renderAction, SIGNAL(triggered()), this, SLOT(renderScene()));

    aboutAction = new QAction("About", this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    descriptionAction = new QAction("Scene Description", this);
    connect(descriptionAction, SIGNAL(triggered()), this, SLOT(sceneDescription()));
}

void window::createMenu(void)
{
    fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    editMenu = menuBar()->addMenu("Edit");
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);

    renderMenu = menuBar()->addMenu("Render");

    threadMenu = renderMenu->addMenu("Threads");
    for(int i = 0; i < 6; i++)
        threadMenu->addAction(threadAction[i]);
    connect(threadMenu, SIGNAL(triggered(QAction*)), this, SLOT(thread(QAction*)));

    blockMenu = renderMenu->addMenu("Blocks");
    for(int i = 0; i < 7; i++)
        blockMenu->addAction(blockAction[i]);
    connect(blockMenu, SIGNAL(triggered(QAction*)), this, SLOT(block(QAction*)));

    renderMenu->addAction(renderAction);

    helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(descriptionAction);
}

void window::createToolbar(void)
{
    toolBar = addToolBar("main");
    toolBar->setMovable(false);

    toolBar->addAction(newAction);
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addSeparator();
    toolBar->addAction(copyAction);
    toolBar->addAction(pasteAction);
    toolBar->addSeparator();
    toolBar->addAction(renderAction);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    currentFile = new QComboBox();
    currentFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    currentFile->setMaximumSize(QSize(150, 25));
    toolBar->addWidget(currentFile);
    connect(currentFile, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFile(int)));
}

