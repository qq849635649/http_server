INCLUDEPATH += \
    src/ \
    include \
    modules

HEADERS += \
    src/log/timegeneration.h \
    src/singleton.h \
    src/config.h \
    src/server.h \
    src/mainloop.h \
    src/process.h \
    src/util/os_util.h \
    src/log/logger.h \
    src/log/linebuffer.h \
    src/log/filehandler.h \
    src/app/application.h \
    src/app/master.h \
    src/app/worker.h \
    src/shm/sharedstack.h \
    src/shm/sharedrwlock.h \
    src/shm/slabs.h \
    src/shm/sharedstack.h \
    src/shm/sharedrwlock.h \
    src/shm/sharedallocator.h \
    src/datamanager.h \
    src/shm/hashtable.h

SOURCES += \
    src/main.cpp \
    src/log/timegeneration.cpp \
    src/config.cpp \
    src/server.cpp \
    src/process.cpp \
    src/util/os_util.cpp \
    src/logger.cpp \
    src/app/master.cpp \
    src/app/worker.cpp \
    src/shm/slabs.cc \
    src/datamanager.cpp \
    src/shm/hashtable.cpp

OTHER_FILES += \
    etc/base.conf
