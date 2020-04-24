#ifndef _WORKER_MUSIC_PLAYER_H
#define _WORKER_MUSIC_PLAYER_H

#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QAudioOutput>

#include "workers/Worker.h"

class WorkerMusicPlayer : public Worker
{
	Q_OBJECT

public:
	WorkerMusicPlayer();
	WorkerMusicPlayer(float fSampleRate, float fDuration);
	~WorkerMusicPlayer();

protected:
	void doWork();
	bool init();

public slots:
	void play();
	void setSignalValues(std::vector<float> vSignalValues);
	void setFps(float);
	void setDuration(float);

signals:
	

private:
	std::vector<float> m_vSignalValues;
	float m_fFps;
	float m_fDuration;
	QAudioFormat m_oAudioFormat;
	QAudioOutput* m_pAudioOutput;
	QByteArray* m_pByteBuffer;
	int m_i32NbSamples;
};

#endif