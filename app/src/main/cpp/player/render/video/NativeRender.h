//
// Created by admin on 2022/5/15.
//

#ifndef MEDIAPLAYGROUND_NATIVERENDER_H
#define MEDIAPLAYGROUND_NATIVERENDER_H

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "VideoRender.h"

class NativeRender : public VideoRender {
public:
    NativeRender(JNIEnv *env, jobject surface);

    virtual ~NativeRender();

    virtual void Init(int videoWidth, int videoHeight, int *dstSize);

//    virtual void RenderVideoFrame(NativeImage *pImage);

    virtual void UnInit();

private:
    ANativeWindow_Buffer m_NativeWindowBuffer;
    ANativeWindow *m_NativeWindow = nullptr;
    int m_DstWidth;
    int m_DstHeight;
};


#endif //MEDIAPLAYGROUND_NATIVERENDER_H
