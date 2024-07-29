# Android.mk for building the Android executable

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Set the source files
LOCAL_SRC_FILES := $(LOCAL_PATH)/../src/main.cpp \
									 $(LOCAL_PATH)/../src/yuvTool.cpp

# Set the output executable name
LOCAL_MODULE := yuvTool

# Set the include paths
LOCAL_C_INCLUDES := ./src

#set link libs
LOCAL_LDLIBS := -llog
# Set the compiler flags
LOCAL_CFLAGS := -std=c++11 -DPIC -D__ANDROID__ -D__HAS_LOG__ -O2 -fPIC -fvisibility=hidden
# Set the linker flags
LOCAL_LDFLAGS := -Wall, -static-libgcc -static-libstdc++
# Define the build target

# include external/libcxx/libcxx.mk
include $(BUILD_EXECUTABLE)