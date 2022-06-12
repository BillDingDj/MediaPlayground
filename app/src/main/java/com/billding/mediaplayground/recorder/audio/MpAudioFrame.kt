package com.billding.mediaplayground.recorder.audio

import android.media.MediaCodec.BufferInfo
import com.billding.mediaplayground.recorder.MpFrame
import com.billding.mediaplayground.recorder.MpFrameType
import java.nio.ByteBuffer

class MpAudioFrame : MpFrame {

    private val mByteBuffer: ByteBuffer
    private val mBufferInfo: BufferInfo

    constructor(inputBuffer: ByteBuffer, bufferInfo: BufferInfo): super(MpFrameType.BUFFER) {
        mByteBuffer = inputBuffer
        mBufferInfo = bufferInfo
    }
}
