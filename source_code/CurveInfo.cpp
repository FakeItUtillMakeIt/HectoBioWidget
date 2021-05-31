#include "CurveInfo.h"
#include "time.h"
#include <qwt_color_map.h>
#include <QColor>



void CurveInfo::addCurve(QString id) {
	//QColor currentColor = getNextColor();
	QColor color ;
	//QColor currentColor = color.darker();
	//创建一条新曲线并设置
	QwtPlotCurve* curve = new QwtPlotCurve(id);
	//添加到list
	_curves.insert(id, curve);

	curve->setStyle(QwtPlotCurve::Lines);
	curve->setPaintAttribute(QwtPlotCurve::FilterPoints, true);
	//setCurveColor(id, currentColor);
	curve->setLegendAttribute(curve->LegendShowLine);
	curve->attach(this);

	//创建数据集
	TimeSerialData* dataset = new TimeSerialData();
	//添加进list
	_data.insert(id, dataset);
}

void CurveInfo::setVisiableByID(QString id, bool visiable) {
	if (_curves.contains(id))
	{
		_curves.value(id)->setVisible(visiable);
		if (visiable)
		{
			_curves.value(id)->attach(this);
		}
		else
		{
			_curves.value(id)->detach();
		}
	}
}

void CurveInfo::hideCurve(QString id) {
	this->setVisiableByID(id, false);
}

void CurveInfo::showCurve(QString id) {
	this->setVisiableByID(id, true);

}

void CurveInfo::appendData(QString dataname, quint64 ms, double value) {
	/*datalock.lock();
	if (!data.contains(dataname))
		addCurve(dataname);
	TimeSerialData* dataset = _data.value(dataname);

	dataset->append(time, value);

	QwtPlotCurve* curve = _curves.value(dataname);
	curve->setRawSamples(dataset->getPlotX(), dataset->getPlotY(), dataset->getPlotCount());

	setAxisScale(QwtPlot::xBottom, dataset->getmsstart(), dataset->getmsstop(), 10.0);
	setAxisScale(QwtPlot::yLeft, -30, 30, 5);
	datalock.unlock();*/
	
}

