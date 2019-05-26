#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QImage>

//#include "commondef.h"
//#define VideoFrameWidth 320
//#define VideoFrameHeight 256
class ShowWidget;

/**********************************图像界面处理类*********************************************/
class ImageProcess: public QObject
{
public:
   explicit ImageProcess(ShowWidget *parent = nullptr);

   void PaintRect(int x, int y, int W, int H, QColor color, Qt::PenStyle style,QPainter *painter);
private:
};

/**********************************显示窗口*********************************************/

class ShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowWidget(QWidget *parent = nullptr);
    void UpdateImage(unsigned char *data,int width,int height);
    ~ShowWidget();
signals:

private:
    QImage m_qShowImage;

    ImageProcess *m_imageProcess;

    unsigned char *m_ucImageBuffer;
    int m_iWidth=0;
    int m_iHeight=0;
    QVector<QVector<QRgb> >   m_colorTableList; 
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // SHOWWIDGET_H
