#ifndef _WORKER_SIGNAL_GENERATOR_H
#define _WORKER_SIGNAL_GENERATOR_H

#include "workers/Worker.h"
#include "SineGenerator.h"

class WorkerSignalGenerator : public Worker
{
	Q_OBJECT

public:
	WorkerSignalGenerator();
	~WorkerSignalGenerator();

protected:
	void doWork();
	bool init();

public slots:
	void setSignalFeatures(float, float, float, int, std::vector<float>, std::vector<float>);
	void generate();

signals:
	void sigBroadcastHarmonicSignals(std::vector<std::vector<float>>);
	void sigBroadcastFullSignals(std::vector<std::vector<float>>);
	void sigBroadcastFullSignal(std::vector<float>);

private:
	SineGenerator* m_pSineGenerator;
	float m_fFps;
	float m_fDuration;
	float m_fFrequency;
	int m_i32NbHarmonics;
	std::vector<float> m_vAmplitude;
	std::vector<float> m_vPhase;

};

#endif