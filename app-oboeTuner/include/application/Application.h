/**
* \file Application.h
* \brief Defines Application
* \author Guillaume Gibert
* \date 27/01/2020
*/

#ifndef _APPLICATION_H
#define _APPLICATION_H

// Window
#include "interface/MainWindow.h"

// Qt
#include <QThread>

#include "workers/WorkerSignalGenerator.h"
#include "workers/WorkerMusicPlayer.h"
#include "workers/WorkerFFT.h"

#define DO_FREQ 261.6
#define RE_FREQ 293.7
#define MI_FREQ 329.6
#define FA_FREQ 349.2
#define SOL_FREQ 392.0
#define LA_FREQ 440.0
#define SI_FREQ 493.9

enum Notes
{
	DO,
	RE,
	MI,
	FA,
	SOL,
	LA,
	SI
};


/**
* \class Application
* \brief The object which represents the application and manages it.
*/
class Application: public QObject
{
	Q_OBJECT

public:
	/**
	* \brief The constructor of the object.
	*/
	Application();

	/**
	* \brief The delete of the object.
	*/
	~Application();

	/**
	* \brief The function that initialize the application.
	*/
	void init(MainWindow* window);



private:
	void registerMetaTypes();
	bool loadConfigFile(QString sConfigFilename);
	void initWorkers();
	void setWorkerConnections();
	void moveWorkersToThread();
	void stopWorkerThreads();
	void deleteWorkers();

signals:
	
	// -------------------- SIGNALS FOR THE WINDOW --------------------
	void sigBroadcastSignalFeatures(float fFps, float fDuration, float fFrequency, int nbHarmonics, std::vector<float> vAmplitude, std::vector<float> vPhase);
	

public slots:

	// -------------------- WINDOW SLOTS --------------------	
	void setSelectedInstrumentAndNote(int, int);
	


public:
	
private:
	MainWindow* m_window = nullptr;						/**< The main window. */
	
	WorkerSignalGenerator* m_pWorkerSignalGenerator;
	QThread m_TWorkerSignalGenerator;

	WorkerMusicPlayer* m_pWorkerMusicPlayer;
	QThread m_TWorkerMusicPlayer;

	WorkerFFT* m_pWorkerFFT;
	QThread m_TWorkerFFT;

	bool m_bIsConfigFileLoaded;
	int m_i32NbHarmonics;
	float m_fDuration;
	float m_fSamplingRate;
	std::vector<float> m_vDefaultHarmonicsAmplitudes;
	std::vector<float> m_vDefaultHarmonicsPhases;
	std::vector<float> m_vInvFrequencyHarmonicsAmplitudes;
	std::vector<float> m_vInvFrequencyHarmonicsPhases;
	std::vector<float> m_vOboeHarmonicsAmplitudes;
	std::vector<float> m_vOboeHarmonicsPhases;
};

#endif