//
// Created by admin on 2022/5/15.
//

#include "AudioDecoder.h"

long AudioDecoder::GetAudioDecoderTimestampForAVSync(void *context) {
    return 0;
}

void AudioDecoder::OnDecoderReady() {

}

void AudioDecoder::OnDecoderDone() {

}

void AudioDecoder::OnFrameAvailable(AVFrame *frame) {

}

void AudioDecoder::ClearCache() {
    BaseDecoder::ClearCache();
}
