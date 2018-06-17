!FBXSTRUCT_PRI {

CONFIG += FBXSTRUCT_PRI

INCLUDEPATH += $$PWD/../

HEADERS += \
    $$PWD/mesh.h\
    $$PWD/joint.h\
    $$PWD/skeleton.h\
    $$PWD/cluster.h\
    $$PWD/rig.h

SOURCES += \
    $$PWD/mesh.cpp \
    $$PWD/joint.cpp \
    $$PWD/skeleton.cpp \
    $$PWD/cluster.cpp\
    $$PWD/rig.cpp
}
