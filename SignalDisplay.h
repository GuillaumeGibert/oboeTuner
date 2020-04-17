#ifndef SIGNAL_DISPLAY_
#define SIGNAL_DISPLAY_

#include <QWidget>
#include <QPainter>
#include <iostream>
#include <sstream>
#include <deque>

class SignalDisplay : public QWidget
{
	Q_OBJECT
	
	public:
		SignalDisplay();
		~SignalDisplay();
	
	public:
        void setSignalLabels(std::vector<std::string>);
        void setWidgetSize(QSize);
        void setXYRange(QSize, QSize);
		void setLegends(std::string, std::string);
		void setFps(float);
		void setTicks(int, int);
		void setXTickLabelOffsets(int, int);
		void setYTickLabelOffsets(int, int);
		void setSignalLabelOffsets(int, int);
		void setXLegendOffsets(int, int);
		void setYLegendOffsets(int, int);
		void setDrawLine(bool);
	
    public slots:
        void setSignalEnabled(int);

    protected:
        void checkSettings();
	
	
    protected:
		bool m_bVerbose; 
		bool m_bDrawCurves;				/**< draw the curves ? */
		bool m_bDrawLines;					/**< draw lines ? or points */
		bool m_bInitialization;				/**< is intialization ok? */
	
		int m_i32NumberOfSignals;			/**< number of curves */
		int m_i32MaxNumberofSignals;			/**< max number of curves */
		int m_i32XTic;						/**< x tic of the curves  (like xtics for gnuplot) */
		int m_i32YTic;						/**< y tic of the curves  (like ytics for gnuplot)  */
		int m_i32ScrollingOffsetXTic;			/**< x tic scrolling offset */
		int m_i32ScrollingOffsetXLegend;		/**< x legend scrolling offset */
		
		int m_i32XTickLabelOffsetX;			/**< x tick label along x-axis offset */
		int m_i32XTickLabelOffsetY;			/**< x tick label along y-axis offset */
		
		int m_i32YTickLabelOffsetX;			/**< y tick label along x-axis offset */
		int m_i32YTickLabelOffsetY;			/**< y tick label along y-axis offset */
		
        int m_i32SignalLabelOffsetX;			/**< curve label along x-axis offset */
        int m_i32SignalLabelOffsetY;			/**< curve label along y-axis offset */

		int m_i32XLegendOffsetX;				/**< x-axis legend along x-axis offset */
		int m_i32XLegendOffsetY;				/**< x-axis legend along y-axis offset */
		
		int m_i32YLegendOffsetX;				/**< y-axis legend along x-axis offset */
		int m_i32YLegendOffsetY;				/**< y-axis legend along y-axis offset */
	
		float  m_fXRescaleFactorA;          		/**< rescaling factor A for x-axis values to pixel conversion y = Ax + B */
		float  m_fXRescaleFactorB;				/**< rescaling factor B for x-axis values to pixel conversion y = Ax + B */

		float  m_fYRescaleFactorA;				/**< rescaling factor A for y-axis values to pixel conversion y = Ax + B */
		float  m_fYRescaleFactorB;				/**< rescaling factor B for y-axis values to pixel conversion y = Ax + B */

		float m_fFps;						/**< signal sampling rate (FPS) in Hz */

		std::string m_sXLegend;				/**< legend of the x-axis */
		std::string m_sYLegend;				/**< legend of the y-axis */
		
		QSize m_oSize;						/**< size of the widget */

		QSize m_oXRange;					/**< range of values along the x-axis i.e. time duration */
		QSize m_oYRange;					/**< range of values along the y-axis */

		QVector<QString> m_vSignalLabels;                   	 /**< array of the curves titles */
		std::vector<std::list<float> > m_vSignalValues; 	/**< array of the list of the current values of the curves to display */
		std::vector<QColor> m_vColors;                      		/**< array of the curves colors */
		std::vector<bool> m_vSignalEnabled;
		
};

#endif
