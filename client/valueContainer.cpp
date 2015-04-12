#include <valueContainer.h>
#include <mainwindow.h>

#include <QDebug>

valueContainer::valueContainer(fileManager* m, PropertiesWidget::PROPERTIES prop, QString title, int num, QString* fields, DataType type) :
    QGroupBox(title)
{
    manager = m;
    property = prop;

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setupContents(num, fields, type);
    valueDataType = type;
}

void valueContainer::setupContents(int num, QString* fields, DataType type)
{
    QFormLayout* layout = new QFormLayout;

    for(int i = 0; i < num; i++){
        QAbstractSpinBox* newBox;
        if(type == TYPE_INT){
            newBox = new QSpinBox();
            connect(newBox, SIGNAL(valueChanged(int)), window::getInstance(), SLOT(fileEdited()));
        }
        else if(type == TYPE_FLOAT){
            newBox = new QDoubleSpinBox();
            connect(newBox, SIGNAL(valueChanged(double)), window::getInstance(), SLOT(fileEdited()));
        }
        layout->addRow(new QLabel(fields[i] + ": "), newBox);
        values.push_back(newBox);
    }

    setLayout(layout);
}

void valueContainer::readOnly(bool b)
{
    for(int i = 0; i < values.size(); i++)
        values[i]->setReadOnly(b);
}

void valueContainer::range(int min, int max)
{
    for(int i = 0; i < values.size(); i++){
        switch(valueDataType)
        {
            case TYPE_INT:
            {
                QSpinBox* val = (QSpinBox*)values[i];
                val->setMinimum(min);
                val->setMaximum(max);
                break;
            }
            case TYPE_FLOAT:
            {
                QDoubleSpinBox* val = (QDoubleSpinBox*)values[i];
                val->setMinimum((double)min);
                val->setMaximum((double)max);
                break;
            }
        }
    }
}

void valueContainer::getData(void* data)
{
    switch(valueDataType)
    {
        case TYPE_INT:
        {
            int* int_data  =(int*)data;
            for(int i = 0; i < values.size(); i++){
                QSpinBox* val = (QSpinBox*)values[i];
                int_data[i] = val->value();
            }
            break;
        }
        case TYPE_FLOAT:
        {
            float* float_data = (float*)data;
            for(int i = 0; i < values.size(); i++){
                QDoubleSpinBox* val = (QDoubleSpinBox*)values[i];
                float_data[i] = (float)val->value();
            }
            break;
        }
    }
}

void valueContainer::write(void* data)
{
    switch(property)
    {
        case PropertiesWidget::IMAGESIZE:
            getData((void*)((sceneData*)data)->imageSize);
            break;
        case PropertiesWidget::GAMMA:
            getData((void*)&((sceneData*)data)->gamma);
            break;
        case PropertiesWidget::AMBIENT:
            getData((void*)&((sceneData*)data)->ambient);
            break;
        case PropertiesWidget::BACKGROUND:
            getData((void*)((sceneData*)data)->backgroundColor);
            break;
        case PropertiesWidget::RECURSIVE:
            getData((void*)((sceneData*)data)->recursive);
            break;
        case PropertiesWidget::POSITION:
            getData((void*)((cameraData*)data)->position);
            break;
        case PropertiesWidget::LOOKAT:
            getData((void*)((cameraData*)data)->lookat);
            break;
        case PropertiesWidget::UP:
            getData((void*)((cameraData*)data)->up);
            break;
    }
}

void valueContainer::setData(void* data)
{
    switch(valueDataType)
    {
        case TYPE_INT:
        {
            int* int_data = (int*)data;
            for(int i = 0; i < values.size(); i++){
                QSpinBox* val = (QSpinBox*)values[i];
                val->setValue(int_data[i]);
            }
            break;
        }
        case TYPE_FLOAT:
        {
            float* float_data = (float*)data;
            for(int i = 0; i < values.size(); i++){
                QDoubleSpinBox* val = (QDoubleSpinBox*)values[i];
                val->setValue((double)float_data[i]);
            }
            break;
        }
    }
}

void valueContainer::read(void* data)
{
    switch(property)
    {
        case PropertiesWidget::IMAGESIZE:
            setData((void*)((sceneData*)data)->imageSize);
            break;
        case PropertiesWidget::GAMMA:
            setData((void*)&((sceneData*)data)->gamma);
            break;
        case PropertiesWidget::AMBIENT:
            setData((void*)&((sceneData*)data)->ambient);
            break;
        case PropertiesWidget::BACKGROUND:
            setData((void*)((sceneData*)data)->backgroundColor);
            break;
        case PropertiesWidget::RECURSIVE:
            setData((void*)((sceneData*)data)->recursive);
            break;
        case PropertiesWidget::POSITION:
            setData((void*)((cameraData*)data)->position);
            break;
        case PropertiesWidget::LOOKAT:
            setData((void*)((cameraData*)data)->lookat);
            break;
        case PropertiesWidget::UP:
            setData((void*)((cameraData*)data)->up);
            break;
    }
}

antiContainer::antiContainer(fileManager* m) :
    QGroupBox("Antialiasing")
{
    manager = m;

    QFormLayout* antiLayout = new QFormLayout;

    type = new QComboBox();
    type->addItem("Basic");
    type->addItem("Uniform");
    type->addItem("Jitter");
    type->addItem("Adaptive");
    connect(type, SIGNAL(currentIndexChanged(int)), this, SLOT(changed(int)));
    connect(type, SIGNAL(currentIndexChanged(int)), window::getInstance(), SLOT(fileEdited()));

    antiLayout->addRow(type);

    sampling = new QSpinBox();
    connect(sampling, SIGNAL(valueChanged(int)), window::getInstance(), SLOT(fileEdited()));

    threshold = new QDoubleSpinBox();
    connect(threshold, SIGNAL(valueChanged(double)), window::getInstance(), SLOT(fileEdited()));

    antiLayout->addRow(new QLabel("Sampling: "), sampling);
    antiLayout->addRow(new QLabel("Threshold: "), threshold);

    setLayout(antiLayout);

    changeType(0);
}

void antiContainer::readOnly(bool b)
{
    type->setEnabled(!b);
    sampling->setReadOnly(b);
    threshold->setReadOnly(b);
}

void antiContainer::changed(int val)
{
    changeType(val);
}

void antiContainer::changeType(int val)
{
    switch(val)
    {
        case 0:
            sampling->setEnabled(false);
            threshold->setEnabled(false);
            break;
        case 1:
            sampling->setEnabled(true);
            threshold->setEnabled(false);
            break;
        case 2:
            sampling->setEnabled(true);
            threshold->setEnabled(false);
            break;
        case 3:
            sampling->setEnabled(true);
            threshold->setEnabled(true);
            break;
    }
}

void antiContainer::write(sceneData* scene)
{
    switch(type->currentIndex())
    {
        case 0:
            scene->antialiasing.type = "basic";
            break;
        case 1:
            scene->antialiasing.type = "uniform";
            break;
        case 2:
            scene->antialiasing.type = "jitter";
            break;
        case 3:
            scene->antialiasing.type = "adaptive";
            break;
    }
    scene->antialiasing.sampling = sampling->value();
    scene->antialiasing.threshold = threshold->value();
}

void antiContainer::read(sceneData* scene)
{
    QString t = scene->antialiasing.type;
    if(t == "basic")
            type->setCurrentIndex(0);
    if(t == "uniform")
            type->setCurrentIndex(1);
    if(t == "jitter")
            type->setCurrentIndex(2);
    if(t == "adaptive")
            type->setCurrentIndex(3);

    sampling->setValue(scene->antialiasing.sampling);
    threshold->setValue(scene->antialiasing.threshold);
}

renderModeContainer::renderModeContainer(fileManager* m) :
    QGroupBox("Render Mode")
{
    manager = m;
    modeLayout = new QFormLayout;

    mode = new QComboBox();
    mode->addItem("Direct Lighting");
    mode->addItem("Photon Mapping");
    connect(mode, SIGNAL(currentIndexChanged(int)), this, SLOT(changed(int)));
    connect(mode, SIGNAL(currentIndexChanged(int)), window::getInstance(), SLOT(fileEdited()));

    modeLayout->addRow(mode);

    directLabel = new QLabel("Ambient");
    ambient = new QDoubleSpinBox();
    ambient->setRange(0, 1);
    connect(ambient, SIGNAL(valueChanged(double)), window::getInstance(), SLOT(fileEdited()));

    photonLabels[0] = new QLabel("Count");
    photonCount = new QSpinBox();
    photonCount->setMaximum(INT_MAX);
    connect(photonCount, SIGNAL(valueChanged(int)), window::getInstance(), SLOT(fileEdited()));

    photonLabels[1] = new QLabel("Samples");
    samples = new QSpinBox();
    samples->setMaximum(INT_MAX);
    connect(samples, SIGNAL(valueChanged(int)), window::getInstance(), SLOT(fileEdited()));

    photonLabels[2] = new QLabel("Radius");
    radius = new QDoubleSpinBox();
    connect(radius, SIGNAL(valueChanged(double)), window::getInstance(), SLOT(fileEdited()));

    photonLabels[3] = new QLabel("Bounces");
    bounces = new QSpinBox();
    connect(bounces, SIGNAL(valueChanged(int)), window::getInstance(), SLOT(fileEdited()));

    setLayout(modeLayout);

    changeMode(0);
}

void renderModeContainer::readOnly(bool b)
{
    mode->setEnabled(!b);
    ambient->setReadOnly(b);
    photonCount->setReadOnly(b);
    samples->setReadOnly(b);
    radius->setReadOnly(b);
    bounces->setReadOnly(b);
}

void renderModeContainer::write(sceneData* scene)
{
    switch(mode->currentIndex())
    {
        case 0:
            scene->mode = "standard";
            scene->ambient = ambient->value();
            break;
        case 1:
            scene->mode = "photon";
            scene->photon.photonCount = photonCount->value();
            scene->photon.maxSamples = samples->value();
            scene->photon.maxRadius = radius->value();
            scene->photon.bounces = bounces->value();
            break;
    }
}

void renderModeContainer::read(sceneData* scene)
{
    QString m = scene->mode;
    if(m == "standard"){
        mode->setCurrentIndex(0);
        ambient->setValue(scene->ambient);
    }
    else{
        mode->setCurrentIndex(1);
        photonCount->setValue(scene->photon.photonCount);
        samples->setValue(scene->photon.maxSamples);
        radius->setValue(scene->photon.maxRadius);
        bounces->setValue(scene->photon.bounces);
    }
}

void renderModeContainer::changed(int id)
{
    changeMode(id);
}

void renderModeContainer::changeMode(int id)
{
    QLayoutItem* item;
    while((item = modeLayout->takeAt(1))){
        item->widget()->setParent(NULL);
        delete item;
    }
    switch(id){
        case 0:
            modeLayout->addRow(directLabel, ambient);
            break;
        case 1:
            modeLayout->addRow(photonLabels[0], photonCount);
            modeLayout->addRow(photonLabels[1], samples);
            modeLayout->addRow(photonLabels[2], radius);
            modeLayout->addRow(photonLabels[3], bounces);
            break;
    }
}
