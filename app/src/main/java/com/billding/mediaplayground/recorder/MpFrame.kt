package com.billding.mediaplayground.recorder

/**
 * 音频和视频的单位帧
 */
open class MpFrame {

    private val mFrameType: MpFrameType

    constructor(type: MpFrameType) {
        mFrameType = type
    }

    protected fun getFrameType() = mFrameType
}