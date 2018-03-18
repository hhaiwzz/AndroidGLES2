//
// Created by hhai on 3/17/18.
//

#ifndef GLES2_JNICALL_H
#define GLES2_JNICALL_H

#include <jni.h>

extern "C"
{

JNIEXPORT void JNICALL
Java_hhai_gles2_main_Init(JNIEnv *env, jobject instance, jobject assetManager, jstring currentPath);

JNIEXPORT void JNICALL
Java_hhai_gles2_SimpleGLRenderer_Render(JNIEnv *env, jobject instance);

};

#endif //GLES2_JNICALL_H
