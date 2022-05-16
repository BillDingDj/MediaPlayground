//
// Created by admin on 2022/5/15.
//

#ifndef MEDIAPLAYGROUND_VIDEODECODER_H
#define MEDIAPLAYGROUND_VIDEODECODER_H

extern "C" {
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavcodec/jni.h>
};

#include "libavutil/frame.h"
#include "BaseDecoder.h"
#include "VideoRender.h"

class VideoDecoder : public BaseDecoder {

public:
    VideoDecoder(char *url){
        Init(url, AVMEDIA_TYPE_VIDEO);
    }

    virtual ~VideoDecoder(){
        UnInit();
    }

    int GetVideoWidth()
    {
        return m_VideoWidth;
    }

    int GetVideoHeight()
    {
        return m_VideoHeight;
    }

    void SetVideoRender(VideoRender *videoRender)
    {
        m_VideoRender = videoRender;
    }

private:
    virtual void OnDecoderReady();
    virtual void OnDecoderDone();
    virtual void OnFrameAvailable(AVFrame *frame);

    const AVPixelFormat DST_PIXEL_FORMAT = AV_PIX_FMT_RGBA;

    int m_VideoWidth = 0;
    int m_VideoHeight = 0;

    int m_RenderWidth = 0;
    int m_RenderHeight = 0;

    AVFrame *m_RGBAFrame = nullptr;
    uint8_t *m_FrameBuffer = nullptr;

    SwsContext *m_SwsContext = nullptr;

    VideoRender *m_VideoRender = nullptr;
};


#endif //MEDIAPLAYGROUND_VIDEODECODER_H
