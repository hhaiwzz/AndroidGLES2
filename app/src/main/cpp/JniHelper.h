//
// Created by hhai on 3/18/18.
//

#ifndef GLES2_JNIHELPER_H
#define GLES2_JNIHELPER_H

#include <jni.h>
#include <android/asset_manager_jni.h>
#include <string>
#include <pthread.h>
#include <stdio.h>

using std::string;

class JniHelper
{
private:
    const char* JNIHELPER_TAG = "JniHelper";

    // Use to lock file when extracting from asset
    mutable pthread_mutex_t threadMutex;
    AAssetManager *assetManager = nullptr;
    string currentPath;

public:
    static JniHelper *jniHelper;

    JniHelper(JNIEnv *env, jobject assetManager, jstring currentPath);
    ~JniHelper();

    string GetFullPath(const string &path);
    bool ExtractFile(const string &assetName);
};

#endif //GLES2_JNIHELPER_H
