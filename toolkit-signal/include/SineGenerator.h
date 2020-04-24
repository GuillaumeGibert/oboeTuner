#ifndef SINE_GENERATOR_H
#define SINE_GENERATOR_H

#include <QObject>
#include <QDebug>


#define _USE_MATH_DEFINES
#include "math.h"

class SineGenerator : public QObject
{
    Q_OBJECT

public:
    SineGenerator();
	SineGenerator(float fFps, float fFrequency, float fAmplitude, float fPhase, float fDuration);
    ~SineGenerator();

public:
	std::vector<float> generate();

public slots:
	void setFps(float);
	void setFrequency(float);
	void setAmplitude(float);
	void setPhase(float);
	void setDuration(float);

signals:
   

private:
	float m_fFps;
	float m_fFrequency;
	float m_fAmplitude;
	float m_fPhase;
	float m_fDuration;
};

#endif // SINE_GENERATOR_H
