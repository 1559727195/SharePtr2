#include <jni.h>
#include <string>
#include<iostream>
#include<string>
#include<fstream>
#include<list>
#include<vector>
#include<map>
#include<set>
#include<cctype>//ctype无法打开，包含tolower()函数和ispunct函数
#include<algorithm>
#include<utility>//保存pair的头文件
#include<iostream>
#include<string>
#include<fstream>
#include<list>
#include<vector>
#include<map>
#include<set>
#include<cctype>//ctype无法打开，包含tolower()函数和ispunct函数
#include<algorithm>
#include<utility>//保存pair的头文件
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void newint(JNIEnv *env) ;
using namespace std;

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_massky_shareptr_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    //main();
//    main5();
    std::string hello = "Hello from C++";

    jstring javaString;
    javaString = env->NewStringUTF("HelloWord");

    const jbyte *str;
    jboolean isCopy;
    str = (const jbyte *) env->GetStringUTFChars(javaString, &isCopy);
    if (0 != str) {
        printf("Java string: %s", str);

        if (JNI_TRUE == isCopy) {
            printf("C string is a copy of the Java string.");
        } else {
            printf("C string points to actual string.");
        }
    }

    env->ReleaseStringUTFChars(javaString, (const char *) str);
    newint(env);
    return env->NewStringUTF(hello.c_str());
}


void fread() {

}


void malloc() {
    int *dynamicIntArray = (int*) malloc(sizeof(int) * 16);
    if (NULL == dynamicIntArray) {

    } else {
        *dynamicIntArray = 0;
        dynamicIntArray[8] = 8;

        free(dynamicIntArray);
        dynamicIntArray = NULL;
        int* newDynamicIntArray = (int*) realloc(dynamicIntArray, sizeof(int) * 32);

    }
}

void excute_system() {
    int result;
    result = system("mkdir /data/data/com.example.hellojni/temp");

    FILE* stream;
    stream = popen("ls","r");

//    while (NULL != fgets(buff)) {
//
//    }

}

void newint(JNIEnv *env) {
    jintArray  javaArray;
    javaArray = env->NewIntArray(10);
    if (0 != javaArray) {

    }

    jint  nativeArray[10];
    env->GetIntArrayRegion(javaArray,0,10,nativeArray);

    env->SetIntArrayRegion(javaArray,0,10,nativeArray);

    jint * nativeDirectArray;
    jboolean isCopy;

    nativeDirectArray = env->GetIntArrayElements(javaArray,&isCopy);

    env->ReleaseIntArrayElements(javaArray,nativeDirectArray,0);

    unsigned  char* buffer = (unsigned char *) malloc(1024);

    jobject  directBuffer;
    directBuffer = env->NewDirectByteBuffer(buffer, 1024);

    buffer = (unsigned char *) env->GetDirectBufferAddress(directBuffer);

    jclass  clazz;
//    clazz = env->GetObjectClass(instance);

    jfieldID  instanceFieldId;
    instanceFieldId = env->GetFieldID(clazz,"instanceField"
    ,"Ljava/lang/String");

    jfieldID  staticFieldId;
    staticFieldId = env-> GetStaticFieldID(clazz,"staticField",""
            "Ljava/lang/String");

//    jstring  instanceField;
//    instanceField = env->GetObjectField(instance,instanceFieldId);
//
//    jstring  staticField;
//    staticField = (jstring) env->GetStaticObjectField(clazz, staticFieldId);
//
//    jmethodID  instanceMethodId;
//    instanceMethodId = env->GetMethodID(clazz,"instanceMethod",
//    "()Ljava/lang/String");
//
//    jthrowable  ex;
//    env->CallVoidMethod(instance,throwingMethodId);
//    ex = env->ExceptionOccurred();
//
//    env->FindClass("java/lang/String");
//
//    jclass weakGlobalClazz;
//
//    weakGlobalClazz = env->NewWeakGlobalRef(localClazz);
//
//    if (JNI_OK == env->MonitorEnter(obj)) {
//
//    }
//
//    if (JNI_OK == env->MonitorExit(obj)) {
//
//    }


    JavaVM* cachedjvm;

    (*cachedjvm).AttachCurrentThread((JNIEnv **) cachedjvm, NULL);
 //


}


//void main1() {
//    ifstream in("1.txt");
//    string s;
//    StrBlob blob;
//    while (getline(in,s))
//    {
//        blob.pushback(s);
//    }
//
////    for (StrBlobPtr pbeg(blob.begin()), pend(blob.end()); pbeg != pend;
////         pbeg.incr())
////        cout << pbeg.deref() << std::endl;
//
//
//    unique_ptr<int[]> up(new int[10]);
//    up.release();
//
//
//    shared_ptr<int> sp(new int[10],[](int *p) {
//        delete[] p;
//    });
//
//    sp.reset();
//
//    for (size_t i = 0; i != 10; ++i) {
//        *(sp.get() + i) = i;
//    }





//auto p = make_shared<int>(42);
//weak_ptr<int> wp(p);
//if (shared_ptr<int> np = wp.lock()) {
//
//}
//

//int ix = 1024, *pi = &ix, *pi2 = new int(2048);
//
//typedef  unique_ptr<int> IntP;
//
//IntP p0(ix);
//
//IntP p1(pi);
//IntP p2(pi2);
//IntP p3(&ix);
//
//IntP p4(new int(2048));
//IntP p5(p2.get());
//
//shared_ptr<int> p (new int(42));
//
//process(shared_ptr<int>(p));
//
//process(shared_ptr<int>(p.get()));
//
//
//unique_ptr<string> p1(new string("Stegosuurus"));
//unique_ptr<string> p2(p1.release());
//unique_ptr<string> p3(new string("Stegosuurus1"));
//p2.reset(p3.release());

//auto p = p2.release();



//nt *q = new int(42), *r = new int(100);
//  r = q;

//  auto q2 = make_shared<int >(42), r2 = make_shared<int >(100);
//  r2 = q2;

//  shared_ptr<double > p1;
//  shared_ptr<int> p2(new int (42));
//}


