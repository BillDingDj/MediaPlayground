package com.billding.mediaplayground.activity

import android.graphics.SurfaceTexture
import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.TextureView
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.widget.ConstraintLayout
import com.billding.mediaplayground.R

/**
 * 承载了SurfaceView和TextureView的Activity
 */
class TextureSurfaceActivity : AppCompatActivity() {
    companion object {
        const val TAG = "TextureSurfaceActivity"
    }

    private var mSurfaceViewHolder: ViewGroup? = null

    private var mTextureViewHolder: ViewGroup? = null
    private var mTextureView: TextureView? = null
    private var mSurfaceView: SurfaceView? = null

    private var mSurfaceViewInit = false
    private var mTextureViewInit = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_surface_a_layout)
        mSurfaceViewHolder = findViewById(R.id.surface_view_holer)
        mTextureViewHolder = findViewById(R.id.texture_view_holder)
        mSurfaceViewHolder?.setOnClickListener {
            if (!mSurfaceViewInit) {
                initSurfaceView()
                mSurfaceViewInit = true
            }
        }

        mTextureViewHolder?.setOnClickListener {
            if (!mTextureViewInit) {
                initTextureView()
                mTextureViewInit = true
            }
        }
    }

    private fun initTextureView() {
        val textureView = TextureView(this).apply {
            surfaceTextureListener = object : TextureView.SurfaceTextureListener {
                override fun onSurfaceTextureAvailable(
                    surface: SurfaceTexture,
                    width: Int,
                    height: Int
                ) {
                    Log.d(TAG, "onSurfaceTextureAvailable, width: $width, height: $height")
                }

                override fun onSurfaceTextureSizeChanged(
                    surface: SurfaceTexture,
                    width: Int,
                    height: Int
                ) {
                    Log.d(TAG, "onSurfaceTextureSizeChanged, width: $width, height: $height")

                }

                override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
                    Log.d(TAG, "onSurfaceTextureDestroyed")

                    return false
                }

                override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {
                    Log.d(TAG, "onSurfaceTextureUpdated")

                }
            }
        }
        mTextureViewHolder?.addView(
            textureView,
            ConstraintLayout.LayoutParams(
                ConstraintLayout.LayoutParams.MATCH_PARENT,
                ConstraintLayout.LayoutParams.MATCH_PARENT
            )
        )
        mTextureView = textureView
    }

    private fun initSurfaceView() {
        val surfaceView = SurfaceView(this).apply {
            holder.addCallback(object : SurfaceHolder.Callback {
                override fun surfaceCreated(holder: SurfaceHolder) {
                    Log.d(TAG, "surfaceCreated")

                }

                override fun surfaceChanged(
                    holder: SurfaceHolder,
                    format: Int,
                    width: Int,
                    height: Int
                ) {
                    Log.d(TAG, "surfaceChanged, width: $width, height: $height, format: $format")

                }

                override fun surfaceDestroyed(holder: SurfaceHolder) {
                    Log.d(TAG, "surfaceDestroyed")

                }

            })
        }
        mTextureViewHolder?.addView(
            surfaceView,
            ConstraintLayout.LayoutParams(
                ConstraintLayout.LayoutParams.MATCH_PARENT,
                ConstraintLayout.LayoutParams.MATCH_PARENT
            )
        )
        mSurfaceView = surfaceView
    }
}
