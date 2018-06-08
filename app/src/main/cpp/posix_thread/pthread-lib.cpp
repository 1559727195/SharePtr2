//
// Created by zhu on 2018/6/5.
//
#include <pthread.h>
#include <jni.h>
#include <semaphore.h>
int pthread_create(pthread_t* thread,
pthread_attr_t const* attr,
void* (*start_routine)(void*),
void* arg);

int pthread_join(pthread_t thread,void** ret_val);

int pthread_mutex_init(
        pthread_mutex_t* mutex,
        const pthread_attr_t*
        attr
);

static jobject gObj = NULL;

static  pthread_mutex_t mutex;
//jint JNI_OnLoad(JavaVM *vm, void *reserved) {
//    // Cache the JavaVM interface pointer
//    gVm = vm;
//
//    return JNI_VERSION_1_4;
//}

extern int sem_init(sem_t* sem,int pshared, unsigned int value);

extern  int sem_wait(sem_t* sem);

