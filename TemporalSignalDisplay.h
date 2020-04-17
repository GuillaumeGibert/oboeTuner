#ifndef TEMPORAL_SIGNAL_DISPLAY_
#define TEMPORAL_SIGNAL_DISPLAY_

#include "SignalDisplay.h"

class TemporalSignalDisplay : public SignalDisplay
{
	Q_OBJECT
	
	public:
		TemporalSignalDisplay();
		~TemporalSignalDisplay();
    public slots:
        /**
		* \brief Sets a new input set of values to draw, the array must have the same size than the signal label curve array.
		* \param [in] aFCurvesValues : array of input values, corresponding to the title curve array
		*/
        void setNewValues(std::vector<float> vSignalValues);
		
	private:
		/**
		* \brief paint event
		*/		    
		void paintEvent(QPaintEvent *);
	
};

#endif
