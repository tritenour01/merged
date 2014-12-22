#ifndef VALUECONTAINER_H
#define VALUECONTAINER_H

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QFormLayout>
#include <QComboBox>
#include <vector>
#include <QMessageBox>

#include <fileManager.h>
#include <dataModel.h>
#include <propertiesWidget.h>

class antiContainer : public QGroupBox
{
    Q_OBJECT

    public:

        antiContainer(fileManager*);

        void readOnly(bool);

        void write(sceneData*);
        void read(sceneData*);

    private slots:
        void changed(int);

    private:

        fileManager* manager;

        QComboBox* type;
        QSpinBox* sampling;
        QDoubleSpinBox* threshold;//this should be a float

        void changeType(int);
};

class valueContainer : public QGroupBox
{
    Q_OBJECT

    public:

        enum DataType {TYPE_INT, TYPE_FLOAT};

        valueContainer(fileManager*, PropertiesWidget::PROPERTIES, QString, int, QString*, DataType);
        PropertiesWidget::PROPERTIES property;

        void setupContents(int, QString*, DataType);

        void readOnly(bool);

        void range(int, int);

        void write(void*);
        void read(void*);

    private:

        fileManager* manager;

        std::vector<QAbstractSpinBox*> values;
        DataType valueDataType;

        void getData(void*);
        void setData(void*);
};

#endif // VALUECONTAINER_H
