INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/elfwidget.ui \
    $$PWD/dialogelf.ui \
    $$PWD/programheaderwidget.ui \
    $$PWD/sectionheaderwidget.ui

HEADERS += \
    $$PWD/elfwidget.h \
    $$PWD/elf_defs.h \
    $$PWD/dialogelf.h \
    $$PWD/programheaderwidget.h \
    $$PWD/sectionheaderwidget.h

SOURCES += \
    $$PWD/elfwidget.cpp \
    $$PWD/elf_defs.cpp \
    $$PWD/dialogelf.cpp \
    $$PWD/programheaderwidget.cpp \
    $$PWD/sectionheaderwidget.cpp

!contains(XCONFIG, xelf) {
    XCONFIG += xelf
    include($$PWD/../../Formats/xelf.pri)
}

!contains(XCONFIG, formatwidget) {
    XCONFIG += formatwidget
    include($$PWD/../formatwidget.pri)
}
