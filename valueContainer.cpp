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
