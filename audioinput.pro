TEMPLATE = app
TARGET = audioinput

QT += multimedia widgets

HEADERS       = audioinput.h\
			BufferedSignalDisplay.h\
			Buffering.h\
			FFT.h\
			SignalDisplay.h\
			TemporalSignalDisplay.h

SOURCES       = audioinput.cpp \
			BufferedSignalDisplay.cpp\
			Buffering.cpp\
			FFT.cpp\
			SignalDisplay.cpp\
			TemporalSignalDisplay.cpp\
                main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/audioinput
INSTALLS += target
include(../shared/shared.pri)