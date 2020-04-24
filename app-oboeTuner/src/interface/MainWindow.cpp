/**
* \file MainWindow.cpp
* \brief Defines the main Qt window
* \author Guillaume Gibert
* \date 27/01/2020
*/

#include "interface/MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	m_pWSignalDisplay(nullptr), m_pWPowerSpectrumDisplay(nullptr),
	m_i32InstrumentIndex(0), m_i32NoteIndex(0),
	m_pNoteMapper(new QSignalMapper(this)), m_pInstrumentMapper(new QSignalMapper(this))
{
	ui->setupUi(this);
	//this->setWindowTitle(QString("Music Note Synthesizer"));
	
	initWidgets();
	setInterfaceConnections();
	
}

MainWindow::~MainWindow()
{
	auto deleteAndNullify = [](auto pointer) -> void
	{
		if (nullptr != pointer)
		{
			delete pointer;
			pointer = nullptr;
		}
	};

	deleteAndNullify(m_pWSignalDisplay);
	deleteAndNullify(m_pWPowerSpectrumDisplay);
	   
	deleteAndNullify(m_pNoteMapper);
	deleteAndNullify(m_pInstrumentMapper);
}

void MainWindow::initWidgets()
{
	// Signal display
	if (nullptr == m_pWSignalDisplay)
	{
		m_pWSignalDisplay = new BufferedSignalDisplay();
		m_pWSignalDisplay->setMinimumSize(600, 600);
		m_pWSignalDisplay->setWidgetSize(QSize(640, 480));
		std::vector<std::string> vSignalLabels;
		vSignalLabels.push_back("Signal");
		m_pWSignalDisplay->setSignalLabels(vSignalLabels);
		m_pWSignalDisplay->setFps(8000.0);
		m_pWSignalDisplay->setXYRange(QSize(0, 1), QSize(-1, 1));
		m_pWSignalDisplay->setLegends("Time (s)", "Signal amplitude");
		m_pWSignalDisplay->setTicks(1, 1);
		m_pWSignalDisplay->setDrawLine(true);

		ui->hlSignalDisplays->addWidget(m_pWSignalDisplay);
	}

	// Power Spectrum display
	if (nullptr == m_pWPowerSpectrumDisplay)
	{
		m_pWPowerSpectrumDisplay = new BufferedSignalDisplay();
		m_pWPowerSpectrumDisplay->setMinimumSize(600, 600);
		m_pWPowerSpectrumDisplay->setWidgetSize(QSize(640, 480));
		std::vector<std::string> vSignalLabels;
		vSignalLabels.push_back("Signal");
		m_pWPowerSpectrumDisplay->setSignalLabels(vSignalLabels);
		m_pWPowerSpectrumDisplay->setFps(8000.0);
		m_pWPowerSpectrumDisplay->setXYRange(QSize(0, 4000), QSize(-10, 100));
		m_pWPowerSpectrumDisplay->setLegends("Frequency (Hz)", "Power spectrum");
		m_pWPowerSpectrumDisplay->setTicks(5, 500);
		m_pWPowerSpectrumDisplay->setDrawLine(true);

		ui->hlSignalDisplays->addWidget(m_pWPowerSpectrumDisplay);
	}
	
}

void MainWindow::setInterfaceConnections()
{
	// Notes	
		// in
	QObject::connect(ui->pbNotesDo, SIGNAL(clicked()), m_pNoteMapper, SLOT(map()));
	QObject::connect(ui->pbNotesRe, SIGNAL(clicked()), m_pNoteMapper, SLOT(map()));
	QObject::connect(ui->pbNotesMi, SIGNAL(clicked()), m_pNoteMapper, SLOT(map()));
	QObject::connect(ui->pbNotesFa, SIGNAL(clicked()), m_pNoteMapper, SLOT(map()));
	QObject::connect(ui->pbNotesSol, SIGNAL(clicked()), m_pNoteMapper, SLOT(map()));
	QObject::connect(ui->pbNotesLa, SIGNAL(clicked()), m_pNoteMapper, SLOT(map()));
	QObject::connect(ui->pbNotesSi, SIGNAL(clicked()), m_pNoteMapper, SLOT(map()));
		// maping
	m_pNoteMapper->setMapping(ui->pbNotesDo, 0);
	m_pNoteMapper->setMapping(ui->pbNotesRe, 1);
	m_pNoteMapper->setMapping(ui->pbNotesMi, 2);
	m_pNoteMapper->setMapping(ui->pbNotesFa, 3);
	m_pNoteMapper->setMapping(ui->pbNotesSol, 4);
	m_pNoteMapper->setMapping(ui->pbNotesLa, 5);
	m_pNoteMapper->setMapping(ui->pbNotesSi, 6);
		// out
	QObject::connect(m_pNoteMapper, SIGNAL(mapped(int)), this, SLOT(setNoteIndex(int)));


	// Instruments	
		// in
	QObject::connect(ui->pbInstrumentFundamental, SIGNAL(clicked()), m_pInstrumentMapper, SLOT(map()));
	QObject::connect(ui->pbInstrumentInvFreq, SIGNAL(clicked()), m_pInstrumentMapper, SLOT(map()));
	QObject::connect(ui->pbInstrumentOboe, SIGNAL(clicked()), m_pInstrumentMapper, SLOT(map()));
		// maping
	m_pInstrumentMapper->setMapping(ui->pbInstrumentFundamental, 0);
	m_pInstrumentMapper->setMapping(ui->pbInstrumentInvFreq, 1);
	m_pInstrumentMapper->setMapping(ui->pbInstrumentOboe, 2);
		// out
	QObject::connect(m_pInstrumentMapper, SIGNAL(mapped(int)), this, SLOT(setInstrumentIndex(int)));

	
}

void MainWindow::setNoteIndex(int i32NoteIndex)
{
	m_i32NoteIndex = i32NoteIndex;

	emit sigBroadcastInstrumentAndNote(m_i32InstrumentIndex, m_i32NoteIndex);
}

void MainWindow::setInstrumentIndex(int i32InstrumentIndex)
{
	m_i32InstrumentIndex = i32InstrumentIndex;

	emit sigBroadcastInstrumentAndNote(m_i32InstrumentIndex, m_i32NoteIndex);
}

void MainWindow::setHarmonicSignals(std::vector<std::vector<float>> vHarmonicSignals)
{
	//m_pWHarmonicSignalDisplay->setNewValues(vHarmonicSignals);
}

void MainWindow::setFullSignals(std::vector<std::vector<float>> vFullSignals)
{
	m_pWSignalDisplay->setNewValues(vFullSignals);
}

void MainWindow::setPowerSpectrum(std::vector<std::vector<float>> vPowerSpectrum)
{
	m_pWPowerSpectrumDisplay->setNewValues(vPowerSpectrum);
}
