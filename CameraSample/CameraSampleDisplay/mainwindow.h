#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../CameraDevice/CameraDevice.h"
#include "CycleQueue.h"
#include "ImageProcessWarpper.h"
#include "PerformanceTimer.h"
#include "showwidget.h"
#include <QCamera>
#include <QFile>
#include <QMainWindow>
#include <QObject>
#include <QThread>
namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void PushImageData(unsigned char *Buffer, int size);

    void DisplayImage();

  private:
    Ui::MainWindow *ui;

    CameraDevice *m_pCameraDevice;

    QCamera *m_pCamera;

    void SetImageProcessCallback();

    CCycleQueue *m_cImageBufferRGB32 = nullptr;

    CCycleQueue *m_cImageBufferRGB24 = nullptr;

    PerformanceTimer *m_displayTimer = nullptr;

    unsigned char *m_ucImageDataRGB32 = nullptr;

    unsigned char *m_ucImageDataRGB24 = nullptr;

    QThread *m_qtImageProcessThread = nullptr;

    ImageProcessWarpper *m_imageProcessWarpper = nullptr;

    void ConfigImageProcess();

    QString m_qsFilePath = "C:/Users/ALEX/Desktop/RecordFile";

    QFile *m_recordFile = nullptr;
  signals:
    void sig_startRecord(QString filename);
  private slots:
    void on_recordButton_clicked();
};

#endif // MAINWINDOW_H
