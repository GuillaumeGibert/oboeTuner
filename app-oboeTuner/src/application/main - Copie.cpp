/**
* \file main.cpp
* \brief Defines the main
* \author Guillaume Gibert
* \date 27/01/2020
*/

#include "application/Application.h"
#include <QApplication>

#include <QtWidgets/QApplication>
 

int main(int argc, char *argv[]) 
{ 
	QApplication a(argc, argv);
	
	Application app;
	MainWindow w;

	w.show();
	app.init(&w);

	return a.exec();
	
}
