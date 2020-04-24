#include "workers/WorkerFFT.h"

WorkerFFT::WorkerFFT(): 
	m_pFFT(nullptr),
	m_fFps(0.0)
{
	init();
}

WorkerFFT::~WorkerFFT()
{
	if (nullptr != m_pFFT)
	{
		delete m_pFFT;
		m_pFFT = nullptr;
	}
}

void WorkerFFT::doWork()
{

}

bool WorkerFFT::init()
{
	if (nullptr == m_pFFT)
	{
		m_pFFT = new FFT();
		m_pFFT->setNbSignals(1);
		m_pFFT->setFps(8000);

		
		m_bIsWorkerInitialized = true;
	}

	

	return m_bIsWorkerInitialized;
}

void WorkerFFT::setFps(float fFps)
{
	m_oWorkerMutex.lockForWrite();
		m_pFFT->setFps(fFps);
	m_oWorkerMutex.unlock();
}

void WorkerFFT::setNbSignals(int i32NbSignals)
{
	m_oWorkerMutex.lockForWrite();
		m_pFFT->setNbSignals(i32NbSignals);
	m_oWorkerMutex.unlock();
}

void WorkerFFT::setSignalValues(std::vector<std::vector<float>> vSignalValues)
{
	m_oWorkerMutex.lockForWrite();
		m_pFFT->setBufferedSignalValues(vSignalValues);
		std::vector<std::vector<float>> l_vPowerSpectrumValues = m_pFFT->getPowerSpectrumValues();
		emit sigBroadcastPowerSpectrumValues(l_vPowerSpectrumValues);
	m_oWorkerMutex.unlock();
}

