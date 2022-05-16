//
// Created by admin on 2022/5/15.
//

#ifndef MEDIAPLAYGROUND_FFMPEGPLAYER_H
#define MEDIAPLAYGROUND_FFMPEGPLAYER_H


#include <jni.h>
#include "VideoDecoder.h"
#include "AudioDecoder.h"
#include "AudioRender.h"
/**
 * 解码器包括VideoDecoder 和AudioDecoder
 * 渲染器包括VideoRender 和AudioRender
 */
class FFmpegPlayer {
public:
    FFmpegPlayer(){};
    ~FFmpegPlayer(){};

    void Init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface);
    void UnInit();

    void Play();
    void Pause();
    void Stop();
    void SeekToPosition(float position);
    long GetMediaParams(int paramType);

private:
    JNIEnv *GetJNIEnv(bool *isAttach);
    jobject GetJavaObj();
    JavaVM *GetJavaVM();

    static void PostMessage(void *context, int msgType, float msgCode);

    JavaVM *m_JavaVM = nullptr;
    jobject m_JavaObj = nullptr;

    VideoDecoder *m_VideoDecoder = nullptr;
    AudioDecoder *m_AudioDecoder = nullptr;

    VideoRender *m_VideoRender = nullptr;
    AudioRender *m_AudioRender = nullptr;
};


#endif //MEDIAPLAYGROUND_FFMPEGPLAYER_H
