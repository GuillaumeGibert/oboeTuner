#include "Buffering.h"

Buffering::Buffering()
{
    m_fDuration = 0.0f;
    m_fShift = 0.0f;
    m_fFps = 0.0f;
    m_i32NbSignals = 0;
    m_i32SampleIndex = 0;
    m_i32ShiftIndex = 0;
    m_bIsBufferReady = false;
    m_bIsFirstBuffer = true;
}

Buffering::~Buffering()
{

}

void Buffering::setDuration(float fDuration)
{
    m_fDuration = fDuration;
}

void Buffering::setShift(float fShift)
{
    m_fShift = fShift;
}

void Buffering::setFps(float fFps)
{
    m_fFps = fFps;
}

void Buffering::setNbSignals(int i32NbSignals)
{
    m_i32NbSignals = i32NbSignals;
    m_vBufferedSignal.resize(i32NbSignals + 1);
}

void Buffering::setSignalValues(std::vector<float> vInputData)
{
    if (m_fDuration == 0.0f || m_fShift == 0.0f || m_fFps == 0.0f || m_i32NbSignals == 0)
    {
        qDebug() << "[ERROR] (Buffering::setSignalValues) Buffering module not initialized correctly!";
        qDebug() << "[INFO] Please check the values of these variables: m_fDuration, m_fShift, m_fFps, m_i32NbSignals";
        return;
    }

    // computes relative time for each sample
    if (m_bIsFirstBuffer)
    {
        m_vBufferedSignal[0].push_back((float)m_i32SampleIndex / m_fFps);
        m_i32SampleIndex++;
    }

    // gets the most recent sample
    for (int l_signal=0; l_signal < m_i32NbSignals; l_signal++)
        m_vBufferedSignal[l_signal+1].push_back(vInputData[l_signal]);

    m_i32ShiftIndex++;

    // removes the oldest sample
    if (!m_bIsFirstBuffer)
    {
        for (int l_signal=0; l_signal < m_i32NbSignals; l_signal++)
            m_vBufferedSignal[l_signal+1].pop_front();
    }

    // checks if buffer is ready
    if (m_bIsFirstBuffer && m_vBufferedSignal[0].size() == m_fDuration * m_fFps)
    {
        m_bIsBufferReady = true;
        m_bIsFirstBuffer = false;
    }
    else if (!m_bIsFirstBuffer && m_i32ShiftIndex == m_fShift * m_fFps)
    {
        m_bIsBufferReady = true;
    }

    // sends buffer if ready
    if (m_bIsBufferReady)
    {
        emit sigBroadcastBufferedSignalValues(m_vBufferedSignal);
        m_bIsBufferReady = false;
        m_i32SampleIndex = 0;
        m_i32ShiftIndex = 0;
    }


}
