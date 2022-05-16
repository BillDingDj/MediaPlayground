//
// Created by admin on 2022/5/15.
//

#ifndef MEDIAPLAYGROUND_VIDEORENDER_H
#define MEDIAPLAYGROUND_VIDEORENDER_H

class VideoRender {
public:
    VideoRender(int type){
        m_RenderType = type;
    }
    virtual ~VideoRender(){}
    virtual void Init(int videoWidth, int videoHeight, int *dstSize) = 0;
//    virtual void RenderVideoFrame(NativeImage *pImage) = 0;
    virtual void UnInit() = 0;

    int GetRenderType() {
        return m_RenderType;
    }
private:
    int m_RenderType = 0;
//    int m_RenderType = VIDEO_RENDER_ANWINDOW;
};

#endif //MEDIAPLAYGROUND_VIDEORENDER_H
