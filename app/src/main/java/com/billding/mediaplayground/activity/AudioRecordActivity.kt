package com.billding.mediaplayground.activity

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Column
import androidx.compose.material.Button
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment

class AudioRecordActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            MainUi()
        }
    }

    @Composable
    fun MainUi() {
        Column(
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Button(
                onClick = {
                    startRecord()
                },
            ) {
                Text(text = "Start")
            }

            Button(onClick = {
                stopRecord()
            }) {
                Text(text = "Stop")
            }
        }
    }

    private fun startRecord() {

    }

    private fun stopRecord() {

    }

    private fun checkAudioRecordPermission() {

    }
}
