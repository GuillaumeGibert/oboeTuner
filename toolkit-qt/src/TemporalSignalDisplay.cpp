#include "TemporalSignalDisplay.h"

TemporalSignalDisplay::TemporalSignalDisplay()
{}

TemporalSignalDisplay::~TemporalSignalDisplay()
{}

void TemporalSignalDisplay::setNewValues(std::vector<float> aFCurvesValues)
{
	if (!m_bInitialization)
		checkSettings();
	else
	{
        if (aFCurvesValues.size() != m_i32NumberOfSignals)
		{
			if (m_bVerbose)
                std::cerr << "[ERROR] (TemporalSignalDisplay::setNewValues) the size of the vector doesn't match the predefined number of curves! " << std::endl;
			m_bDrawCurves = false;
			return;
		}
		else
		{
			bool l_bIsSroll = false;
			m_bDrawCurves = true;

            for (int ii = 0; ii < m_i32NumberOfSignals; ++ii)
			{
                if (m_vSignalValues[ii].size() == (m_oXRange.height() - m_oXRange.width()) * m_fFps)
				{
                    m_vSignalValues[ii].pop_front();
					l_bIsSroll = true;
				}

                m_vSignalValues[ii].push_back(aFCurvesValues[ii]);
			}

			// apply scrolling offset
			if (l_bIsSroll)
			{
				++m_i32ScrollingOffsetXTic;

				if (m_i32ScrollingOffsetXTic % (int)(m_i32XTic * m_fFps) == 0)
				{
					++m_i32ScrollingOffsetXLegend;
				}

				m_i32ScrollingOffsetXTic = m_i32ScrollingOffsetXTic % (int)(m_i32XTic * m_fFps);
			}
		}

		update();
	}
}

void TemporalSignalDisplay::paintEvent(QPaintEvent *)
{
	QPainter l_oPainter(this);

	if (m_bDrawCurves)
	{
		// draw background
		l_oPainter.fillRect(0, 0, m_oSize.width(), m_oSize.height(), Qt::lightGray);

		// draw tics
		l_oPainter.setPen(Qt::white);

		// x-ticks
        for (int l_xTick = 0; l_xTick < (m_oXRange.height() - m_oXRange.width()) / m_i32XTic + 1; ++l_xTick)
		{
            if (l_xTick * (m_fFps * m_i32XTic) - m_i32ScrollingOffsetXTic > 0 && l_xTick * (m_fFps * m_i32XTic) - m_i32ScrollingOffsetXTic < m_oSize.width()*m_fFps)
				l_oPainter.drawLine(	QPoint((l_xTick * (m_fFps * m_i32XTic) - m_i32ScrollingOffsetXTic) * m_fXRescaleFactorA + m_fXRescaleFactorB, 0),
										QPoint((l_xTick * (m_fFps * m_i32XTic) - m_i32ScrollingOffsetXTic) * m_fXRescaleFactorA + m_fXRescaleFactorB, m_oSize.height()));
		}

		// y-ticks > 0
        for (int l_yTick = 0; l_yTick < m_oYRange.height() / m_i32YTic; ++l_yTick)
		{
			l_oPainter.drawLine(	QPoint(0, (l_yTick * m_i32YTic) * m_fYRescaleFactorA + m_fYRescaleFactorB),
									QPoint(m_oSize.width(), (l_yTick * m_i32YTic) * m_fYRescaleFactorA + m_fYRescaleFactorB));
		}

		// y-ticks < 0
		for (int l_yTick = 0; l_yTick > m_oYRange.width() / m_i32YTic; --l_yTick)
		{
			l_oPainter.drawLine(	QPoint(0, (l_yTick * m_i32YTic) * m_fYRescaleFactorA + m_fYRescaleFactorB),
									QPoint(m_oSize.width(), (l_yTick * m_i32YTic) * m_fYRescaleFactorA + m_fYRescaleFactorB));
		}
		
		// draw legends
		// x-axis
        for (int l_xTick = 0; l_xTick < (m_oXRange.height() - m_oXRange.width()) / m_i32XTic + 1; ++l_xTick)
		{
            if (l_xTick * (m_fFps * m_i32XTic) - m_i32ScrollingOffsetXTic > 0 && l_xTick * (m_fFps * m_i32XTic) - m_i32ScrollingOffsetXTic < m_oSize.width()*m_fFps)
			{
                std::ostringstream l_osXLegend;
				l_osXLegend << (l_xTick + m_i32ScrollingOffsetXLegend) * m_i32XTic;
				l_oPainter.drawText(QPoint((l_xTick * (m_fFps * m_i32XTic) - m_i32ScrollingOffsetXTic) * m_fXRescaleFactorA + m_fXRescaleFactorB + m_i32XTickLabelOffsetX,
					m_oSize.height() + m_i32XTickLabelOffsetY),
					l_osXLegend.str().c_str()
				);
			}
		}

		// y-axis > 0
        for (int l_yTick = 0; l_yTick < m_oYRange.height() / m_i32YTic; ++l_yTick)
		{
            std::ostringstream l_osYLegend;
			l_osYLegend << l_yTick * m_i32YTic;
			l_oPainter.drawText(	QPoint(0 + m_i32YTickLabelOffsetX,
									(l_yTick * m_i32YTic) * m_fYRescaleFactorA + m_fYRescaleFactorB + m_i32YTickLabelOffsetY),
									l_osYLegend.str().c_str()
								);
		}

		// y-axis < 0
		for (int l_yTick = 0; l_yTick > m_oYRange.width() / m_i32YTic; --l_yTick)
		{
            std::ostringstream l_osYLegend;
			l_osYLegend << l_yTick * m_i32YTic;
			l_oPainter.drawText(QPoint(0 + m_i32YTickLabelOffsetX,
				(l_yTick * m_i32YTic) * m_fYRescaleFactorA + m_fYRescaleFactorB + m_i32YTickLabelOffsetY),
				l_osYLegend.str().c_str()
			);
		}
		
		// curve names
        uint l_i32LegendTextWidth = (m_oSize.width() / 2) / static_cast<uint>(m_vSignalValues.size());

        for (uint l_curveNumber = 0; l_curveNumber < m_vSignalValues.size(); l_curveNumber++)
		{
            if (m_vSignalEnabled[l_curveNumber]) {
                //if(m_bVerbose)
                //	std::cout << "curve " <<  l_curveNumber << "is active (curve names)" << std::endl;

                l_oPainter.setPen(m_vColors[l_curveNumber%m_vColors.size()]);

                l_oPainter.drawText(	QPoint((m_oSize.width() / 2) + (l_curveNumber * l_i32LegendTextWidth) + m_i32SignalLabelOffsetX,
                                                0 + m_i32SignalLabelOffsetY),
                                                m_vSignalLabels[l_curveNumber]
									);
			}
		}

		// x- and y-axis legend
		l_oPainter.setPen(Qt::white);
		l_oPainter.drawText(QPoint(m_oSize.width() + m_i32XLegendOffsetX,
			m_oSize.height() + m_i32XLegendOffsetY),
			QString::fromStdString(m_sXLegend)
		);

		l_oPainter.drawText(QPoint(m_i32YLegendOffsetX,
			m_i32YLegendOffsetY),
			QString::fromStdString(m_sYLegend)
		);


		// draw values
		std::vector<std::vector<QPoint> > l_aAP3fPointsToDraw;
        for (uint ii = 0; ii < m_vSignalValues.size(); ++ii)
		{

			std::vector<QPoint> l_aP3fCurvePoints;

			uint jj = 0;
            for (std::list<float>::iterator it = m_vSignalValues[ii].begin(); it != m_vSignalValues[ii].end(); ++it, ++jj)
			{
				float l_fVal = (*it);

				// checks extreme values
				if (l_fVal > m_oYRange.height())
				{
					l_fVal = m_oYRange.height();
				}
				else if (l_fVal < m_oYRange.width())
				{
					l_fVal = m_oYRange.width();
				}

				int l_fValToPlot = l_fVal * m_fYRescaleFactorA + m_fYRescaleFactorB;

				l_aP3fCurvePoints.push_back(QPoint(jj*m_fXRescaleFactorA + m_fXRescaleFactorB, (int)l_fValToPlot));
			}

			l_aAP3fPointsToDraw.push_back(l_aP3fCurvePoints); 
		}

		for (uint ii = 0; ii < l_aAP3fPointsToDraw.size(); ++ii)
		{
            if (m_vSignalEnabled[ii])
            {
                l_oPainter.setPen(m_vColors[ii%m_vColors.size()]);

				if (m_bDrawLines)
				{
					if (l_aAP3fPointsToDraw[ii].size() > 1) // 2 points are necessary to draw a line!
					{
						for (uint jj = 0; jj < l_aAP3fPointsToDraw[ii].size() - 1; ++jj)
						{
							l_oPainter.drawLine(l_aAP3fPointsToDraw[ii].at(jj), l_aAP3fPointsToDraw[ii].at(jj + 1));
						}
					}
				}
				else
				{
					for (uint jj = 0; jj < l_aAP3fPointsToDraw[ii].size(); ++jj)
					{
						l_oPainter.drawPoint(l_aAP3fPointsToDraw[ii].at(jj));
					}
				}
			}
		}
	}
}
