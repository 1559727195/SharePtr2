//
// Created by zhu on 2018/6/4.
//

#include <jni.h>
#include <pthread.h>
#include <cstdio>
#include <unistd.h>

static  void* nativeWorkerThread (void* args);
// Native worker thread arguments
struct NativeWorkerArgs {
    jint id;
    jint iterations;
};

// Method ID can be cached
static jmethodID gOnNativeMessage = NULL;

// Java VM interface pointer
static JavaVM *gVm = NULL;

// Global reference to object
static jobject gObj = NULL;

// Mutex instance
static pthread_mutex_t mutex;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    // Cache the JavaVM interface pointer
    gVm = vm;

    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_massky_shareptr_JNIUtils_sayHelloFromJNI_1one_1lib(JNIEnv *env, jclass type) {

    // TODO


    return env->NewStringUTF("");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_massky_shareptr_MainActivity_nativeInit(JNIEnv *env, jobject instance) {


    // Initialize mutex
    if (0 != pthread_mutex_init(&mutex, NULL)) {
        // Get the exception class
        jclass exceptionClazz = env->FindClass(
                "java/lang/RuntimeException");

        // Throw exception
        env->ThrowNew(exceptionClazz, "Unable to initialize mutex");
        goto exit;
    }

    // If object global reference is not set
    if (NULL == gObj) {
        // Create a new global reference for the object
        gObj = env->NewGlobalRef(instance);

        if (NULL == gObj) {
            goto exit;
        }
    }

    // If method ID is not cached
    if (NULL == gOnNativeMessage) {
        // Get the class from the object
        jclass clazz = env->GetObjectClass(instance);

        // Get the method id for the callback
        gOnNativeMessage = env->GetMethodID(clazz,
                                            "onNativeMessage",
                                            "(Ljava/lang/String;)V");

        // If method could not be found
        if (NULL == gOnNativeMessage) {
            // Get the exception class
            jclass exceptionClazz = env->FindClass(
                    "java/lang/RuntimeException");

            // Throw exception
            env->ThrowNew(exceptionClazz, "Unable to find method");
        }
    }

    exit:
    return;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_massky_shareptr_MainActivity_nativeFree(JNIEnv *env, jobject instance) {

    if(0 != pthread_mutex_destroy(&mutex)) {
        jclass  exceptionClazz = env->FindClass(
            "java/lang/RuntimeException"
        );
        env->ThrowNew(exceptionClazz,
        "Unable to destroy mutex");
        goto exit;
    }

    // TODO
    if (NULL != gObj) {
        env->DeleteGlobalRef(gObj);
        gObj = NULL;
    }

    exit:
    return;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_massky_shareptr_MainActivity_nativeWorker(JNIEnv *env, jobject instance, jint id,
                                                   jint interations) {
    if(0 != pthread_mutex_destroy(&mutex)) {
        jclass  exceptionClazz = env->FindClass(
                "java/lang/RuntimeException"
        );
        env->ThrowNew(exceptionClazz,
                      "Unable to destroy mutex");
        goto exit;
    }
    // TODO
    for (jint i = 0; i < interations;i++) {
        char message[26];

        sprintf(
                message,"Worker %d: Interation %d",
                id,i
        );

        jstring  messageString = env->NewStringUTF(message);

        env->CallVoidMethod(
          instance,
          gOnNativeMessage
          ,messageString
        );

        if (NULL != env->ExceptionOccurred()) {
            break;
        }

        sleep(1);
    }

    if(0 != pthread_mutex_destroy(&mutex)) {
        jclass  exceptionClazz = env->FindClass(
                "java/lang/RuntimeException"
        );
        env->ThrowNew(exceptionClazz,
                      "Unable to destroy mutex");
        goto exit;
    }
    exit:
    return;

}

extern "C"
JNIEXPORT void JNICALL
Java_com_massky_shareptr_MainActivity_posixThreads(JNIEnv *env, jobject instance, jint threads,
                                                   jint iterations) {

    pthread_t * handles = new pthread_t[threads];

    for (jint i = 0; i < threads; i++) {
        NativeWorkerArgs* nativeWorkerArgs = new NativeWorkerArgs();
        nativeWorkerArgs->id = i;
        nativeWorkerArgs->iterations = iterations;
        pthread_t  thread;
        int result = pthread_create(
          &handles[i],
          NULL,
          nativeWorkerThread,
          (void*)nativeWorkerArgs
        );
        if (0 != result) {
            jclass  excetionClazz = env->FindClass("java/lang/RuntimeException");

            env->ThrowNew(excetionClazz, "Unable to create thread");
            goto exit;
        }

//        for (int i = 0; i < threads; i++) {
//            void* result = NULL;
//            if (0 != pthread_join(handles[i],&result)) {
//                jclass  excetionClazz = env->FindClass("java/lang/RuntimeException");
//
//                env->ThrowNew(excetionClazz, "Unable to join thread");
//            } else {
//                char message[26];
//
//                sprintf(
//                        message,"Worker %d: Interation %d",
//                        i,result
//                );
//
//                jstring  messageString = env->NewStringUTF(message);
//
//                env->CallVoidMethod(
//                        instance,
//                        gOnNativeMessage
//                        ,messageString
//                );
//
//                if (NULL != env->ExceptionOccurred()) {
//                    goto exit;
//                }
//            }
//        }
        exit:
        return;
    }

}


static  void* nativeWorkerThread (void* args) {
    JNIEnv* env = NULL;
    if (0 == gVm->AttachCurrentThread(&env,NULL)) {

        NativeWorkerArgs* nativeWorkerArgs = (NativeWorkerArgs *) args;

        Java_com_massky_shareptr_MainActivity_nativeWorker(env,
        gObj,
        nativeWorkerArgs->id,
        nativeWorkerArgs->iterations);

        delete nativeWorkerArgs;
        gVm->DetachCurrentThread();
    }

    //JNIEnv *env, jobject instance, jint id,
    //jint interations
    //,,
//    Java_com_massky_shareptr_MainActivity_nativeWorker(JNIEnv *env, jobject instance, jint id,
//            jint interations)

    return (void*) 1;
}

