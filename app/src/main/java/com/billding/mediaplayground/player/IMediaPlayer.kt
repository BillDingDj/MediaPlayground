package com.billding.mediaplayground.player

import android.view.Surface

interface IMediaPlayer {
    fun initPlayer(path: String, videoRenderType: Int, surface: Surface)

    fun play()

    fun stop()

    fun pause()

    /**
     * @param: float:[0, 1]
     */
    fun seek(position: Float)

    fun release()

    fun setPlayerStateChangeListener(listener: IPlayerStateChangeListener)
}