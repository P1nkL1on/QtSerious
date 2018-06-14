!IOFBX_PRI {

CONFIG += IOFBX_PRI

INCLUDEPATH += $$PWD/../

HEADERS += \
    $$PWD/iofbx.h \    
    $$PWD/fbxnode.h \
    $$PWD/fbxgeometrymesh.h \
    $$PWD/fbxconnection.h \
    $$PWD/fbxposenode.h \
    $$PWD/fbxsubdeformercluster.h \
    $$PWD/fbxmodeljoint.h\
    $$PWD/fbxparsedcontainer.h

SOURCES += \
    $$PWD/iofbx.cpp \    
    $$PWD/fbxnode.cpp \
    $$PWD/fbxgeometrymesh.cpp \
    $$PWD/fbxconnection.cpp \
    $$PWD/fbxposenode.cpp \
    $$PWD/fbxsubdeformercluster.cpp \
    $$PWD/fbxmodeljoint.cpp\
    $$PWD/fbxparsedcontainer.cpp
}
