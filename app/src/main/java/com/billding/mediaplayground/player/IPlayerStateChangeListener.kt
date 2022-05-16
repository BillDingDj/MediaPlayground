package com.billding.mediaplayground.player

interface IPlayerStateChangeListener {

    fun onPlay()

    fun onStop()

    fun onComplete()
}