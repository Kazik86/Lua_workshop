LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL2

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../include/lua
LOCAL_CPP_FEATURES += exceptions

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	src/actor.cpp \
	src/actorMgr.cpp \
	src/fsm.cpp \
	src/gMove.cpp \
	src/gRandomPos.cpp \
	src/gTexture.cpp \
	src/gUnitTest.cpp \
	src/gadget.cpp \
	src/game.cpp \
	src/luaLibs.cpp \
	src/luaModule.cpp \
	src/luaState.cpp \
	src/main.cpp \
	src/renderer.cpp \
	src/textureMgr.cpp \
	src/userdata.cpp

LOCAL_SHARED_LIBRARIES := gnustl_shared SDL2 SDL2_image lua

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
