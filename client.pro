#-------------------------------------------------
#
# Project created by QtCreator 2014-10-08T21:38:10
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app

CONFIG += c++11 static

INCLUDEPATH += $$quote(C:/Program Files (x86)\CodeBlocks\MinGW\x86_64-w64-mingw32\include)
INCLUDEPATH += $$quote(C:\Users\Tim Ritenour\Documents\C++\raytracer\raytracer)

CONFIG(Debug){
    LIBS += -static -LC:\raytracerBin\ -lraytracerd
    PRE_TARGETDEPS += C:\raytracerBin\libraytracerd.a

    LIBS += -static -L"C:\Program"" ""Files"" ""(x86)\CodeBlocks\MinGW\x86_64-w64-mingw32\lib" -lsfml-graphics-d
    PRE_TARGETDEPS += "C:\Program"" ""Files"" ""(x86)\CodeBlocks\MinGW\x86_64-w64-mingw32\lib\libsfml-graphics-d.a"
}
else{
    LIBS += -static -LC:/raytracerBin/ -lraytracer
    PRE_TARGETDEPS += C:/raytracerBin/libraytracer.a

    LIBS += -static -L"C:\Program"" ""Files"" ""(x86)\CodeBlocks\MinGW\x86_64-w64-mingw32\lib" -lsfml-graphics
    PRE_TARGETDEPS += "C:\Program"" ""Files"" ""(x86)\CodeBlocks\MinGW\x86_64-w64-mingw32\lib\libsfml-graphics.a"
}

SOURCES += main.cpp \
    mainwindow.cpp \
    previewWidget.cpp \
    valueContainer.cpp \
    propertiesWidget.cpp \
    editWidget.cpp \
    consoleWidget.cpp \
    fileManager.cpp \
    scanner.cpp \
    loader.cpp \
    runner.cpp \
    UIimage.cpp \
    networkingDialog.cpp \
    UIlogger.cpp \
    deviceManager.cpp \
    device.cpp \
    connection.cpp

HEADERS  += \
    mainwindow.h \
    previewWidget.h \
    valueContainer.h \
    propertiesWidget.h \
    editWidget.h \
    consoleWidget.h \
    fileManager.h \
    scanner.h \
    loader.h \
    dataModel.h \
    runner.h \
    UIimage.h \
    networkingDialog.h \
    UIlogger.h \
    deviceManager.h \
    device.h \
    connection.h
