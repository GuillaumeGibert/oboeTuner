/**
* \file MainWindow.h
* \brief Defines Application
* \author Guillaume Gibert
* \date 27/01/2020
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QSignalMapper>

#include "BufferedSignalDisplay.h"

// Qt stuff
namespace Ui {
class MainWindow;
}


/**
* \class MainWindow
* \brief Represents the main window of the application.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	/**
	* \brief The constructor.
	* \param [in] parent: the widget parent
	*/
	explicit MainWindow(QWidget *parent = 0);

	
	/**
	* \brief The function to delete the object.
	*/
	~MainWindow();

	

public slots:
	void setNoteIndex(int note);
	void setInstrumentIndex(int i32InstrumentIndex);
	void setHarmonicSignals(std::vector<std::vector<float>>);
	void setFullSignals(std::vector<std::vector<float>>);
	void setPowerSpectrum(std::vector<std::vector<float>>);

signals:
	void sigBroadcastInstrumentAndNote(int, int);
	void play();
	
private:
	void initWidgets();
	void setInterfaceConnections();
	
private:
	Ui::MainWindow *ui;									// Also Qt stuff
	
	BufferedSignalDisplay* m_pWSignalDisplay;
	BufferedSignalDisplay* m_pWPowerSpectrumDisplay;

	int m_i32InstrumentIndex;
	int m_i32NoteIndex;

	QSignalMapper* m_pNoteMapper;
	QSignalMapper* m_pInstrumentMapper;
};

#endif // MAINWINDOW_H
