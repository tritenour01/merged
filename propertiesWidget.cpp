#include <propertiesWidget.h>
#include <valueContainer.h>

PropertiesWidget::PropertiesWidget(fileManager* m)
{
    manager = m;

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

    QString gammaField[1] = "Factor";
    gamma = new valueContainer(manager, GAMMA, "Gamma", 1, gammaField, valueContainer::TYPE_FLOAT);
    gamma->range(0, 5);
    layout->addWidget(gamma);

    QString backgroundFields[3] = {"R", "G", "B"};
    background = new valueContainer(manager, BACKGROUND, "Background Color", 3, backgroundFields, valueContainer::TYPE_FLOAT);
    background->range(0, 1);
    layout->addWidget(background);

    mode = new renderModeContainer(manager);
    layout->addWidget(mode);

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

    QVBoxLayout* l = new QVBoxLayout;
    l->addWidget(area);
    setLayout(l);
}

void PropertiesWidget::readOnly(bool b)
{
    imageSize->readOnly(b);
    gamma->readOnly(b);
    background->readOnly(b);
    recursion->readOnly(b);
    antialiasing->readOnly(b);
    mode->readOnly(b);
    pos->readOnly(b);
    look->readOnly(b);
    up->readOnly(b);
}

void PropertiesWidget::read(void)
{
    sceneData scene;
    manager->read(fileManager::SCENE, &scene);

    imageSize->read(&scene);
    gamma->read(&scene);
    background->read(&scene);
    recursion->read(&scene);
    antialiasing->read(&scene);
    mode->read(&scene);

    cameraData camera;
    manager->read(fileManager::CAMERA, &camera);

    pos->read(&camera);
    look->read(&camera);
    up->read(&camera);
}

void PropertiesWidget::write(void)
{
    sceneData scene;
    gamma->write(&scene);
    imageSize->write(&scene);
    background->write(&scene);
    recursion->write(&scene);
    antialiasing->write(&scene);
    mode->write(&scene);

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
