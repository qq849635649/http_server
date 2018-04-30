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
    src/util/os.cpp \
    src/os.cpp

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
    src/util/os.h \
    src/os.h

OTHER_FILES += \
    etc/base.conf
