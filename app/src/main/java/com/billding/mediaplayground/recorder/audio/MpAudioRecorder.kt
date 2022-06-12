package com.billding.mediaplayground.recorder.audio

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.media.AudioFormat
import android.media.AudioRecord
import android.media.MediaCodec
import android.media.MediaRecorder
import android.os.Handler
import android.os.HandlerThread
import android.os.Looper
import androidx.core.app.ActivityCompat
import java.nio.ByteBuffer
import java.nio.ByteOrder

/**
 * AudioRecorder
 */
class MpAudioRecorder {

    companion object {
        const val ERROR_CODE_CREATE = 0
        const val ERROR_CODE_START = 1
        const val ERROR_CODE_STOP = 2
        const val ERROR_CODE_RELEASE = 3
    }


    private val mConfig: MPAudioCaptureConfig
    private val mListener: MpAudioCaptureListener

    private val mRecordThread: HandlerThread
    private val mRecordHandler: Handler

    private val mReadThread: HandlerThread
    private val mReadHandler: Handler

    private val mMainHandler = Handler(Looper.getMainLooper())
    private var mAudioRecord: AudioRecord? = null

    private var mMinBufferSize = 0

    @Volatile
    private var mRecording = false

    private val mContext: Context

    constructor(context: Context, config: MPAudioCaptureConfig, listener: MpAudioCaptureListener) {
        mContext = context
        mConfig = config
        mListener = listener

        mRecordThread = HandlerThread("Mp_Audio_Record_Thread")
        mRecordThread.start()
        mRecordHandler = Handler(mRecordThread.looper)

        mReadThread = HandlerThread("Mp_Audio_Read_Thread")
        mReadThread.start()
        mReadHandler = Handler(mReadThread.looper)

        mRecordHandler.post {
            setupAudioRecord()
        }
    }

    private fun setupAudioRecord() {
        if (mAudioRecord == null) {
            mMinBufferSize = AudioRecord.getMinBufferSize(
                mConfig.sampleRate,
                mConfig.channel,
                AudioFormat.ENCODING_PCM_16BIT
            )

            try {
                if (ActivityCompat.checkSelfPermission(
                        mContext,
                        Manifest.permission.RECORD_AUDIO
                    ) != PackageManager.PERMISSION_GRANTED
                ) {
                    mAudioRecord = AudioRecord(
                        MediaRecorder.AudioSource.MIC,
                        mConfig.sampleRate,
                        mConfig.channel,
                        AudioFormat.ENCODING_PCM_16BIT,
                        mMinBufferSize
                    )
                } else {
                    throw Exception("Permission required.")
                }

            } catch (e: Exception) {
                errorCallback(ERROR_CODE_CREATE, e.message.toString())
            }
        }
    }

    private fun errorCallback(code: Int, msg: String) {
        mMainHandler.post {
            mListener.onError(code, msg)
        }
    }

    fun start() {
        mRecordHandler.post {
            mAudioRecord?.let {
                if (!mRecording) {
                    try {
                        // 开始录制
                        mAudioRecord?.startRecording()
                        mRecording = true
                    } catch (e: Exception) {
                        errorCallback(ERROR_CODE_START, e.message.toString())
                    }

                    mReadHandler.post{
                        readFromAudioRecorder()
                    }
                }
            }
        }
    }

    private fun readFromAudioRecorder() {
        while (mRecording) {
            val pcmData = ByteArray(mMinBufferSize)
            val readSize = mAudioRecord?.read(pcmData, 0, mMinBufferSize) ?: return
            if (readSize > 0) {
                val buffer = ByteBuffer.allocateDirect(readSize)
                    .put(pcmData)
                    .order(ByteOrder.nativeOrder())

                buffer.position(0)
                val bufferInfo = MediaCodec.BufferInfo()
                bufferInfo.presentationTimeUs = System.nanoTime()/ 1000
                bufferInfo.size = readSize
                val bufferFrame = MpAudioFrame(buffer, bufferInfo)
                mListener.onFrameAvailable(bufferFrame)
            }
        }
    }

    fun stop() {
        mRecordHandler.post{
            mAudioRecord?.let {
                if (mRecording) {
                    try {
                        mAudioRecord?.stop()
                        mRecording = false
                    } catch (e: Exception) {
                        errorCallback(ERROR_CODE_STOP, e.message.toString())
                    }
                }
            }
        }
    }

    fun release() {
        mRecordHandler.post {
            mAudioRecord?.let {
                if (mRecording) {
                    try {
                        mAudioRecord?.stop()
                        mRecording = false
                    } catch (e: Exception) {
                        errorCallback(ERROR_CODE_RELEASE, e.message.toString())
                    }

                    try {
                        mAudioRecord?.release()
                        mRecording = false
                    } catch (e: Exception) {
                        errorCallback(ERROR_CODE_RELEASE, e.message.toString())
                    }

                    mAudioRecord = null
                }
            }
        }
        mRecordThread.quit()
        mReadThread.quit()
    }

}