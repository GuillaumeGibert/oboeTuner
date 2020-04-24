#ifndef _WORKER_FFT_H
#define _WORKER_FFT_H

#include "workers/Worker.h"
#include "FFT.h"

class WorkerFFT : public Worker
{
	Q_OBJECT

public:
	WorkerFFT();
	~WorkerFFT();

protected:
	void doWork();
	bool init();

public slots:
	void setSignalValues(std::vector<std::vector<float>> vSignalValues);
	void setFps(float);
	void setNbSignals(int);
	

signals:
	void sigBroadcastPowerSpectrumValues(std::vector<std::vector<float>>);

private:
	FFT* m_pFFT;
	std::vector<float> m_vSignalValues;
	float m_fFps;
	
};

#endif