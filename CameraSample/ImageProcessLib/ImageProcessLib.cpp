#include "ImageProcessLib.h"
#include <QDebug>
ImageProcessLib::ImageProcessLib()
{
}

bool ImageProcessLib::InitLib(char *filename, int width, int height)
{
    m_cfileName = filename;
    m_iImageHeight = height;
    m_iImageWidth = width;

    avcodec_register_all();
    av_register_all();

    avformat_alloc_output_context2(&m_pFormatCtx, NULL, NULL, filename);

    m_pOutputFmt = m_pFormatCtx->oformat;

    if (avio_open(&m_pFormatCtx->pb, filename, AVIO_FLAG_READ_WRITE))
    {
        qDebug() << "output file open fail";
        return false;
    }

    m_pAVStream = avformat_new_stream(m_pFormatCtx, 0);

    if (m_pAVStream == NULL)
    {
        qDebug() << "failed allocating output stream";
        return false;
    }

    m_pAVStream->time_base.num = 1;

    m_pAVStream->time_base.den = 1;

    m_pCodecCtx = m_pAVStream->codec;

    m_pCodecCtx->codec_id = m_pOutputFmt->video_codec;

    m_pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;

    m_pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

    m_pCodecCtx->width = m_iImageWidth;

    m_pCodecCtx->height = m_iImageHeight;

    m_pCodecCtx->time_base.num = 1;

    m_pCodecCtx->time_base.den = 25;

    m_pCodecCtx->bit_rate = 400000;

    m_pCodecCtx->gop_size = 12;

    if (m_pCodecCtx->codec_id == AV_CODEC_ID_H264)
    {
        m_pCodecCtx->qmin = 10;
        m_pCodecCtx->qmax = 51;
        m_pCodecCtx->qcompress = 0.6;
    }
    if (m_pCodecCtx->codec_id == AV_CODEC_ID_MPEG2VIDEO)
        m_pCodecCtx->max_b_frames = 2;
    if (m_pCodecCtx->codec_id == AV_CODEC_ID_MPEG1VIDEO)
        m_pCodecCtx->mb_decision = 2;

    m_pCodec = avcodec_find_encoder(m_pCodecCtx->codec_id);
    if (!m_pCodec)
    {
        qDebug() << "no right encodec";
        return false;
    }

    if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)
    {
        qDebug() << "open encoder fail";
        return false;
    }

    //初始化帧

    m_pFrame = av_frame_alloc();
    m_pFrame->width = m_pCodecCtx->width;
    m_pFrame->height = m_pCodecCtx->height;
    m_pFrame->format = m_pCodecCtx->pix_fmt;
    m_iYSize = avpicture_get_size(m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height);

    m_ucPkt = (uint8_t *)av_malloc(m_iYSize);

    avpicture_fill((AVPicture *)m_pFrame, m_ucPkt, m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height);

    avformat_write_header(m_pFormatCtx, NULL);

    //创建已编码帧
    int y_size = m_pCodecCtx->width * m_pCodecCtx->height;
    av_new_packet(m_pAVPacket, y_size * 3);

    return true;
}

void ImageProcessLib::EncodeFrame(uint8_t *inputData)
{
    int y_size = m_pCodecCtx->width * m_pCodecCtx->height;

    memcpy(m_ucPkt, inputData, m_iImageHeight * m_iImageWidth * (3 / 2));

    m_pFrame->data[0] = m_ucPkt;
    m_pFrame->data[1] = m_ucPkt + y_size;
    m_pFrame->data[2] = m_ucPkt + y_size * (5 / 4);

    m_pFrame->pts = m_iFrameCount;

    m_iFrameCount++;

    int got_picture = 0;

    int ret = avcodec_encode_video2(m_pCodecCtx, m_pAVPacket, m_pFrame, &got_picture);

    if (ret < 0)
    {
        qDebug() << "encoder fail";
        m_iFrameCount = 0;
    }

    if (got_picture == 1)
    {
        qDebug() << "encoder success";

        m_pAVPacket->stream_index = m_pAVStream->index;

        av_packet_rescale_ts(m_pAVPacket, m_pCodecCtx->time_base, m_pAVStream->time_base);

        m_pAVPacket->pos = -1;

        ret = av_interleaved_write_frame(m_pFormatCtx, m_pAVPacket);

        //av_free_packet(m_pAVPacket);
    }
}

void ImageProcessLib::Uninit()
{
    int ret = FlushEncoder(m_pFormatCtx, 0);

    if (ret < 0)
    {
        qDebug() << "flushing encoder failed";
    }

    av_write_trailer(m_pFormatCtx);

    if (m_pAVStream)
    {
        avcodec_close(m_pAVStream->codec);

        av_free(m_pFrame);

        av_free(m_ucPkt);
    }

    if (m_pFormatCtx)
    {
        avio_close(m_pFormatCtx->pb);

        avformat_free_context(m_pFormatCtx);
    }

    //fclose(m_cfileName);
}

int ImageProcessLib::FlushEncoder(AVFormatContext *fmt_ctx, unsigned int stream_index)
{
    int ret;
    int got_frame;
    AVPacket enc_pkt;
    //    if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &
    //          CODEC_CAP_DELAY))
    //        return 0;
    while (1)
    {
        qDebug() << "Flushing stream encoder" << QString::number(stream_index, 10);
        enc_pkt.data = NULL;
        enc_pkt.size = 0;
        av_init_packet(&enc_pkt);
        ret = avcodec_encode_video2(fmt_ctx->streams[stream_index]->codec, &enc_pkt,
                                    NULL, &got_frame);
        av_frame_free(NULL);
        if (ret < 0)
            break;
        if (!got_frame)
        {
            ret = 0;
            break;
        }
        qDebug() << "success encoder 1 frame";

        // parpare packet for muxing
        enc_pkt.stream_index = stream_index;
        av_packet_rescale_ts(&enc_pkt,
                             fmt_ctx->streams[stream_index]->codec->time_base,
                             fmt_ctx->streams[stream_index]->time_base);
        ret = av_interleaved_write_frame(fmt_ctx, &enc_pkt);
        if (ret < 0)
            break;
    }
    return ret;
}

bool ImageConvert::YV12ToRGB24_Native(unsigned char *pYUV, unsigned char *pBGR24, int width, int height)
{
    if (width < 1 || height < 1 || pYUV == NULL || pBGR24 == NULL)
        return false;
    const long len = width * height;
    unsigned char *yData = pYUV;
    unsigned char *vData = &yData[len];
    unsigned char *uData = &vData[len >> 2];

    int bgr[3];
    int yIdx, uIdx, vIdx, idx;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            yIdx = i * width + j;
            vIdx = (i / 2) * (width / 2) + (j / 2);
            uIdx = vIdx;

            bgr[0] = (int)(yData[yIdx] + 1.732446 * (uData[vIdx] - 128));                                  // b分量
            bgr[1] = (int)(yData[yIdx] - 0.698001 * (uData[uIdx] - 128) - 0.703125 * (vData[vIdx] - 128)); // g分量
            bgr[2] = (int)(yData[yIdx] + 1.370705 * (vData[uIdx] - 128));                                  // r分量

            for (int k = 0; k < 3; k++)
            {
                idx = (i * width + j) * 3 + k;
                if (bgr[k] >= 0 && bgr[k] <= 255)
                    pBGR24[idx] = bgr[k];
                else
                    pBGR24[idx] = (bgr[k] < 0) ? 0 : 255;
            }
        }
    }
    return true;
}

bool ImageConvert::YV12ToRGB24_Table(unsigned char *pYUV, unsigned char *pBGR24, int width, int height)
{
    if (width < 1 || height < 1 || pYUV == NULL || pBGR24 == NULL)
        return false;
    const long len = width * height;
    unsigned char *yData = pYUV;
    unsigned char *vData = &yData[len];
    unsigned char *uData = &vData[len >> 2];

    int bgr[3];
    int yIdx, uIdx, vIdx, idx;
    int rdif, invgdif, bdif;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            yIdx = i * width + j;
            vIdx = (i / 2) * (width / 2) + (j / 2);
            uIdx = vIdx;

            rdif = Table_fv1[vData[vIdx]];
            invgdif = Table_fu1[uData[uIdx]] + Table_fv2[vData[vIdx]];
            bdif = Table_fu2[uData[uIdx]];

            bgr[0] = yData[yIdx] + bdif;
            bgr[1] = yData[yIdx] - invgdif;
            bgr[2] = yData[yIdx] + rdif;

            for (int k = 0; k < 3; k++)
            {
                idx = (i * width + j) * 3 + k;
                if (bgr[k] >= 0 && bgr[k] <= 255)
                    pBGR24[idx] = bgr[k];
                else
                    pBGR24[idx] = (bgr[k] < 0) ? 0 : 255;
            }
        }
    }
    return true;
}

unsigned char clip_value(unsigned char x, unsigned char min_val, unsigned char max_val)
{
    if (x > max_val)
    {
        return max_val;
    }
    else if (x < min_val)
    {
        return min_val;
    }
    else
    {
        return x;
    }
}

bool ImageConvert::RGB24ToYV12(unsigned char *pRGB, unsigned char *pYUV, int width, int height)
{
    unsigned char *ptrY, *ptrU, *ptrV, *ptrRGB;
    memset(pYUV, 0, width * height * 3 / 2);
    ptrY = pYUV;
    ptrU = pYUV + width * height;
    ptrV = ptrU + (width * height * 1 / 4);
    unsigned char y, u, v, r, g, b;
    for (int j = 0; j < height; j++)
    {
        ptrRGB = pRGB + width * j * 3;
        for (int i = 0; i < width; i++)
        {
            r = *(ptrRGB++);
            g = *(ptrRGB++);
            b = *(ptrRGB++);
            y = (unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
            u = (unsigned char)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
            v = (unsigned char)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
            *(ptrY++) = clip_value(y, 0, 255);
            if (j % 2 == 0 && i % 2 == 0)
            {
                *(ptrU++) = clip_value(u, 0, 255);
            }
            else
            {
                if (i % 2 == 0)
                {
                    *(ptrV++) = clip_value(v, 0, 255);
                }
            }
        }
    }
    return true;
}
