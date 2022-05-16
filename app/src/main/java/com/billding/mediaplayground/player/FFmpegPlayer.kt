package com.billding.mediaplayground.player

import android.util.Log
import android.view.Surface

/**
 * 基于FFmpeg的视频播放器
 */
class FfmpegPlayer : IMediaPlayer {

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("media-playground")
        }

        const val TAG = "FfmpegPlayer"
    }

    private var mNativePlayerHandle= 0L

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun native_init(path: String, videoRenderType: Int, surface: Surface): Long
    external fun native_play(handle: Long)
    external fun native_stop(handle: Long)
    external fun native_pause(handle: Long)
    external fun native_seek(handle: Long, position: Float)
    external fun native_release(handle: Long)

    override fun initPlayer(path: String, videoRenderType: Int, surface: Surface) {
        mNativePlayerHandle = native_init(path, videoRenderType, surface)
    }

    override fun play() {
        ensurePlayerReady {
            native_play(mNativePlayerHandle)
        }
    }

    override fun stop() {
        ensurePlayerReady {
            native_stop(mNativePlayerHandle)
        }
    }

    override fun pause() {
        ensurePlayerReady {
            native_pause(mNativePlayerHandle)
        }
    }

    override fun seek(position: Float) {
        if (position < 0 || position >1) {
            Log.e(TAG, "position error: $position")
            return
        }
        ensurePlayerReady {
            native_pause(mNativePlayerHandle)
        }
    }

    override fun release() {
        ensurePlayerReady {
            native_release(mNativePlayerHandle)
        }
    }

    override fun setPlayerStateChangeListener(listener: IPlayerStateChangeListener) {

    }

    private fun ensurePlayerReady(action: () -> Unit) {
        if (mNativePlayerHandle > 0) {
            action.invoke()
        }
    }

}