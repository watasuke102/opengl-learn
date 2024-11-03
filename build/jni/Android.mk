LOCAL_PATH := $(call my-dir)
UNITY_DIR := $(HOME)/Unity/Hub/Editor/2022.3.11f1
include $(CLEAR_VARS)

SRC_DIR          := ../../src
LOCAL_MODULE     := glesdraw_android
LOCAL_SRC_FILES  := $(SRC_DIR)/lib.cpp $(SRC_DIR)/shader.cpp $(SRC_DIR)/graphic.cpp
LOCAL_LDLIBS     := -lGLESv3
LOCAL_CPPFLAGS   += -I../../inc -I$(UNITY_DIR)/Editor/Data/PluginAPI -std=c++17 -fno-exceptions -fno-rtti -lGLESv3

include $(BUILD_SHARED_LIBRARY)
