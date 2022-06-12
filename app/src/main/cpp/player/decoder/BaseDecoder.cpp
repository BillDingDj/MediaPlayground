//
// Created by admin on 2022/5/15.
//

#include "BaseDecoder.h"
#include "LogUtil.h"

int BaseDecoder::Init(const char *url, AVMediaType mediaType) {
    LOGCATD("BaseDecoder::Init, url=%s", url);
    strcpy(m_Url, url);

    return 0;
}

void BaseDecoder::UnInit() {

    LOGCATD("BaseDecoder::UnInit");
    if (m_Thread) {
        Stop();
        m_Thread->join();
        delete m_Thread;
        m_Thread = nullptr;
    }
    LOGCATD("BaseDecoder::UnInit end");
}

void BaseDecoder::Start() {
    if (m_Thread == nullptr) {
        StartDecodingThread();
    } else {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_DecoderState = STATE_DECODING;
        m_Cond.notify_all();
    }
}
/**
 * 开启子线程进行解码
 */
void BaseDecoder::StartDecodingThread() {
    m_Thread = new thread(DoAVDecoding, this);
}

void BaseDecoder::DoAVDecoding(BaseDecoder *decoder) {
    LOGCATD("DecoderBase::DoAVDecoding");
    do {
        if(decoder->InitFFDecoder() != 0) {
            break;
        }
        decoder->OnDecoderReady();
        decoder->DecodingLoop();
    } while (false);

    decoder->UnInitDecoder();
    decoder->OnDecoderDone();
}

int BaseDecoder::InitFFDecoder() {

    int result = -1;
    do {
        m_AVFormatContext = avformat_alloc_context();

        if (avformat_open_input(&m_AVFormatContext, m_Url, NULL, NULL) != 0) {
            LOGCATE("BaseDecoder::InitFFDecoder avformat_open_input fail.");
            break;
        }

        if (avformat_find_stream_info(m_AVFormatContext, NULL) < 0) {
            LOGCATE("BaseDecoder::InitFFDecoder avformat_find_stream_info fail.");
            break;
        }

        for (int i = 0; i < m_AVFormatContext->nb_streams; ++i) {
            if (m_AVFormatContext->streams[i]->codecpar->codec_type == m_MediaType) {
                m_StreamIndex = i;
                break;
            }
        }

        if(m_StreamIndex == -1) {
            LOGCATE("BaseDecoder::InitFFDecoder Fail to find stream index.");
            break;
        }

        // 获取解码器参数
        AVCodecParameters *codecParameters = m_AVFormatContext->streams[m_StreamIndex]->codecpar;

        m_AVCodec = avcodec_find_decoder(codecParameters->codec_id);

        if (m_AVCodec == nullptr) {
            LOGCATE("BaseDecoder::InitFFDecoder avcodec_find_decoder fail.");
            break;
        }

        m_AVCodecContext = avcodec_alloc_context3(m_AVCodec);
        if (avcodec_parameters_to_context(m_AVCodecContext, codecParameters) != 0) {
            LOGCATE("BaseDecoder::InitFFDecoder avcodec_alloc_context3 fail.");
            break;
        }

        AVDictionary *pAVDictionary = nullptr;
        av_dict_set(&pAVDictionary, "buffer_size", "1024000", 0);
        av_dict_set(&pAVDictionary, "stimeout", "20000000", 0);
        av_dict_set(&pAVDictionary, "max_delay", "30000000", 0);
        av_dict_set(&pAVDictionary, "rtsp_transport", "tcp", 0);

        result = avcodec_open2(m_AVCodecContext, m_AVCodec, &pAVDictionary);
        if (result < 0) {
            LOGCATE("BaseDecoder::InitFFDecoder avcodec_open2 fail. result = %d", result);
            break;
        }

        result = 0;
        m_Duration = m_AVFormatContext->duration / AV_TIME_BASE * 1000;
        m_Packet = av_packet_alloc();
        m_Frame = av_frame_alloc();

    } while (false);

    return result;
}

void BaseDecoder::UnInitDecoder() {
    LOGCATE("DecoderBase::UnInitDecoder");
    if(m_Frame != nullptr) {
        av_frame_free(&m_Frame);
        m_Frame = nullptr;
    }

    if(m_Packet != nullptr) {
        av_packet_free(&m_Packet);
        m_Packet = nullptr;
    }

    if(m_AVCodecContext != nullptr) {
        avcodec_close(m_AVCodecContext);
        avcodec_free_context(&m_AVCodecContext);
        m_AVCodecContext = nullptr;
        m_AVCodec = nullptr;
    }

    if(m_AVFormatContext != nullptr) {
        avformat_close_input(&m_AVFormatContext);
        avformat_free_context(m_AVFormatContext);
        m_AVFormatContext = nullptr;
    }
}

void BaseDecoder::DecodingLoop() {

    LOGCATD("BaseDecoder::DecodingLoop start");
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_DecoderState = STATE_DECODING;
        lock.unlock();
    }

    for (;;) {
        while (m_DecoderState == STATE_PAUSE) {
            std::unique_lock<std::mutex> lock(m_Mutex);
            LOGCATD("BaseDecoder::DecodingLoop waiting");
            m_Cond.wait_for(lock, std::chrono::milliseconds(10));
            m_StartTimeStamp = GetSysCurrentTime() - m_CurTimeStamp;

        }

        if (m_DecoderState == STATE_STOP) {
            break;
        }

        if (m_StartTimeStamp == -1) {
            m_StartTimeStamp = GetSysCurrentTime();
        }

        if (DecodeOnePacket() != 0) {
            std::unique_lock<std::mutex> lock(m_Mutex);
            m_DecoderState = STATE_PAUSE;
        }
    }
    LOGCATD("BaseDecoder::DecodingLoop end");

}

void BaseDecoder::UpdateTimeStamp() {

    LOGCATD("BaseDecoder::UpdateTimeStamp");
    std::unique_lock<std::mutex> lock(m_Mutex);
    if (m_Frame->pkt_dts != AV_NOPTS_VALUE) {
        m_CurTimeStamp = m_Frame->pkt_dts;
    } else if (m_Frame->pts != AV_NOPTS_VALUE) {
        m_CurTimeStamp = m_Frame->pts;
    } else {
        m_CurTimeStamp = 0;
    }

    m_CurTimeStamp = (int64_t)((m_CurTimeStamp * av_q2d(m_AVFormatContext->streams[m_StreamIndex]->time_base)) * 1000);
    if (m_SeekPosition > 0 && m_SeekSuccess) {
        m_StartTimeStamp = GetSysCurrentTime() - m_CurTimeStamp;
        m_SeekPosition = 0;
        m_SeekSuccess = false;
    }

}

long BaseDecoder::AVSync() {
    LOGCATD("BaseDecoder::AVSync");
    long curSysTime = GetSysCurrentTime();
    long elapsedTime = curSysTime - m_CurTimeStamp;

    long delay = 0;

    if (m_CurTimeStamp > elapsedTime) {
        auto sleepTime = static_cast<unsigned  int>(m_CurTimeStamp - elapsedTime);
        sleepTime = sleepTime > DELAY_THRESHOLD ? DELAY_THRESHOLD : sleepTime;
        av_usleep(sleepTime * 1000);
    }

    delay = elapsedTime - m_CurTimeStamp;

    return delay;
}

int BaseDecoder::DecodeOnePacket() {
    LOGCATD("BaseDecoder::DecodeOnePacket");
    // 需要seek
    if (m_SeekPosition > 0) {
        int64_t  seek_target = static_cast<int64_t>(m_SeekPosition * 1000000);
        int64_t seek_min = INT64_MIN;
        int64_t seek_max = INT64_MAX;

        int seek_ret = avformat_seek_file(m_AVFormatContext, -1, seek_min, seek_target, seek_max, 0);
        if (seek_ret < 0) {
            m_SeekSuccess = false;
            LOGCATD("BaseDecoder::DecodeOnePacket error");
        } else {
            if (m_StreamIndex != -1) {
                avcodec_flush_buffers(m_AVCodecContext);
            }
            ClearCache();
            m_SeekSuccess = true;
            LOGCATD("BaseDecoder::DecodeOnePacket success");
        }
    }

    return 0;
}

void BaseDecoder::SeekToPosition(float position) {

}

void BaseDecoder::Pause() {

}

float BaseDecoder::GetCurrentPosition() {
    return m_CurTimeStamp;
}
