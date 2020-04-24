#include "workers/Worker.h"

// CONSTRUCTOR
Worker::Worker(): 
	m_bVerbose(false),
	m_bIsWorkerInitialized(false),
	m_sConfigFilename(""), m_bIsConfigFileSet(false),
	m_bWorkStarted(false),
	m_pTimer(new QBasicTimer), m_dFps(0.0), m_bIsFpsSet(false),
	m_pElapsedTimer(new QElapsedTimer)
{

}

// DESTRUCTOR
Worker::~Worker()
{
	if (nullptr != m_pTimer)
	{
		delete m_pTimer;
		m_pTimer = nullptr;
	}
		

	if (nullptr != m_pElapsedTimer)
	{
		delete m_pElapsedTimer;
		m_pElapsedTimer = nullptr;
	}
		
}

bool Worker::init()
{
	return true;
}

bool Worker::loadConfigFile()
{
	return true;
}


void Worker::timerEvent(QTimerEvent *e)
{
	Q_UNUSED(e);

	doWork();
}

void Worker::startWork()
{
	if (!m_bIsFpsSet || m_dFps == 0.0)
	{
		qDebug() << "[ERROR] (Worker->startWork): Frame rate is not set or set to 0!";
		return;
	}

	if (!m_bWorkStarted)
	{
		m_bWorkStarted = true;
		m_pTimer->start(1000.0 / m_dFps, this);
		m_pElapsedTimer->start();
		emit sigSendWorkerStatus(m_bWorkStarted);
	}
	else
	{
		if(m_bVerbose)
			qDebug() << "[INFO] (Worker->startWork): Thread already started!";
	}
	
}

void Worker::stopWork()
{
	if (m_bWorkStarted)
	{
		m_bWorkStarted = false;
		m_pTimer->stop();
		emit sigSendWorkerStatus(m_bWorkStarted);
	}
	else
	{
		if(m_bVerbose)
			qDebug() << "[INFO] (Worker->startWork): Thread already stopped!";
	}
	
}

// SETTERS and GETTERS
void Worker::setConfigFilename(std::string sConfigFilename)
{
	m_oWorkerMutex.lockForWrite();
		m_sConfigFilename = sConfigFilename;
	m_oWorkerMutex.unlock();

	m_bIsConfigFileSet = loadConfigFile();


	if (!m_bIsConfigFileSet)
	{
		qDebug() << "[ERROR] (Worker::setConfigFilename) Could not load config file!";
	}
	else
	{
		if (m_bVerbose)
			qDebug() << "[INFO] (Worker::setConfigFilename) config file loaded successfully!";
	}
}

double Worker::getFps()
{
	double dFps;

	m_oWorkerMutex.lockForRead();
		dFps = m_dFps;
	m_oWorkerMutex.unlock();

	return dFps;
}

void Worker::setFps(double dFps)
{
	m_oWorkerMutex.lockForWrite();
		m_dFps = dFps;
		m_bIsFpsSet = true;
	m_oWorkerMutex.unlock();
	
	if (m_bVerbose)
	{
		qDebug() << "[INFO] (Worker->setFps): Fps set to: " + (int)dFps;
	}
}

bool Worker::getVerboseMode()
{
	bool bVerbose;
	
	m_oWorkerMutex.lockForRead();
		bVerbose = m_bVerbose;
	m_oWorkerMutex.unlock();

	return bVerbose;
}


void Worker::setVerboseMode(bool bVerbose)
{
	m_oWorkerMutex.lockForWrite();
		m_bVerbose = bVerbose;
	m_oWorkerMutex.unlock();
}