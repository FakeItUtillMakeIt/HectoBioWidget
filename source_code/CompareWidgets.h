#pragma once

#include <QWidget>
#include "ui_CompareWidgets.h"
#include "qwt_plot_curve.h"
#include <QMenu>
#include <QAction>

class CompareWidgets : public QWidget
{
	Q_OBJECT

public:
	CompareWidgets(QWidget *parent = Q_NULLPTR);
	~CompareWidgets();

	double time[500];
	double val[1000] = { 0 };

	void readDataFromONT(const char* read_file, QwtPlotCurve* curve);
	double* readDataFromONT(const char* read_file);
private:

	void createActions();
	void createMenu();
	void createContentMenu();

	void selcetFile();

	void selectList(QStringList selectedFilename);
	//¶ÁÈ¡ÎÄ¼þ
	void readDataAsText(QString& read_file, double* time, double* showdata);
	void readDataAsStream(QString& read_file, QwtPlotCurve* curve);
	void readDataAsHdf5(const char* read_file, QwtPlotCurve* curve);
	

private:
	QwtPlotCurve* curve1 = new QwtPlotCurve("signal2");
	//int timer = 0;
	//int time_dur = 50;
	Ui::CompareWidgets uicw;
	//void timerEvent(QTimerEvent*);

	QAction* fileopenAction;
	QMenu* menu;

private slots:
	void on_compare_btn();
	void on_select_file_btn();
	void on_eraser_Allplot_btn();
};
