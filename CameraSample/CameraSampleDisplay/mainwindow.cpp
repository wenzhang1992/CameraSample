#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVideoSurfaceFormat>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pCameraDevice=new CameraDevice(this);

    m_pCamera=new QCamera(this);

    m_pCamera->setViewfinder(m_pCameraDevice);

    SetImageProcessCallback();

    QVideoSurfaceFormat format;

    m_pCameraDevice->start(format);

    m_pCamera->start();

    m_displayTimer=new PerformanceTimer(this);

    m_ucImageData=new unsigned char[1280*720*4];

    m_displayTimer->start(40);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_displayTimer;

    delete m_cImageBuffer;

    delete []m_ucImageData;

}

void MainWindow::PushImageData(unsigned char *Buffer, int size)
{
    int full=0;
    m_cImageBuffer->PushBack(Buffer,size,full);
}

void MainWindow::DisplayImage()
{
    int size=1280*720*4;

    m_cImageBuffer->GetFront(m_ucImageData,size);

    ui->DisplayWindow->UpdateImage(m_ucImageData,1280,720);
}

void MainWindow::SetImageProcessCallback()
{
    m_cImageBuffer=new CCycleQueue();

    m_cImageBuffer->InitQueue(1280*720*4);

    m_pCameraDevice->m_pImageProcess=[](void *Buffer,int size,void *Owner)-> void {
        MainWindow *pOwner=reinterpret_cast<MainWindow*>(Owner);
        pOwner->PushImageData(reinterpret_cast<unsigned char*>(Buffer),size);
        return;
    };
}


