//
// Created by hhai on 3/18/18.
//

#include "JniCall.h"
#include "JniHelper.h"
#include "SimpleShader.h"

JniHelper *jniHelper = nullptr;

SimpleShader simpleShader;

JNIEXPORT void JNICALL
Java_hhai_gles2_main_Init(JNIEnv *env, jobject instance, jobject assetManager, jstring currentPath)
{
    jniHelper = new JniHelper(env, assetManager, currentPath);
//    simpleShader.LoadShaderFromFile("Shaders/SimpleVertexShader.vert", "Shaders/SimpleFragmentShader.frag");
}

JNIEXPORT void JNICALL
Java_hhai_gles2_SimpleGLRenderer_Render(JNIEnv *env, jobject instance)
{

}
