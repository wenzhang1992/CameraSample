#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QVideoSurfaceFormat>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pCameraDevice = new CameraDevice(this);

    m_pCamera = new QCamera(this);

    m_pCamera->setViewfinder(m_pCameraDevice);

    SetImageProcessCallback();

    QVideoSurfaceFormat format;

    m_pCameraDevice->start(format);

    m_pCamera->start();

    m_displayTimer = new PerformanceTimer(this);

    m_ucImageDataRGB32 = new unsigned char[1280 * 720 * 4];

    m_ucImageDataRGB24 = new unsigned char[1280 * 720 * 3];

    m_displayTimer->start(40);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_displayTimer;

    delete m_cImageBufferRGB32;

    delete[] m_ucImageDataRGB32;

    delete[] m_ucImageDataRGB24;

    delete m_cImageBufferRGB24;

    m_imageProcessWarpper->SetStatus(false);

    if (m_qtImageProcessThread->isRunning())
    {
        m_qtImageProcessThread->quit();
        delete m_qtImageProcessThread;
    }

    if (m_recordFile->isOpen())
    {
        m_recordFile->close();
    }

    delete m_recordFile;
}

void MainWindow::PushImageData(unsigned char *Buffer, int size)
{
    int full = 0;
    m_cImageBufferRGB32->PushBack(Buffer, size, full);
}

void MainWindow::DisplayImage()
{
    int size = 1280 * 720 * 4;

    m_cImageBufferRGB32->GetFront(m_ucImageDataRGB32, size);

    ui->DisplayWindow->UpdateImage(m_ucImageDataRGB32, 1280, 720);
}

void MainWindow::SetImageProcessCallback()
{
    m_cImageBufferRGB32 = new CCycleQueue();

    m_cImageBufferRGB24 = new CCycleQueue();

    m_cImageBufferRGB32->InitQueue(1280 * 720 * 4);

    m_cImageBufferRGB24->InitQueue(1280 * 720 * 3);

    m_pCameraDevice->m_pImageProcess = [](void *Buffer, int size, void *Owner) -> void {
        unsigned char *ucBuffer = reinterpret_cast<unsigned char *>(Buffer);
        MainWindow *pOwner = reinterpret_cast<MainWindow *>(Owner);
        //送显示缓存
        pOwner->PushImageData(reinterpret_cast<unsigned char *>(Buffer), size);
        //送编码缓存
        for (int i = 0; i < 720; i++)
        {
            for (int j = 0; j < (1280 * 4); j += 4)
            {
                pOwner->m_ucImageDataRGB24[i * 720 + j] = ucBuffer[j + 1];
                pOwner->m_ucImageDataRGB24[i * 720 + j + 1] = ucBuffer[j + 2];
                pOwner->m_ucImageDataRGB24[i * 720 + j + 2] = ucBuffer[j + 3];
            }
        }
        int full = 0;
        pOwner->m_cImageBufferRGB24->PushBack(pOwner->m_ucImageDataRGB24, 1280 * 720 * 3, full);
        return;
    };
}

void MainWindow::ConfigImageProcess()
{
    m_imageProcessWarpper = new ImageProcessWarpper(m_cImageBufferRGB24);

    m_qtImageProcessThread = new QThread();

    m_imageProcessWarpper->moveToThread(m_qtImageProcessThread);

    connect(m_qtImageProcessThread, SIGNAL(finished()), m_imageProcessWarpper, SLOT(deleteLater()));

    connect(m_qtImageProcessThread, SIGNAL(finished()), m_qtImageProcessThread, SLOT(deleteLater()));

    connect(this, SIGNAL(sig_startRecord(QString)), m_imageProcessWarpper, SLOT(slot_processing(QString)));

    m_qtImageProcessThread->start();

    // connect()
}

void MainWindow::on_recordButton_clicked()
{
    m_recordFile = new QFile(m_qsFilePath + "/RecordFile.avi");
    if (!m_recordFile->open(QIODevice::ReadWrite))
    {
        return;
    }
    m_recordFile->close();

    m_imageProcessWarpper->SetStatus(true);

    emit(sig_startRecord(m_qsFilePath + "/RecordFile.avi"));
}
