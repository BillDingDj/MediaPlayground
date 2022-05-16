#include <jni.h>
#include <string>
#include "FFmpegPlayer.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_billding_mediaplayground_player_FfmpegPlayer_native_1init(JNIEnv *env, jobject thiz,
                                                                   jstring path,
                                                                   jint video_render_type,
                                                                   jobject surface) {
    const char *url = env->GetStringUTFChars(path, nullptr);
    FFmpegPlayer *player = new FFmpegPlayer();
    player->Init(env, thiz, const_cast<char *>(url), video_render_type, surface);
    env->ReleaseStringUTFChars(path, url);
    return reinterpret_cast<long> (player);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_billding_mediaplayground_player_FfmpegPlayer_native_1play(JNIEnv *env, jobject thiz,
                                                                   jlong handle) {
    if (handle != 0) {
        FFmpegPlayer *player = reinterpret_cast<FFmpegPlayer *>(handle);
        player->Play();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_billding_mediaplayground_player_FfmpegPlayer_native_1stop(JNIEnv *env, jobject thiz,
                                                                   jlong handle) {
    if (handle != 0) {
        FFmpegPlayer *player = reinterpret_cast<FFmpegPlayer *>(handle);
        player->Stop();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_billding_mediaplayground_player_FfmpegPlayer_native_1pause(JNIEnv *env, jobject thiz,
                                                                    jlong handle) {
    if (handle != 0) {
        FFmpegPlayer *player = reinterpret_cast<FFmpegPlayer *>(handle);
        player->Pause();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_billding_mediaplayground_player_FfmpegPlayer_native_1seek(JNIEnv *env, jobject thiz,
                                                                   jlong handle, jfloat position) {
    if (handle != 0) {
        FFmpegPlayer *player = reinterpret_cast<FFmpegPlayer *>(handle);
        player->SeekToPosition(position);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_billding_mediaplayground_player_FfmpegPlayer_native_1release(JNIEnv *env, jobject thiz,
                                                                      jlong handle) {
    if(handle != 0)
    {
        FFmpegPlayer *ffMediaPlayer = reinterpret_cast<FFmpegPlayer *>(handle);
        ffMediaPlayer->UnInit();
    }
}