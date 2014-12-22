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
    createStatusbar();

    loading = false;
}

void window::setup(void)
{
    manager = new fileManager();
    runner = new Runner();

    networkDialog = new NetworkingDialog();

    properties = new PropertiesWidget(manager, "Properties", this);
    edit = new editWidget(manager, this);
    console = new consoleWidget("Output Console", this);
    preview = new previewWidget(this);

    logger = new UILogger(console);
    Log::setLogger(logger);

    setView(0);
    threadAction[0]->trigger();
    blockAction[0]->trigger();
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

void window::imageChanged(void)
{
    preview->imageChanged();
}

void window::renderDone(void)
{
    preview->renderComplete();
}

void window::newScene(void)
{
    QString newFile = manager->createFile();
    currentFile->addItem(newFile, newFile);

    int index = currentFile->findData(newFile);
    if(index != -1)
        currentFile->setCurrentIndex(index);

    currentFile->setItemText(index, newFile + "*");
    manager->edited();
}

void window::openScene(void)
{
    QString fileName = manager->openFile(this);
    if(fileName == "")
        return;

    currentFile->addItem(fileName, fileName);
    int index = currentFile->findData(fileName);
    currentFile->setCurrentIndex(index);
}

void window::saveScene(void)
{
    properties->write();
    edit->write();
    QString fileName = manager->saveFile();
    if(fileName != ""){
        QString currentText = currentFile->currentText();
        QString newText = currentText.mid(0, currentText.length() - 1);
        currentFile->setItemText(currentFile->currentIndex(), newText);
        if(currentFile->currentText() != fileName){
            currentFile->setItemText(currentFile->currentIndex(), fileName);
            currentFile->setItemData(currentFile->currentIndex(), fileName);
        }
    }
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
        numThreads = 4;
    else
        numThreads = threads.toInt();
}

void window::block(QAction* action)
{
    for(int i = 0; i < 7; i++)
        blockAction[i]->setChecked(false);
    action->setChecked(true);

    QString blocks = action->text();
    numBlocks = blocks.toInt();
}

void window::networking(void)
{
    networkDialog->show();
    networkDialog->raise();
    networkDialog->activateWindow();
}

void window::renderScene(void)
{
    string data = manager->getData();
    if(data == "")
        return;
    qDebug()<<numThreads;
    qDebug()<<numBlocks;
    runner->runRenderer(data, numThreads, numBlocks);
    setView(1);
}

void window::about(void)
{
    showMsg("about");
}

void window::sceneDescription(void)
{
    showMsg("description");
}

void window::changeFile(QString val)
{
    loading = true;
    properties->write();
    edit->write();
    manager->setCurrentFile(val);
    properties->read();
    edit->read();
    loading = false;
}

void window::changeView(int val)
{
    setView(val);
}

void window::setImage(QImage* image)
{
    preview->setImage(image);
}

void window::createActions(void)
{
    newAction = new QAction(QIcon("C:/Qt/Tools/QtCreator/bin/untitled/new.png"), "New", this);
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newScene()));

    openAction = new QAction(QIcon("C:/Qt/Tools/QtCreator/bin/untitled/open.png"), "Open", this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), this, SLOT(openScene()));

    saveAction = new QAction(QIcon("C:/Qt/Tools/QtCreator/bin/untitled/save.png"), "Save", this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveScene()));

    quitAction = new QAction("Quit", this);
    quitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

    copyAction = new QAction(QIcon("C:/Qt/Tools/QtCreator/bin/untitled/copy.png"), "Copy", this);
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(QIcon("C:/Qt/Tools/QtCreator/bin/untitled/paste.png"), "Paste", this);
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

    networkAction = new QAction(QIcon("C:/Qt/Tools/QtCreator/bin/untitled/wifi.png"), "Networking", this);
    connect(networkAction, SIGNAL(triggered()), this, SLOT(networking()));

    renderAction = new QAction(QIcon("C:/Qt/Tools/QtCreator/bin/untitled/play.png"), "Render", this);
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

    renderMenu->addAction(networkAction);
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
    toolBar->addAction(networkAction);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    currentFile = new QComboBox();
    currentFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    currentFile->setMaximumSize(QSize(150, 25));
    toolBar->addWidget(currentFile);
    connect(currentFile, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeFile(QString)));
}

void window::createStatusbar(void)
{
    view = new QComboBox(this);
    view->addItem("Scene Editor");
    view->addItem("Render Preview");
    connect(view, SIGNAL(currentIndexChanged(int)), this, SLOT(changeView(int)));

    progress = new QProgressBar(this);
    progress->setTextVisible(false);

    statusBar()->addWidget(view);
    statusBar()->addWidget(progress, 1);
}

void window::setView(int v)
{
    if(v == 0){
        preview->setParent(NULL);
        setCentralWidget(edit);
        console->setVisible(false);
        properties->setVisible(true);
    }
    else if(v == 1){
        edit->setParent(NULL);
        setCentralWidget(preview);
        properties->setVisible(false);
        console->setVisible(true);
    }
}

