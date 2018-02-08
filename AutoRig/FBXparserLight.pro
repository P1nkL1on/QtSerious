#-------------------------------------------------
#
# Project created by QtCreator 2017-11-21T16:54:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FBXparserLight
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include (../modules/ModelLoader/ModelLoader.pri)
include (../modules/Stereometry/Stereometry.pri)
include (../modules/TestViewer/TestViewer.pri)
include (../modules/SceneTools/SceneTools.pri)

include(../modules/Eigen/Eigen.pri)
include(../modules/Derivable/Derivable.pri)

include (LoaderFBXlight/LoaderFBX.pri)
include (StructFBX/StructFBX.pri)

include (TestAutoRig/TestAutoRig.pri)
include (MeshComparer/MeshComparer.pri)
include (GaussNewton/GaussNewton.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui


