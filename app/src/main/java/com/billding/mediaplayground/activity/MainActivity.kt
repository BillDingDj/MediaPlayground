package com.billding.mediaplayground.activity

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.billding.mediaplayground.R

/**
 * 主Activity
 */
class MainActivity : AppCompatActivity() {

    private var mTextureButton: View? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        mTextureButton = findViewById(R.id.button_texture_activity)
        bindTextureButtonFunction()
    }

    private fun bindTextureButtonFunction() {
        mTextureButton?.setOnClickListener {
            val intent = Intent(this, TextureSurfaceActivity::class.java)
            startActivity(intent)
        }
    }
}