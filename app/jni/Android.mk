# Build ImageProcessing

LOCAL_PATH := $(call my-dir)

# Build Image Processing library

include $(CLEAR_VARS)
 
OPENCV_LIB_TYPE:=STATIC
OPENCV_INSTALL_MODULES:=on

include /home/cobalt/Android/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_MODULE    := Processing
LOCAL_SRC_FILES := Processing.cpp
LOCAL_LDLIBS +=  -llog -ldl -ljnigraphics

include $(BUILD_SHARED_LIBRARY)
