#ifndef PERFORMANCETIMER_H
#define PERFORMANCETIMER_H

#include <QObject>
#include <Windows.h>

class PerformanceTimer : public QObject
{
    Q_OBJECT
public:
    explicit PerformanceTimer(QObject *parent = 0);
    ~PerformanceTimer();
public slots:
    void start(int timeInterval);

    void stop();

    friend void WINAPI CALLBACK PeriodCycle(uint,uint,DWORD_PTR,DWORD_PTR,DWORD_PTR);
signals:
    void timeout();
private:
    int m_interval;
    int m_id;


};

#endif // PERFORMANCETIMER_H
