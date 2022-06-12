//
// Created by admin on 2022/5/15.
//

#include "OpenSLRender.h"

void OpenSLRender::Init() {

}

void OpenSLRender::ClearAudioCache() {

}

void OpenSLRender::RenderAudioFrame(uint8_t *pData, int dataSize) {

}

void OpenSLRender::UnInit() {

}

int OpenSLRender::CreateOutputMixer() {
    return 0;
}

int OpenSLRender::CreateEngine() {
    return 0;
}

int OpenSLRender::CreateAudioPlayer() {
    return 0;
}

int OpenSLRender::GetAudioFrameQueueSize() {
    return 0;
}

void OpenSLRender::StartRender() {

}

void OpenSLRender::HandleAudioFrameQueue() {

}

void OpenSLRender::CreateSLWaitingThread(OpenSLRender *openSlRender) {

}

void OpenSLRender::AudioPlayerCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context) {

}
