#ifndef _WORKER_H
#define _WORKER_H

#include <QObject>
#include <QReadWriteLock>
#include <QBasicTimer>
#include <QElapsedTimer>
#include <QDebug>


class Worker: public QObject
{
	Q_OBJECT
	
public:
	Worker();
	~Worker();

public slots:
	void setConfigFilename(std::string);
	void startWork();
	void stopWork();

signals:
	void sigChangeTabWidget(int);
	void sigSendWorkerStatus(bool);

// Methods
public:
	double getFps();
	void setFps(double dFps);

	bool getVerboseMode();
	void setVerboseMode(bool bVerbose);

protected:
	void timerEvent(QTimerEvent *e);
	virtual void doWork() = 0;
	virtual bool loadConfigFile();
	virtual bool init();

private:
	

// Member variables
protected:
	bool m_bVerbose;

	std::string m_sConfigFilename;
	bool m_bIsConfigFileSet;
	
	QReadWriteLock      m_oWorkerMutex;     /**< parameters mutex */
	bool m_bWorkStarted;
	
	QBasicTimer* m_pTimer;                  /**< qt timer */
	QElapsedTimer* m_pElapsedTimer;
	double m_dFps;
	bool m_bIsFpsSet;

	bool m_bIsWorkerInitialized;

private:

};


#endif