#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QDebug>

typedef struct fileInfo
{
    QString fileName;
    QString filePath;
    bool saved;
    bool edited;
}fileInfo;

typedef struct antiData
{
    QString type;
    int sampling;
    float threshold;

    antiData()
    {
        type = "basic";
        sampling = 0;
        threshold = 0.0f;
    }

    QString toString(void)
    {
        qDebug()<<type;
        if(type == "basic")
            return "";
        else if(type == "uniform")
            return "uniform " + QString::number(sampling) + "\n";
        else if(type == "jitter")
            return "jitter " + QString::number(sampling) + "\n";
        else if(type == "adaptive"){
            QString thres = QString::number(threshold);
            return "adaptive " + QString::number(sampling) + " <" + thres + ", " + thres + ", " + thres + ">\n";
        }
    }

}antiData;

typedef struct sceneData
{
    int imageSize[2];
    float ambient;
    int backgroundColor[3];
    int recursive[3];
    antiData antialiasing;

    sceneData()
    {
        imageSize[0] = 500;
        imageSize[1] = 500;

        ambient = 0.0f;

        backgroundColor[0] = 0;
        backgroundColor[1] = 0;
        backgroundColor[2] = 0;

        recursive[0] = 1;
        recursive[1] = 1;
        recursive[2] = 1;
    }

    QString toString(void)
    {
        return "width " + QString::number(imageSize[0]) + "\n" +
               "height " + QString::number(imageSize[1]) + "\n" +
               "ambient " + QString::number(ambient) + "\n" +

               "background <" + QString::number(backgroundColor[0]) + ", " +
                                QString::number(backgroundColor[1]) + ", " +
                                QString::number(backgroundColor[2]) + ">\n" +

                "depth " + QString::number(recursive[0]) + "\n" +
                "glossyReflectSampling " + QString::number(recursive[1]) + "\n" +

                antialiasing.toString();
    }
}sceneData;

typedef struct cameraData
{
    float position[3];
    float lookat[3];
    float up[3];

    cameraData()
    {
        position[0] = 0;
        position[1] = 0;
        position[2] = 0;

        lookat[0] = 0;
        lookat[1] = 0;
        lookat[2] = 1;

        up[0] = 0;
        up[1] = 1;
        up[2] = 0;
    }

    QString vectorString(float* values)
    {
        return QString("<%1, %2, %3>").arg(QString::number(values[0]),
                                           QString::number(values[1]),
                                           QString::number(values[2]));
    }

    QString toString(void)
    {
        return QString("camera{\n"
                       "    %1\n"
                       "    %2\n"
                       "    %3\n"
                       "}\n").arg(vectorString(position),
                                 vectorString(lookat),
                                 vectorString(up));
    }
}cameraData;

typedef struct fileObject
{
    int fileID;
    fileInfo info;
    sceneData scene;
    cameraData camera;
    QString text;

    QString toString(void)
    {
        return scene.toString() + camera.toString() + text;
    }
}fileObject;

#endif // DATAMODEL_H
