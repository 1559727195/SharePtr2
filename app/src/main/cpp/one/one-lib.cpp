//
// Created by zhu on 2018/6/4.
//

#include <jni.h>
static  jmethodID  gOnNativeMessage = NULL;
extern "C"
JNIEXPORT jstring JNICALL
Java_com_massky_shareptr_JNIUtils_sayHelloFromJNI_1one_1lib(JNIEnv *env, jclass type) {

    // TODO


    return env->NewStringUTF("");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_massky_shareptr_MainActivity_nativeInit(JNIEnv *env, jobject instance) {

    // TODO
    if (NULL == gOnNativeMessage) {
        jclass clazz = env->GetObjectClass(instance);
        gOnNativeMessage = env->GetMethodID(clazz,
                                            "onNativeMessage",
                                            "Ljava/lang/String;)V");

        if (NULL == gOnNativeMessage) {
            jclass exceptionClazz = env->FindClass(
                    "java/lang/RuntimeException"
            );
            env->ThrowNew(exceptionClazz,
                          "Unable to find method");
        }
    }

}extern "C"
JNIEXPORT void JNICALL
Java_com_massky_shareptr_MainActivity_nativeFree(JNIEnv *env, jobject instance) {

    // TODO

}extern "C"
JNIEXPORT void JNICALL
Java_com_massky_shareptr_MainActivity_nativeWorker(JNIEnv *env, jobject instance, jint id,
                                                   jint interations) {

    // TODO

}