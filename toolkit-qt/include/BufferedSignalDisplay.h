#ifndef BUFFERED_SIGNAL_DISPLAY_
#define BUFFERED_SIGNAL_DISPLAY_

#include "SignalDisplay.h"

class BufferedSignalDisplay : public SignalDisplay
{
    Q_OBJECT

    public:
        BufferedSignalDisplay();
        ~BufferedSignalDisplay();
public slots:
        /**
        * \brief Sets a new input set of values to draw, the array must have the same size than the signal label curve array.
        * \param [in] aFCurvesValues : array of input values, corresponding to the title curve array
        */
        void setNewValues(std::vector<std::deque<float>> vSignalValues);

    private:
        /**
        * \brief paint event
        */
        void paintEvent(QPaintEvent *);

    private:
        std::vector<std::deque<float> > m_vSignalValues; 	/**< array of the list of the current values of the curves to display */

};

#endif
