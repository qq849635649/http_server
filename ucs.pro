INCLUDEPATH += \
    src/ \
    include \
    modules

SOURCES += \
    src/main.cpp \
    src/timegeneration.cpp \
    src/slabs.cc \
    src/slabs.cc \
    src/config.cpp \
    src/server.cpp \
    src/process.cpp \
    src/util/os_util.cpp

HEADERS += \
    src/timegeneration.h \
    src/slabs.h \
    src/singleton.h \
    src/sharedstack.h \
    src/sharedrwlock.h \
    src/slabs.h \
    src/sharedstack.h \
    src/sharedrwlock.h \
    src/config.h \
    src/server.h \
    src/mainloop.h \
    src/process.h \
    src/util/os_util.h

OTHER_FILES += \
    etc/base.conf
