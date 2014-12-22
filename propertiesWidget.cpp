#include <propertiesWidget.h>
#include <valueContainer.h>

PropertiesWidget::PropertiesWidget(fileManager* m, QString name, QMainWindow* w) :
    QDockWidget(name, w)
{
    manager = m;

    setVisible(false);
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    area = new QScrollArea(this);
    area->setWidgetResizable(true);
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QWidget* widget = new QWidget();

    //setup the camera properties
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    Header* sceneHeader = new Header("Scene Settings");
    layout->addWidget(sceneHeader);

    QString sizeFields[2] = {"Width", "Height"};
    imageSize = new valueContainer(manager, IMAGESIZE, "Image Size", 2, sizeFields, valueContainer::TYPE_INT);
    imageSize->range(1, 10000);
    layout->addWidget(imageSize);

    QString ambientField[1] = "Factor";
    ambient = new valueContainer(manager, AMBIENT, "Ambient", 1, ambientField, valueContainer::TYPE_FLOAT);
    ambient->range(0, 1);
    layout->addWidget(ambient);

    QString backgroundFields[3] = {"R", "G", "B"};
    background = new valueContainer(manager, BACKGROUND, "Background Color", 3, backgroundFields, valueContainer::TYPE_INT);
    background->range(0, 255);
    layout->addWidget(background);

    QString recursionFields[3] = {"Depth", "Glossy Refl", "Glossy Refr"};
    recursion = new valueContainer(manager, RECURSIVE, "Recursion Settings", 3, recursionFields, valueContainer::TYPE_INT);
    recursion->range(1, 100);
    layout->addWidget(recursion);

    antialiasing = new antiContainer(manager);
    layout->addWidget(antialiasing);

    Header* cameraHeader = new Header("Camera Settings");
    layout->addWidget(cameraHeader);

    QString cameraField[3] = {"X", "Y", "Z"};
    pos = new valueContainer(manager, POSITION, "Position", 3, cameraField, valueContainer::TYPE_FLOAT);
    pos->range(-10000, 10000);
    layout->addWidget(pos);

    look = new valueContainer(manager, LOOKAT, "Look At", 3, cameraField, valueContainer::TYPE_FLOAT);
    look->range(-10000, 10000);
    layout->addWidget(look);

    up = new valueContainer(manager, UP, "Up", 3, cameraField, valueContainer::TYPE_FLOAT);;
    up->range(-10000, 10000);
    layout->addWidget(up);

    QWidget* sceneSpacer = new QWidget();
    sceneSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(sceneSpacer);

    widget->setLayout(layout);

    area->setMinimumSize(200, 100);

    area->setWidget(widget);

    setWidget(area);

    w->addDockWidget(Qt::LeftDockWidgetArea, this);
}

void PropertiesWidget::readOnly(bool b)
{
    imageSize->readOnly(b);
    ambient->readOnly(b);
    background->readOnly(b);
    recursion->readOnly(b);
    antialiasing->readOnly(b);
    pos->readOnly(b);
    look->readOnly(b);
    up->readOnly(b);
}

void PropertiesWidget::read(void)
{
    sceneData scene;
    manager->read(fileManager::SCENE, &scene);

    imageSize->read(&scene);
    ambient->read(&scene);
    background->read(&scene);
    recursion->read(&scene);
    antialiasing->read(&scene);

    cameraData camera;
    manager->read(fileManager::CAMERA, &camera);

    pos->read(&camera);
    look->read(&camera);
    up->read(&camera);
}

void PropertiesWidget::write(void)
{
    sceneData scene;
    imageSize->write(&scene);
    ambient->write(&scene);
    background->write(&scene);
    recursion->write(&scene);
    antialiasing->write(&scene);

    manager->write(fileManager::SCENE, &scene);

    cameraData camera;
    pos->write(&camera);
    look->write(&camera);
    up->write(&camera);

    manager->write(fileManager::CAMERA, &camera);
}

Header::Header(QString title)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(135, 206, 250));
    setAutoFillBackground(true);
    setPalette(pal);
    QVBoxLayout* headerLayout = new QVBoxLayout;
    headerLayout->addWidget(new QLabel(title), 0, Qt::AlignCenter);
    setLayout(headerLayout);
}
