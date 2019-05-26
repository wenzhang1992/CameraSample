#include "ImageProcessLib.h"
#include <QDebug>
ImageProcessLib::ImageProcessLib()
{
}

bool ImageProcessLib::InitLib(const char *filename, int width, int height)
{
    m_iImageHeight = height;
    m_iImageWidth = width;

    avcodec_register_all();
    av_register_all();

    avformat_alloc_output_context2(&m_pFormatCtx, NULL, NULL, filename);
    m_pOutputFmt = m_pFormatCtx->oformat;

    m_pAVStream = avformat_new_stream(m_pFormatCtx, 0);

    if (m_pAVStream == NULL)
    {
        qDebug() << "failed allocating output stream";
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
}

ImageConvert::YV12ToRGB24_Native(unsigned char *pYUV, unsigned char *pBGR24, int width, int height)
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

ImageConvert::YV12ToRGB24_Table(unsigned char *pYUV, unsigned char *pBGR24, int width, int height)
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
