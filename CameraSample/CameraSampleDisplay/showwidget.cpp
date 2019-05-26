#include "showwidget.h"

ShowWidget::ShowWidget(QWidget *parent) : QWidget(parent)
{
    m_ucImageBuffer=new unsigned char[1280*720*4];

    m_imageProcess=new ImageProcess(this);

//    for(int i=0;i<9;++i)
//    {
//        QVector<QRgb>  tmpVector;
//        for(int j=0;j<256;++j)
//        {

//           int b = GdImageLib::IRImage_ColorTable[i][j][0];
//           int g = GdImageLib::IRImage_ColorTable[i][j][1];
//           int r = GdImageLib::IRImage_ColorTable[i][j][2];
//           tmpVector.push_back(qRgb(r,g,b));

//        }
//        m_colorTableList.push_back(tmpVector);
//    }
}

ShowWidget::~ShowWidget()
{
    delete []m_ucImageBuffer;
    m_ucImageBuffer=NULL;
}

void ShowWidget::UpdateImage(unsigned char *data, int width,int height)
{
    memcpy(m_ucImageBuffer,data,width*height*4);
    this->m_iHeight=height;
    this->m_iWidth=width;
    update();
}


void ShowWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, this->width(), this->height()); //先画成黑色
    QImage bmpImg=QImage(m_ucImageBuffer,this->m_iWidth, this->m_iHeight,QImage::Format_RGB32);
    if(bmpImg.isNull())
    {
        return;
    }
    if (bmpImg.size().width() <= 0) return;

    QImage img = bmpImg.scaled(this->size(),Qt::IgnoreAspectRatio);

    painter.drawImage(QPoint(0,0),img.mirrored()); //画出图像

    QWidget::paintEvent(event);

}

ImageProcess::ImageProcess(ShowWidget *parent)
{
    this->setParent(parent);
}

void ImageProcess::PaintRect(int x, int y, int W, int H, QColor color, Qt::PenStyle style,QPainter *painter)
{
    painter->setBrush(Qt::NoBrush);

    QPen pen;

    pen.setColor(color);

    pen.setStyle(style);

    painter->setPen(pen);

    painter->drawRect(x,y,W,H);
}
