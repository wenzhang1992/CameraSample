#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../CameraDevice/CameraDevice.h"
#include <QCamera>
#include "CycleQueue.h"
#include "showwidget.h"
#include "PerformanceTimer.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void PushImageData(unsigned char *Buffer,int size);

    void DisplayImage();
private:
    Ui::MainWindow *ui;

    CameraDevice *m_pCameraDevice;

    QCamera *m_pCamera;

    void SetImageProcessCallback();

    CCycleQueue *m_cImageBuffer=nullptr;

    PerformanceTimer *m_displayTimer=nullptr;

    unsigned char *m_ucImageData=nullptr;
};

#endif // MAINWINDOW_H
