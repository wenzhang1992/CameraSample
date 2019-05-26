#ifndef IMAGEPROCESSWARPPER_H
#define IMAGEPROCESSWARPPER_H

#include "../ImageProcessLib/ImageProcessLib.h"
#include "CycleQueue.h"
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
class ImageProcessWarpper : public QObject
{
    Q_OBJECT
  public:
    ImageProcessWarpper(QObject *parent = 0);

    ImageProcessWarpper(CCycleQueue *Buffer, QObject *parent = 0);

    void SetStatus(bool status);

  private:
    ImageProcessLib *m_imageProcesser = nullptr;

    CCycleQueue *m_cImageBuffer = nullptr;

    bool m_bStatus = false;

    QMutex m_locker;

    unsigned char *m_ucImageBufferRGB = nullptr;

    unsigned char *m_ucImageBufferYUV = nullptr;
  public slots:
    void slot_processing(QString filename);
};

#endif // IMAGEPROCESSWARPPER_H
