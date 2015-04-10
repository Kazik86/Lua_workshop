LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := lua

# lista plików Lua z doc/readme.html 'Building Lua on other systems'
LOCAL_SRC_FILES := src/lapi.c \
		   src/lcode.c \
		   src/lctype.c \
		   src/ldebug.c \
		   src/ldo.c \
		   src/ldump.c \
		   src/lfunc.c \
		   src/lgc.c \
		   src/llex.c \
		   src/lmem.c \
		   src/lobject.c \
		   src/lopcodes.c \
		   src/lparser.c \
		   src/lstate.c \
		   src/lstring.c \
		   src/ltable.c \
		   src/ltm.c \
		   src/lundump.c \
		   src/lvm.c \
		   src/lzio.c \
		   src/lauxlib.c \
		   src/lbaselib.c \
		   src/lbitlib.c \
		   src/lcorolib.c \
		   src/ldblib.c \
		   src/liolib.c \
		   src/lmathlib.c \
		   src/loslib.c \
		   src/lstrlib.c \
		   src/ltablib.c \
		   src/loadlib.c \
		   src/linit.c

include $(BUILD_SHARED_LIBRARY)
