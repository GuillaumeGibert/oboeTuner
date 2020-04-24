/**
* \file Application.cpp
* \brief Defines Application
* \author Guillaume Gibert
* \date 27/01/2020
*/

#include "application/Application.h"

Application::Application()
{
	m_pWorkerSignalGenerator = nullptr;
	m_pWorkerMusicPlayer = nullptr;
	m_pWorkerFFT = nullptr;
	m_bIsConfigFileLoaded = false;
	m_i32NbHarmonics = 0;
	m_fDuration = 1.0f;
	m_fSamplingRate = 8000.0;

	loadConfigFile("config.txt");
}


Application::~Application()
{
	// stops the threads
	stopWorkerThreads();

	// deletes the workers
	deleteWorkers();
}

void Application::init(MainWindow* window)
{
	m_window = window;

	initWorkers();
	registerMetaTypes();
	setWorkerConnections();
	moveWorkersToThread();

}

void Application::initWorkers()
{
	if (nullptr == m_pWorkerSignalGenerator)
	{
		m_pWorkerSignalGenerator = new WorkerSignalGenerator();
	}

	if (nullptr == m_pWorkerMusicPlayer)
	{
		m_pWorkerMusicPlayer = new WorkerMusicPlayer(m_fSamplingRate, m_fDuration);
	}

	if (nullptr == m_pWorkerFFT)
	{
		m_pWorkerFFT = new WorkerFFT();
	}
}

void Application::registerMetaTypes()
{
	// register data types
	qRegisterMetaType< std::vector<std::vector<float>> >("std::vector<std::vector<float>>");
}

bool Application::loadConfigFile(QString sConfigFilename)
{
	auto fillVariable = [](QStringList strList, QString key, auto var) -> void
	{
		if (strList[0].contains(key, Qt::CaseSensitive))
		{
			*var = strList[1].toFloat();
		}
	};

	auto fillVector = [](QStringList strList, QString key, int nbHarmonics, std::vector<float>& var) -> void
	{
		if (strList[0].contains(key, Qt::CaseSensitive))
		{
			var.clear();
			for (int l_harmonics = 0; l_harmonics < nbHarmonics; l_harmonics++)
				var.push_back(strList[1 + l_harmonics].toFloat());
		}

	};


	QFile file(sConfigFilename);

	// opens the file
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "[ERROR] (Application::loadConfigFile) Cannot open the Config File!";
		m_bIsConfigFileLoaded = false;
	}
	else
	{
		QTextStream in(&file);
		// 1st line
		QString line;
		
		while (in.readLineInto(&line))
		{
			qDebug() << line;
			QStringList list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

			fillVariable(list, "nbHarmonics:", &m_i32NbHarmonics);
			fillVariable(list, "duration:", &m_fDuration);
			fillVariable(list, "samplingRate:", &m_fSamplingRate);

			fillVector(list, "defaultHarmonicsAmplitudes:", m_i32NbHarmonics, m_vDefaultHarmonicsAmplitudes);
			fillVector(list, "defaultHarmonicsPhases:", m_i32NbHarmonics, m_vDefaultHarmonicsPhases);

			fillVector(list, "invFrequencyHarmonicsAmplitudes:", m_i32NbHarmonics, m_vInvFrequencyHarmonicsAmplitudes);
			fillVector(list, "invFrequencyHarmonicsPhases:", m_i32NbHarmonics, m_vInvFrequencyHarmonicsPhases);

			fillVector(list, "oboeHarmonicsAmplitudes:", m_i32NbHarmonics, m_vOboeHarmonicsAmplitudes);
			fillVector(list, "oboeHarmonicsPhases:", m_i32NbHarmonics, m_vOboeHarmonicsPhases);
		
		}
	
		file.close();

		m_bIsConfigFileLoaded = true;
	}

	return m_bIsConfigFileLoaded;
}

void Application::setWorkerConnections()
{
	// Selection on the interface generates new signals
	QObject::connect(m_window, SIGNAL(sigBroadcastInstrumentAndNote(int, int)), this, SLOT(setSelectedInstrumentAndNote(int, int)));
	QObject::connect(this, SIGNAL(sigBroadcastSignalFeatures(float, float, float, int, std::vector<float>, std::vector<float>)), m_pWorkerSignalGenerator, SLOT(setSignalFeatures(float, float, float, int, std::vector<float>, std::vector<float>)));
	
	// Generated signals are then transmitted to the interface for display
	QObject::connect(m_pWorkerSignalGenerator, SIGNAL(sigBroadcastHarmonicSignals(std::vector<std::vector<float>>)), m_window, SLOT(setHarmonicSignals(std::vector<std::vector<float>>)));
	QObject::connect(m_pWorkerSignalGenerator, SIGNAL(sigBroadcastFullSignals(std::vector<std::vector<float>>)), m_window, SLOT(setFullSignals(std::vector<std::vector<float>>)));
	// and to the music player
	QObject::connect(m_pWorkerSignalGenerator, SIGNAL(sigBroadcastFullSignal(std::vector<float>)), m_pWorkerMusicPlayer, SLOT(setSignalValues(std::vector<float>)));
	// and to the FFT
	QObject::connect(m_pWorkerSignalGenerator, SIGNAL(sigBroadcastFullSignals(std::vector<std::vector<float>>)), m_pWorkerFFT, SLOT(setSignalValues(std::vector<std::vector<float>>)));
	QObject::connect(m_pWorkerFFT, SIGNAL(sigBroadcastPowerSpectrumValues(std::vector<std::vector<float>>)), m_window, SLOT(setPowerSpectrum(std::vector<std::vector<float>>)));

}

void Application::moveWorkersToThread()
{
	// WorkerSignalGenerator
	if (nullptr != m_pWorkerSignalGenerator)
	{
		m_pWorkerSignalGenerator->moveToThread(&m_TWorkerSignalGenerator);
		m_TWorkerSignalGenerator.start();
	}

	// WorkerSignalGenerator
	if (nullptr != m_pWorkerMusicPlayer)
	{
		m_pWorkerMusicPlayer->moveToThread(&m_TWorkerMusicPlayer);
		m_TWorkerMusicPlayer.start();
	}

	// WorkerFFT
	if (nullptr != m_pWorkerFFT)
	{
		m_pWorkerFFT->moveToThread(&m_TWorkerFFT);
		m_TWorkerFFT.start();
	}
}

void Application::stopWorkerThreads()
{
	m_TWorkerSignalGenerator.quit();
	m_TWorkerSignalGenerator.wait();

	m_TWorkerMusicPlayer.quit();
	m_TWorkerMusicPlayer.wait();

	m_TWorkerFFT.quit();
	m_TWorkerFFT.wait();
}

void Application::deleteWorkers()
{
    auto deleteAndNullify = [](auto pointer) -> void
    {
        if (nullptr != pointer)
        {
            delete pointer;
            pointer = nullptr;
        }
    };

    deleteAndNullify(m_pWorkerSignalGenerator);
    deleteAndNullify(m_pWorkerMusicPlayer);
    deleteAndNullify(m_pWorkerFFT);

 }

void Application::setSelectedInstrumentAndNote(int instrument, int note)
{
	// sets harmonics weight values depending on the instrument
	std::vector<float> vAmplitudes;
	std::vector<float> vPhases;

	switch (instrument)
	{
	case 0:
		vAmplitudes = m_vDefaultHarmonicsAmplitudes;
		vPhases = m_vDefaultHarmonicsPhases;
		break;
	case 1:
		vAmplitudes = m_vInvFrequencyHarmonicsAmplitudes;
		vPhases = m_vInvFrequencyHarmonicsPhases;
		break;
	case 2:
		vAmplitudes = m_vOboeHarmonicsAmplitudes;
		vPhases = m_vOboeHarmonicsPhases;
		break;
	}
	
	// sets frequency value depending on the note
	float fFrequency = 0.0;
	
	switch (note)
	{
	case Notes::DO :
		fFrequency = DO_FREQ;
		break;
	case Notes::RE :
		fFrequency = RE_FREQ;
		break;
	case Notes::MI :
		fFrequency = MI_FREQ;
		break;
	case Notes::FA :
		fFrequency = FA_FREQ;
		break;
	case Notes::SOL :
		fFrequency = SOL_FREQ;
		break;
	case Notes::LA :
		fFrequency = LA_FREQ;
		break;
	case Notes::SI :
		fFrequency = SI_FREQ;
		break;
	}

	
	emit sigBroadcastSignalFeatures(m_fSamplingRate, m_fDuration, fFrequency, m_i32NbHarmonics, vAmplitudes, vPhases);
}
