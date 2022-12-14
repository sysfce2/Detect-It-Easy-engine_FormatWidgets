INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogsearchvalues.h \
    $$PWD/searchvalueswidget.h

SOURCES += \
    $$PWD/dialogsearchvalues.cpp \
    $$PWD/searchvalueswidget.cpp

FORMS += \
    $$PWD/dialogsearchvalues.ui \
    $$PWD/searchvalueswidget.ui

!contains(XCONFIG, multisearch) {
    XCONFIG += multisearch
    include($$PWD/../MultiSearch/multisearch.pri)
}

DISTFILES += \
    $$PWD/searchvalueswidget.cmake
