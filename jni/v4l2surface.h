#ifndef _V4L2SURFACE_H_
#define _V4L2SURFACE_H_

#include <android/log.h>
#include <stdio.h>
#include "jni.h"
#include <gui/Surface.h>
#include <android/native_window_jni.h>
#include <android/native_window.h>
#include <system/window.h>
#include <linux/videodev2.h>
#include <utils/StrongPointer.h>
#include "video_capture.h"
#include <pthread.h>
#include <android_runtime/android_view_Surface.h>


__BEGIN_DECLS

namespace android {

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
static sp<Surface> surface;
static sp<ANativeWindow> window = NULL;
static ANativeWindow_Buffer abuffer;
uint8_t  src_image[SCREEN_WIDTH * SCREEN_HEIGHT * 2]; //YUYV

static void *capture_thread(void *arg);


}
__END_DECLS

#endif //_V4L2SURFACE_H_