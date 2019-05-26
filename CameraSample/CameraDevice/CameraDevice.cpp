#include "CameraDevice.h"
#include <QVideoSurfaceFormat>
#include "QDebug"
CameraDevice::CameraDevice(QObject *parent)
{
    this->setParent(parent);

    pOwner=this->parent();

    m_ucImageBuffer=new unsigned char[1280*720*4];
}

CameraDevice::~CameraDevice()
{
    delete []m_ucImageBuffer;
}

QList<QVideoFrame::PixelFormat> CameraDevice::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    QList<QVideoFrame::PixelFormat> list;

    list<<QVideoFrame::Format_RGB32;

    list<<QVideoFrame::Format_RGB24;

    list<<QVideoFrame::Format_YUV420P;

    return list;
}

bool CameraDevice::present(const QVideoFrame &frame)
{
    QVideoFrame frameTemp(frame);

    if(frameTemp.isMapped()==false)
    {
        frameTemp.map(QAbstractVideoBuffer::ReadOnly);

       // qDebug()<<"Frame map bytes:"<<frameTemp.mappedBytes();

        m_currentPixelFormat=frameTemp.pixelFormat();

        memcpy(m_ucImageBuffer,frameTemp.bits(),frameTemp.size().width()*frameTemp.size().height()*4);

        frameTemp.unmap();

        m_pImageProcess(m_ucImageBuffer,frameTemp.size().width()*frameTemp.size().height()*4,pOwner);
    }
    return true;
}

bool CameraDevice::start(QVideoSurfaceFormat &format)
{
    format.setFrameSize(1280,720);

    format.setFrameRate(30);

    return QAbstractVideoSurface::start(format);
}


