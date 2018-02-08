!TESTVIEWER_PRI {

CONFIG += TESTVIEWER_PRI

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/camera.h \
	$$PWD/lightsourse.h \
	$$PWD/testviewer.h \
	$$PWD/graphicsobject.h

SOURCES += \
    $$PWD/camera.cpp \
	$$PWD/lightsourse.cpp \
	$$PWD/testviewer.cpp \
	$$PWD/graphicsobject.cpp
}

