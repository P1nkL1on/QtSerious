!STRUCTFBX_PRI {

CONFIG += STRUCTFBX_PRI

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/structfbx.h

SOURCES += \
    $$PWD/structfbx.cpp
}

HEADERS += \
    $$PWD/skin.h \
    $$PWD/skeleton.h \
    $$PWD/mesh.h \
    $$PWD/rig.h

SOURCES += \
    $$PWD/skin.cpp \
    $$PWD/skeleton.cpp \
    $$PWD/mesh.cpp \
    $$PWD/rig.cpp

