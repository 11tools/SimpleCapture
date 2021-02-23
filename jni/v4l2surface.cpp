#include <jni.h>
#include <android_runtime/AndroidRuntime.h>
#include <android_runtime/android_view_Surface.h>
#include <gui/Surface.h>
#include <assert.h>
#include <utils/Log.h>
#include <media/stagefright/foundation/ADebug.h>
#include <ui/GraphicBufferMapper.h>
#include <cutils/properties.h>
#include "v4l2surface.h"


namespace android {
 
static void start_vlive() {
    pthread_t ntid;
    pthread_create(&ntid, NULL, capture_thread, NULL);
}


void init_display() {
    int32_t result = ANativeWindow_setBuffersGeometry(window.get(), SCREEN_WIDTH, SCREEN_HEIGHT, HAL_PIXEL_FORMAT_YCBCR_422_I);
    if (result < 0) {
        ALOGD("ANativeWindow_setBuffersGeometry failed");
        ANativeWindow_release(window.get());
        window = NULL;
        return;
    }
    ANativeWindow_acquire(window.get());
}

void copy_display(unsigned char* src, int width, int height){
    
    if (ANativeWindow_lock(window.get(), &abuffer, NULL) < 0) {
        ALOGD("ANativeWindow_lock failed");
        ANativeWindow_release(window.get());
        window = NULL;
        return;
    }
  
    //ALOGD("h = %d w = %d stride = %d ", buffer.height, buffer.width, buffer.stride);
    //ALOGD("display buff size = %d video buff size = %d", buffer.width * buffer.height * 4, buffer.stride / buffer.width);

    uint8_t *line = (uint8_t *) abuffer.bits;
    //video_capture(line, SCREEN_WIDTH, SCREEN_HEIGHT);

    /*for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            line[(x+y*SCREEN_WIDTH)*4] =  src_image[(x+y*SCREEN_WIDTH)*3] ; //R
            line[(x+y*SCREEN_WIDTH)*4 + 1] = src_image[(x+y*SCREEN_WIDTH)*3 + 1] ; //G
            line[(x+y*SCREEN_WIDTH)*4 + 2] = src_image[(x+y*SCREEN_WIDTH)*3 + 2] ; //B
            line[(x+y*SCREEN_WIDTH)*4 + 3] = -1 ; //A
        }
    }*/

    memcpy(line,src_image,SCREEN_WIDTH*SCREEN_HEIGHT*2);

    if (ANativeWindow_unlockAndPost(window.get()) < 0) {
        ALOGE("ANativeWindow_unlockAndPost failed");
    }
}


void deinit_display(){
    ANativeWindow_release(window.get());
}


void *capture_thread(void *arg){

	int width = SCREEN_WIDTH, height = SCREEN_HEIGHT;
    memset(src_image, -1, sizeof(src_image));
	ALOGD("Capture thread start!");
    init_display();
	init_video_capture(width, height);
	for(; ;){
		video_capture(src_image, width, height);
        copy_display(src_image,width, height);
	}
    free_video_capture();
    deinit_display();
}




static jboolean
nativeSetVideoSurface(JNIEnv *env, jobject thiz, jobject jsurface){
	ALOGE("[%s]%d",__FILE__,__LINE__);
	surface = android_view_Surface_getSurface(env, jsurface);
	if(android::Surface::isValid(surface)){
		ALOGE("surface is valid ");
	}else {
		ALOGE("surface is invalid ");
		return false;
	}
    sp<IGraphicBufferProducer> gbp;
    gbp = surface->getIGraphicBufferProducer();
    if(gbp != NULL)
        window = new Surface(gbp);
    if(window != NULL) {
        window->incStrong((void*)ANativeWindow_acquire);
    }    
	ALOGE("[%s][%d]\n",__FILE__,__LINE__);
    start_vlive();
	return true;
}


static JNINativeMethod gMethods[] = {
	{"nativeSetVideoSurface",		"(Landroid/view/Surface;)Z", 		(void *)nativeSetVideoSurface},
};
 
static const char* const kClassPathName = "com/example/android/simplejni/MainActivity";
 
// This function only registers the native methods
static int register_func(JNIEnv *env)
{
	ALOGE("[%s]%d",__FILE__,__LINE__);
    return AndroidRuntime::registerNativeMethods(env,
                kClassPathName, gMethods, NELEM(gMethods));
}
 
extern "C" JNIEXPORT  jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	ALOGE("[%s]%d",__FILE__,__LINE__);
    JNIEnv* env = NULL;
    jint result = -1;
 
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed\n");
        goto bail;
    }
    assert(env != NULL);
    ALOGE("JNI_OnLoad load");
	ALOGE("[%s]%d",__FILE__,__LINE__);
   if (register_func(env) < 0) {
        ALOGE("ERROR: MediaPlayer native registration failed\n");
        goto bail;
    }
 
    /* success -- return valid version number */
    result = JNI_VERSION_1_4;
 
bail:
    return result;
}

}