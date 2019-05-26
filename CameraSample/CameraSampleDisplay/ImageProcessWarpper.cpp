#include "ImageProcessWarpper.h"

ImageProcessWarpper::ImageProcessWarpper(QObject *parent) : QObject(parent)
{
}

ImageProcessWarpper::ImageProcessWarpper(CCycleQueue *Buffer, QObject *parent) : QObject(parent)
{
    m_cImageBuffer = Buffer;

    m_imageProcesser = new ImageProcessLib();

    m_ucImageBufferRGB = new unsigned char[1280 * 720 * 3];

    m_ucImageBufferYUV = new unsigned char[1280 * 720 * 1.5];
}

void ImageProcessWarpper::SetStatus(bool status)
{
    QMutexLocker lock(&m_locker);

    m_bStatus = status;
}

void ImageProcessWarpper::slot_processing(QString filename)
{
    char *fileName = filename.toLatin1().data();

    m_imageProcesser->InitLib(fileName, 1280, 720);

    int imageWidth = 1280;

    int imageHeight = 720;

    int imageSize = imageHeight * imageWidth;

    int size = 0;
    while (m_bStatus)
    {
        if (m_cImageBuffer->IsEmpty() == false)
        {
            size = imageSize * 3;
            m_cImageBuffer->GetFront(m_ucImageBufferRGB, size);

            ImageConvert::RGB24ToYV12(m_ucImageBufferRGB,
                                      m_ucImageBufferYUV,
                                      imageWidth,
                                      imageHeight);
            m_imageProcesser->EncodeFrame(reinterpret_cast<uint8_t *>(m_ucImageBufferYUV));
        }
    }
    m_imageProcesser->Uninit();
}
