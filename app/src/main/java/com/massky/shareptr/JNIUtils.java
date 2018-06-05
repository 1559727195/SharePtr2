package com.massky.shareptr;

/**
 * Created by zhu on 2018/6/4.
 */

public class JNIUtils {
    static {
        System.loadLibrary("one-lib");
    }
    public static native  String sayHelloFromJNI_one_lib();
}
