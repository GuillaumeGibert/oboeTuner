#include "SineGenerator.h"

SineGenerator::SineGenerator()
{
	// default values
	m_fFps = 10.0;
	m_fFrequency = 1.0;
	m_fAmplitude = 1.0;
	m_fPhase = 0.0;
	m_fDuration = 1.0;
}

SineGenerator::SineGenerator(float fFps, float fFrequency, float fAmplitude, float fPhase, float fDuration)
{
	// default values
	m_fFps = fFps;
	m_fFrequency = fFrequency;
	m_fAmplitude = fAmplitude;
	m_fPhase = fPhase;
	m_fDuration = fDuration;
}


SineGenerator::~SineGenerator()
{

}

void SineGenerator::setFps(float fFps)
{
	m_fFps = fFps;
}

void SineGenerator::setFrequency(float fFrequency)
{
	m_fFrequency = fFrequency;
}

void SineGenerator::setAmplitude(float fAmplitude)
{
	m_fAmplitude = fAmplitude;
}

void SineGenerator::setPhase(float fPhase)
{
	m_fPhase = fPhase;
}

void SineGenerator::setDuration(float fDuration)
{
	m_fDuration = fDuration;
}

std::vector<float> SineGenerator::generate()
{
	std::vector<float> vSignalValues;
	
	int l_i32NbSamples = m_fDuration * m_fFps;

	// amplitude * sin (2 * pi * frequency * time + phase)
    for (int l_sample = 0; l_sample < l_i32NbSamples; l_sample++)
        vSignalValues.push_back(m_fAmplitude * sin (2.0f * M_PI * m_fFrequency * (float)l_sample / m_fFps + m_fPhase));

    return vSignalValues;
}



