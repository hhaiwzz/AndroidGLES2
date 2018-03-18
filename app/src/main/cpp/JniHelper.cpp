//
// Created by hhai on 3/18/18.
//

#include "JniHelper.h"
#include <android/log.h>
#include <stdio.h>

JniHelper *JniHelper::jniHelper = nullptr;

JniHelper::JniHelper(JNIEnv *env, jobject assetManager, jstring currentPath)
{
    if (jniHelper != nullptr)
    {
        __android_log_print(ANDROID_LOG_DEBUG, JNIHELPER_TAG, "jniHelper already created.");
        return;
    }

    this->assetManager = AAssetManager_fromJava(env, assetManager);

    // move string from java to c++
    const char *path = env->GetStringUTFChars(currentPath, nullptr);
    this->currentPath = string(path);
    env->ReleaseStringUTFChars(currentPath, path);
    __android_log_print(ANDROID_LOG_DEBUG, JNIHELPER_TAG, "current path = %s", this->currentPath.c_str());

    this->jniHelper = this;
    pthread_mutex_init(&threadMutex, nullptr);
}


JniHelper::~JniHelper() {
//    delete assetManager;
//    assetManager = nullptr;
    this->jniHelper = nullptr;

    pthread_mutex_destroy(&threadMutex);
}

string JniHelper::GetFullPath(const string &path)
{
    return string(currentPath + "/" + path);
}

bool JniHelper::ExtractFile(const string &assetName) {
    string fileName = currentPath + "/" + assetName;

    FILE* file = fopen(fileName.c_str(), "rb");
    if (file)
    {
        __android_log_print(ANDROID_LOG_INFO, JNIHELPER_TAG, "Found extracted file in assets: %s", fileName.c_str());
        fclose(file);

        pthread_mutex_unlock(&threadMutex);
        return true;
    }

    pthread_mutex_lock(&threadMutex);
    AAsset* asset = AAssetManager_open(assetManager, assetName.c_str(), AASSET_MODE_STREAMING);

    if (asset != nullptr)
    {
        char buff[BUFSIZ];
        int nb_read = 0;

        FILE* file = fopen(fileName.c_str(), "w");
        while ((nb_read = AAsset_read(asset, buff, BUFSIZ)) > 0)
        {
            fwrite(buff, nb_read, 1, file);
        }

        fclose(file);
        AAsset_close(asset);
        __android_log_print(ANDROID_LOG_DEBUG, JNIHELPER_TAG, "Asset extracted: %s", fileName.c_str());
    }
    else
    {
        __android_log_print(ANDROID_LOG_DEBUG, JNIHELPER_TAG, "Asset not found: %s", fileName.c_str());
    }

    pthread_mutex_unlock(&threadMutex);

    return true;
}
