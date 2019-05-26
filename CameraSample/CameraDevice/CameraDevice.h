#ifndef CAMERADEVICE_H
#define CAMERADEVICE_H

#include "cameradevice_global.h"
#include <QAbstractVideoSurface>
class CAMERADEVICESHARED_EXPORT CameraDevice :public QAbstractVideoSurface
{
    Q_OBJECT
public:

    CameraDevice(QObject *parent=0);

    ~CameraDevice();

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;

    bool present(const QVideoFrame &frame) override;

    bool start(QVideoSurfaceFormat &format);

    typedef void (*ImageReceiveCallback)(void *Buffer,int size,void *Owner);

    ImageReceiveCallback m_pImageProcess;
private:
    QVideoFrame::PixelFormat m_currentPixelFormat;

    unsigned char *m_ucImageBuffer=nullptr;

    void *pOwner;
};

#endif // CAMERADEVICE_H
