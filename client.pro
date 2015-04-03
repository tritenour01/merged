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

win32{
    INCLUDEPATH += $$quote(C:/Program Files (x86)\CodeBlocks\MinGW\x86_64-w64-mingw32\include)
    INCLUDEPATH += $$quote(C:\Users\Tim Ritenour\Documents\C++\raytracer\raytracer)

    CONFIG(Debug){
        LIBS += -static -L"C:\Users\Tim"" ""Ritenour\Documents\C++\raytracer\raytracer\bin\Debug" -lraytracer
        PRE_TARGETDEPS += "C:\Users\Tim"" ""Ritenour\Documents\C++\raytracer\raytracer\bin\Debug\libraytracer.a"

        LIBS += -static -L"C:\Program"" ""Files"" ""(x86)\CodeBlocks\MinGW\x86_64-w64-mingw32\lib" -lsfml-graphics-d
        PRE_TARGETDEPS += "C:\Program"" ""Files"" ""(x86)\CodeBlocks\MinGW\x86_64-w64-mingw32\lib\libsfml-graphics-d.a"
    }
    else{
        LIBS += -static -L"C:\Users\Tim"" ""Ritenour\Documents\C++\raytracer\raytracer\bin\Release" -lraytracer
        PRE_TARGETDEPS += "C:\Users\Tim"" ""Ritenour\Documents\C++\raytracer\raytracer\bin\Release\libraytracer.a"

        LIBS += -static -L"C:\Program"" ""Files"" ""(x86)\CodeBlocks\MinGW\x86_64-w64-mingw32\lib" -lsfml-graphics
        PRE_TARGETDEPS += "C:\Program"" ""Files"" ""(x86)\CodeBlocks\MinGW\x86_64-w64-mingw32\lib\libsfml-graphics.a"
    }
}

linux-g++{
    DEPENDPATH += "/home/tim/Documents/raytracer/raytracer"
    INCLUDEPATH += "/home/tim/Documents/raytracer/raytracer"

    CONFIG(debug, debug|release){
        LIBS += -L"/home/tim/Documents/raytracer/raytracer/bin/Debug" -lraytracer
        PRE_TARGETDEPS += "/home/tim/Documents/raytracer/raytracer/bin/Debug/libraytracer.so"
    }
    else{
        LIBS += -L"/home/tim/Documents/raytracer/raytracer/bin/Release" -lraytracer
        PRE_TARGETDEPS += "/home/tim/Documents/raytracer/raytracer/bin/Release/libraytracer.so"
    }
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
    connection.cpp \
    settingDockWidget.cpp \
    previewWindow.cpp \
    jobWidget.cpp \
    jobManager.cpp \
    renderJob.cpp \
    UIprogressEvent.cpp

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
    connection.h \
    settingDockWidget.h \
    previewWindow.h \
    jobWidget.h \
    jobManager.h \
    renderJob.h \
    UIprogressEvent.h
