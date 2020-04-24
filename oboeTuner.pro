TEMPLATE = app
TARGET = oboeTuner

QT += multimedia widgets
requires(qtConfig(listwidget))
android: QT += androidextras

INCLUDEPATH =   	toolkit-qt/include \
				toolkit-signal/include\
                                app-oboeTuner/include/application

HEADERS       = 	toolkit-qt/include/SignalDisplay.h \
				toolkit-qt/include/BufferedSignalDisplay.h \
				toolkit-qt/include/TemporalSignalDisplay.h \
				toolkit-signal/include/FFT.h \
				toolkit-signal/include/Buffering.h \
				toolkit-signal/include/SineGenerator.h \
				app-oboeTuner/include/application/audioinput.h
		

SOURCES       = 	toolkit-qt/src/SignalDisplay.cpp \
				toolkit-qt/src/BufferedSignalDisplay.cpp \
				toolkit-qt/src/TemporalSignalDisplay.cpp \
				toolkit-signal/src/FFT.cpp \
				toolkit-signal/src/Buffering.cpp \
				toolkit-signal/src/SineGenerator.cpp \
                                app-oboeTuner/src/application/audioinput.cpp \
                                app-oboeTuner/src/application/main.cpp
				
INSTALLS += target

include(shared.pri)