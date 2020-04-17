#include "SignalDisplay.h"

SignalDisplay::SignalDisplay()
{
	// inits parameters
	m_bDrawCurves = false;
	m_bInitialization = false;
	m_i32ScrollingOffsetXTic = 0;
	m_i32ScrollingOffsetXLegend = 0;

	// inits curves colors
	QColor l_aCol[] = 
	{ 	
		Qt::red,  Qt::blue, Qt::magenta, 
		Qt::cyan, Qt::green, Qt::yellow,
		Qt::black, Qt::darkRed, Qt::darkBlue,
		Qt::darkMagenta, Qt::darkCyan, Qt::darkGreen,
		Qt::darkYellow,Qt::white,
	};

    m_i32MaxNumberofSignals = sizeof(l_aCol) / sizeof(l_aCol[0]);
	m_vColors = std::vector<QColor>(l_aCol, l_aCol + sizeof(l_aCol) / sizeof(QColor));
	
	// sets default offset settings
    std::vector<std::string> aSCurvesLabel = std::vector<std::string>();
    setSignalLabels(aSCurvesLabel);
    QSize oSize = QSize(640, 480);
    setWidgetSize(oSize);
    QSize oXRange = QSize(0, 20);
    QSize oYRange = QSize(-200, 200);
    setXYRange(oXRange, oYRange);
    setLegends("Time (s)", "Signal (V)");
    setFps(30.f);
	setTicks(5, 5);
	setXTickLabelOffsets(10, -10);
	setYTickLabelOffsets(10, 10);
	setSignalLabelOffsets(20, 20);
    setXLegendOffsets(-80, -20);
	setYLegendOffsets(10, 10);
	setDrawLine(true);
}


SignalDisplay::~SignalDisplay()
{
	
}

void SignalDisplay::checkSettings()
{
	// checks for bad settings values
    if (m_i32NumberOfSignals == 0 || m_i32NumberOfSignals > m_i32MaxNumberofSignals)
	{
        std::cerr << "[ERROR] (SignalDisplay::checkSettings) Bad input values for Curves labels! " << std::endl;
		// throw
		m_bInitialization = false;
		return;
	}

	if (m_oSize.width() == 0 || m_oSize.height() == 0)
	{
        std::cerr << "[ERROR] (SignalDisplay::checkSettings) Bad input values for widget size! " << std::endl;
		// throw
		m_bInitialization = false;
		return;
	}

	if (m_oXRange.height() - m_oXRange.width() == 0 || m_oYRange.height() - m_oYRange.width() == 0)
	{
        std::cerr << "[ERROR] (SignalDisplay::checkSettings) Bad input values for x- or y- axis range! " << std::endl;
		// throw
		m_bInitialization = false;
		return;
	}
	
	if (m_i32XTic == 0 || m_i32YTic == 0)
	{
        std::cerr << "[ERROR] (SignalDisplay::checkSettings) Bad input values for x- or y-axis tick interval! " << std::endl;
		// throw
		m_bInitialization = false;
		return;
	}

    if (m_fFps == 0.0)
	{
        std::cerr << "[ERROR] (SignalDisplay::checkSettings) Bad input values for sampling rate (FPS)! " << std::endl;
		// throw
		m_bInitialization = false;
		return;
	}
	
	m_bInitialization = true;
}

void SignalDisplay::setSignalLabels(std::vector<std::string> vSignalLabels)
{
    m_i32NumberOfSignals = static_cast<uint>(vSignalLabels.size());
	
	// inits list of curves values
	for (uint ii = 0; ii < vSignalLabels.size(); ++ii)
	{
        std::list<float> l_lCurveValue;
		m_vSignalValues.push_back(l_lCurveValue);
		m_vSignalLabels.push_back(vSignalLabels[ii].c_str());
		m_vSignalEnabled.push_back(true);
	}
}

void SignalDisplay::setWidgetSize(QSize oSize)
{
	m_oSize = oSize;
	resize(m_oSize);
}

void SignalDisplay::setXYRange(QSize oXRange, QSize oYRange)
{
	m_oXRange = oXRange;
	m_oYRange = oYRange;
	
	// computes rescaling factors y = Ax + B
	// x-axis
	m_fXRescaleFactorA = (float)(m_oSize.width()) / ((m_oXRange.height() - m_oXRange.width()) * m_fFps);
	m_fXRescaleFactorB = -m_fXRescaleFactorA * m_oXRange.width();
	// y-axis
	m_fYRescaleFactorA = (float)(-m_oSize.height()) / (m_oYRange.height() - m_oYRange.width());
	m_fYRescaleFactorB = -m_fYRescaleFactorA * m_oYRange.height();
}

void SignalDisplay::setLegends(std::string sXLegend, std::string sYLegend)
{
	m_sXLegend = sXLegend;
	m_sYLegend = sYLegend;
}

void SignalDisplay::setFps(float fFps)
{
	m_fFps = fFps;
}

void SignalDisplay::setTicks(int i32Xtic, int i32Ytic)
{
	m_i32XTic = i32Xtic;
	m_i32YTic = i32Ytic;
}

void SignalDisplay::setXTickLabelOffsets(int i32XTickLabelOffsetX, int i32XTickLabelOffsetY)
{
	m_i32XTickLabelOffsetX = i32XTickLabelOffsetX;
	m_i32XTickLabelOffsetY = i32XTickLabelOffsetY;
}

void SignalDisplay::setYTickLabelOffsets(int i32YTickLabelOffsetX, int i32YTickLabelOffsetY)
{
	m_i32YTickLabelOffsetX = i32YTickLabelOffsetX;
	m_i32YTickLabelOffsetY = i32YTickLabelOffsetY;
}

void SignalDisplay::setSignalLabelOffsets(int i32SignalLabelOffsetX, int i32SignalLabelOffsetY)
{
	m_i32SignalLabelOffsetX = i32SignalLabelOffsetX;
	m_i32SignalLabelOffsetY = i32SignalLabelOffsetY;
}

void SignalDisplay::setXLegendOffsets(int i32XLegendOffsetX, int i32XLegendOffsetY)
{
	m_i32XLegendOffsetX = i32XLegendOffsetX;
	m_i32XLegendOffsetY = i32XLegendOffsetY;
}

void SignalDisplay::setYLegendOffsets(int i32YLegendOffsetX, int i32YLegendOffsetY)
{
	m_i32YLegendOffsetX = i32YLegendOffsetX;
	m_i32YLegendOffsetY = i32YLegendOffsetY;
}

void SignalDisplay::setDrawLine(bool bDrawLines)
{
	m_bDrawLines = bDrawLines;
}

void SignalDisplay::setSignalEnabled(int i32Channel)
{
    m_vSignalEnabled[i32Channel] = !m_vSignalEnabled[i32Channel];
}

