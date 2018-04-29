CROSS = 
Q = @
CXX = $(CROSS)g++
RM = rm -rf
DEBUG = -g
CFLAGS = $(DEBUG) -Wall -c

##########################################################
# 软件信息
##########################################################
TARGET = UcServer
RELEASE_VERSION = 1.11.17
RELEASE_PATH = ucs-release
RELEASE_NAME = $(TARGET)_V$(RELEASE_VERSION).tar.gz

##########################################################
# 路径信息
##########################################################
TOP_PATH = $(shell pwd)
SRC_PATH = $(TOP_PATH)/src
MOD_PATH = $(TOP_PATH)/modules

INC_PATH += -I$(MOD_PATH)
INC_PATH += -I$(SRC_PATH)
INC_PATH += -I$(TOP_PATH)/include

LIB_PATH += -L$(MOD_LIB_PATH)
LIB_PATH += -L$(TOP_PATH)/lib

##########################################################
# 第三方库
##########################################################
LIBS += $(TOP_PATH)/lib/libevent.a
LIBS += $(TOP_PATH)/lib/libconfig.a
LIBS += -lpthread -lrt

##########################################################
# source files
##########################################################
DIRS = $(shell find $(SRC_PATH) -maxdepth 3 -type d)
SRCS_CPP += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
SRCS_CC += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cc))
SRCS_C += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))

##########################################################
# sub Makefile
##########################################################
include $(MOD_PATH)/Makefile

##########################################################
# objs
##########################################################
OBJS_CPP = $(patsubst %.cpp, %.o, $(SRCS_CPP))
OBJS_CC = $(patsubst %.cc, %.o, $(SRCS_CC))
OBJS_C = $(patsubst %.c, %.o, $(SRCS_C))

##########################################################
# building
##########################################################
.PHONY: clean build

all: $(TARGET)

$(TARGET) : $(OBJS_CPP) $(OBJS_CC) $(OBJS_C)
	$(Q) $(CXX) $^ -o $@ $(LIB_PATH) $(LIBS)
	$(Q) echo Create $(TARGET) ok...

package:
	$(Q) $(RM) $(RELEASE_PATH)
	$(Q) $(RM) $(RELEASE_NAME)

$(OBJS_CPP):%.o : %.cpp
	$(Q) $(CXX) $(CFLAGS) $< -o $@ $(INC_PATH)
	$(Q) echo "    CC" $@

$(OBJS_CC):%.o : %.cc
	$(Q) $(CXX) $(CFLAGS) $< -o $@ $(INC_PATH)
	$(Q) echo "    CC" $@

$(OBJS_C):%.o : %.c
	$(Q) $(CXX) $(CFLAGS) $< -o $@ $(INC_PATH)
	$(Q) echo "    CC" $@

clean:
	$(Q) $(RM) $(TARGET) $(RELEASE_PATH) $(RELEASE_NAME)
	$(Q) $(RM) $(OBJS_CPP) $(OBJS_CC) $(OBJS_C)
	$(Q) echo "clean ok..."

