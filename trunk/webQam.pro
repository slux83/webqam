# webQam Qt project file
# Copyright (C) 2009 Alessio Di Fazio
# License: GPLv3

QT += network
QT += sql
TARGET = webQam
TEMPLATE = app
DESTDIR = bin
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui
OBJECTS_DIR = build/obj
RESOURCES = resources/webQam.qrc
CONFIG += debug
CONFIG += release
SOURCES += src/main.cpp \
    src/dao/sqldrivermanager.cpp \
    src/global/camscontroller.cpp \
    src/gui/systemtray.cpp \
    src/gui/settingsgui.cpp \
    src/gui/imagemenu.cpp \
    src/net/imagehttploader.cpp \
    src/dao/webqamdao.cpp \
    src/model/webcam.cpp \
    src/model/folder.cpp \
    src/model/tree/treemodel.cpp \
    src/model/tree/treeitem.cpp \
    src/model/tree/treemodelfactory.cpp \
    src/global/settingscontroller.cpp \
    src/gui/addwebcamdialog.cpp \
    src/gui/webcamviewer.cpp \
    src/gui/aboutdialog.cpp
FORMS += ui/settingsgui.ui \
    ui/addwebcamdialog.ui \
    ui/webcamviewer.ui \
    ui/aboutdialog.ui
HEADERS += src/dao/sqldrivermanager.h \
    src/global/common.h \
    src/global/camscontroller.h \
    src/gui/systemtray.h \
    src/gui/settingsgui.h \
    src/gui/imagemenu.h \
    src/net/imageloader.h \
    src/net/imagehttploader.h \
    src/dao/webqamdao.h \
    src/model/webcam.h \
    src/model/folder.h \
    src/model/tree/treemodel.h \
    src/model/tree/treeitem.h \
    src/model/tree/treemodelfactory.h \
    src/global/settingscontroller.h \
    src/gui/addwebcamdialog.h \
    src/gui/webcamviewer.h \
    src/gui/aboutdialog.h
