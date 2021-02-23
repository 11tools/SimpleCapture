LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	v4l2surface.cpp \
	video_capture.cpp \
	
LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	libbinder \
    	libui \
    	libgui \
    	liblog \
	libandroid_runtime \
	libnativewindow \
    	libnativehelper \
    	libstagefright_foundation
	
LOCAL_MODULE:= libvcapture

LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
