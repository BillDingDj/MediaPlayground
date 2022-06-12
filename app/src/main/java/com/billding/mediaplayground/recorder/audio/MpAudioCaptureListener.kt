package com.billding.mediaplayground.recorder.audio

interface MpAudioCaptureListener {

    fun onError(error: Int, errorMsg: String?)

    fun onFrameAvailable(frame: MpAudioFrame?)
}