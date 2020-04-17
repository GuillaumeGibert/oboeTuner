#ifndef BUFFERING_H
#define BUFFERING_H

#include <QObject>
#include <QDebug>

#include <deque>

class Buffering : public QObject
{
    Q_OBJECT

public:
    Buffering();
    ~Buffering();

public slots:
    void setSignalValues(std::vector<float> vSignalValues);

signals:
    void sigBroadcastBufferedSignalValues(std::vector<std::deque<float>>);

public:
    void setDuration(float);
    void setShift(float);
    void setFps(float);
    void setNbSignals(int);

protected:
    float m_fDuration;
    float m_fShift;
    float m_fFps;
    int m_i32NbSignals;
    std::vector<std::deque<float>> m_vBufferedSignal;
    int m_i32SampleIndex;
    int m_i32ShiftIndex;
    bool m_bIsBufferReady;
    bool m_bIsFirstBuffer;

};

#endif

