//
// Created by admin on 2022/5/15.
//

#include "FFmpegPlayer.h"
#include "OpenSLRender.h"
#include "LogUtil.h"
#include "NativeRender.h"

void FFmpegPlayer::Init(JNIEnv *jniEnv, jobject obj, char *url, int renderType, jobject surface) {

    jniEnv->GetJavaVM(&m_JavaVM);
    m_JavaObj = jniEnv->NewGlobalRef(obj);

    m_AudioDecoder = new AudioDecoder(url);
    m_VideoDecoder = new VideoDecoder(url);

    m_VideoRender = new NativeRender(jniEnv, surface);
    m_VideoDecoder->SetVideoRender(m_VideoRender);
    m_AudioRender = new OpenSLRender();
    m_AudioDecoder->SetAudioRender(m_AudioRender);
}

void FFmpegPlayer::UnInit() {
    if (m_VideoDecoder) {
        delete m_VideoDecoder;
        m_VideoDecoder = nullptr;
    }

    if (m_VideoRender) {
        delete m_VideoRender;
        m_VideoRender = nullptr;
    }

    if (m_AudioDecoder) {
        delete m_AudioDecoder;
        m_AudioDecoder = nullptr;
    }

    if (m_AudioRender) {
        delete m_AudioRender;
        m_AudioRender = nullptr;
    }
    LOGCATD("FFmpeg player uninit.");
    bool isAttach = false;
    GetJNIEnv(&isAttach)->DeleteGlobalRef(m_JavaObj);
    if (isAttach)
        GetJavaVM()->DetachCurrentThread();
}

void FFmpegPlayer::Play() {
    LOGCATD("FFmpegPlayer::Play");
    if (m_AudioDecoder) {
        m_AudioDecoder->Start();
    }

    if (m_VideoDecoder) {
        m_VideoDecoder->Start();
    }
}

void FFmpegPlayer::Pause() {
    LOGCATE("FFMediaPlayer::Pause");
    if (m_VideoDecoder)
        m_VideoDecoder->Pause();

    if (m_AudioDecoder)
        m_AudioDecoder->Pause();

}

void FFmpegPlayer::Stop() {
    LOGCATE("FFMediaPlayer::Stop");
    if (m_VideoDecoder)
        m_VideoDecoder->Stop();

    if (m_AudioDecoder)
        m_AudioDecoder->Stop();
}

void FFmpegPlayer::SeekToPosition(float position) {
    LOGCATE("FFMediaPlayer::SeekToPosition position=%f", position);
    if (m_VideoDecoder)
        m_VideoDecoder->SeekToPosition(position);

    if (m_AudioDecoder)
        m_AudioDecoder->SeekToPosition(position);

}