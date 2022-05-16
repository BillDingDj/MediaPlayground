//
// Created by admin on 2022/5/15.
//

#ifndef MEDIAPLAYGROUND_DECODER_H
#define MEDIAPLAYGROUND_DECODER_H

typedef void (*MessageCallback)(void *, int, float);

typedef long (*AVSyncCallback)(void *);

/**
 * 解码器通用参数
 */
class Decoder {
public:
    virtual void Start() = 0;

    virtual void Pause() = 0;

    virtual void Stop() = 0;

    virtual float GetDuration() = 0;

    virtual void SeekToPosition(float position) = 0;

    virtual float GetCurrentPosition() = 0;

    virtual void SetMessageCallback(void *context, MessageCallback callback) = 0;
};

#endif //MEDIAPLAYGROUND_DECODER_H
