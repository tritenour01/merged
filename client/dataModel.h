#ifndef DATAMODEL_H
#define DATAMODEL_H

typedef struct fileInfo
{
    QString name;
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

typedef struct photonData
{
    int photonCount;
    int maxSamples;
    float maxRadius;
    int bounces;

    photonData()
    {
        photonCount = 200000;
        maxSamples = 50;
        maxRadius = 0.1f;
        bounces = 4;
    }

    QString toString(void)
    {
        return QString("photon{\n"
                       "    %1\n"
                       "    %2\n"
                       "    %3\n"
                       "    %4\n"
                       "}\n").arg(QString::number(photonCount),
                                  QString::number(maxSamples),
                                  QString::number(maxRadius),
                                  QString::number(bounces));
    }
}photonData;

typedef struct sceneData
{
    QString mode;
    int imageSize[2];
    float gamma;
    float ambient;
    float backgroundColor[3];
    int recursive[3];
    antiData antialiasing;
    photonData photon;

    sceneData()
    {
        mode = "standard";
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
        QString s = QString("mode " + mode + "\n" +
                    "width " + QString::number(imageSize[0]) + "\n" +
                    "height " + QString::number(imageSize[1]) + "\n" +
                    "gamma " + QString::number(gamma) + "\n" +

                    "background <" + QString::number(backgroundColor[0]) + ", " +
                                     QString::number(backgroundColor[1]) + ", " +
                                     QString::number(backgroundColor[2]) + ">\n" +

                    "depth " + QString::number(recursive[0]) + "\n" +
                    "glossyReflectSampling " + QString::number(recursive[1]) + "\n" +
                    "glossyRefractSampling " + QString::number(recursive[2]) + "\n" +

                    antialiasing.toString());

        if(mode == "standard")
            s += "ambient " + QString::number(ambient) + "\n";
        else
            s +=photon.toString();
        return s;
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
        return scene.toString() + camera.toString() + "#\n" + text;
    }
}fileObject;

#endif // DATAMODEL_H
