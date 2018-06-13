!IOFBXTESTS_PRI {
CONFIG += IOFBXTESTS_PRI

INCLUDEPATH += $$PWD/../

include (IOfbx.pri);

HEADERS += \    
    $$PWD/iofbxtests.h

SOURCES += \    
    $$PWD/iofbxtests.cpp
}
