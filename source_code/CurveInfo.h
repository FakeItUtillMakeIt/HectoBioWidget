#pragma once
#include <qwt_plot.h>

#include <QWT/qwt.h>
#include <qwt_plot_curve.h>
#include <time.h>
#include "TimeSerialData.h"

class CurveInfo :
    public QwtPlot
{
public:
	CurveInfo();
	void addCurve(QString id);
	void appendData(QString dataname, quint64 ms, double value);
	void setVisiableByID(QString id, bool visiable);
	void hideCurve(QString id);
	void showCurve(QString id);
	void setCurveColor(QString id, QColor color);
	


	QMap <QString, TimeSerialData*> _data;
	QMap <QString, QwtPlotCurve*> _curves;
};

