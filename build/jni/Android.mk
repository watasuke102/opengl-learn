LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE     := glesdraw_android
LOCAL_SRC_FILES  := ../../src/lib.cpp
LOCAL_LDLIBS     := -lGLESv3
LOCAL_CPPFLAGS   += -std=c++17 -fno-exceptions -fno-rtti -lGLESv3

include $(BUILD_DYNAMIC_LIBRARY)
