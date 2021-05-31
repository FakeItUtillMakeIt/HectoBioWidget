#include "CompareWidgets.h"
#include <QPushButton>
#include "qwt_plot_zoomer.h"
#include "QAction"
#include "QMenu"
#include "Hdf5Read.h"

//#include "hdf5.h"
#include <QAbstractItemView>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <Qvariant.h>
#include "QFileDialog"
#include "qDebug"
#include "qwt_plot.h"
#include "QMessageBox"


CompareWidgets::CompareWidgets(QWidget *parent)
	: QWidget(parent)
{
	uicw.setupUi(this);

	for (int i = 0; i < 500; i++)
	{
		time[i] = i;
	}


	createActions();
	createContentMenu();
	//this->resize(300, 400);
	//对比按钮
	connect(uicw.compareBtn, &QPushButton::clicked, this, &CompareWidgets::on_compare_btn);
	//清除按钮
	connect(uicw.eraserAll_btn, &QPushButton::clicked, this, &CompareWidgets::on_eraser_Allplot_btn);

}

CompareWidgets::~CompareWidgets()
{
}



void CompareWidgets::createActions() {

	connect(uicw.selectFileBtn, &QPushButton::clicked, this, &CompareWidgets::on_select_file_btn);

}

void CompareWidgets::createContentMenu() {

}

void CompareWidgets::createMenu() {

}

//选择文件槽函数
void CompareWidgets::selcetFile() {
	QFileDialog* filedialog = new QFileDialog(this);
	filedialog->setWindowTitle("select file");
	filedialog->setDirectory(".");
	//设置文件过滤器
	filedialog->setNameFilter(tr("Serials File(*.fast5 *.h5 *.txt *)"));
	//可以选择多个文件
	filedialog->setFileMode(QFileDialog::ExistingFiles);
	//设置视图模式
	filedialog->setViewMode(QFileDialog::Detail);
	QStringList filenames;
	if (filedialog->exec())
	{
		filenames = filedialog->selectedFiles();
	}
	for (auto tmp : filenames)
	{
		uicw.selectedFile->append(tmp);
	}
	uicw.selectedFile->show();

	this->selectList(filenames);
}

//已选中窗口显示
void CompareWidgets::selectList(QStringList selectedFilename) {

	uicw.selectedList->setSelectionMode(QAbstractItemView::ContiguousSelection);
	int num = 0;
	for (auto tmp : selectedFilename)
	{
		uicw.selectedList->insertItem(num, tmp);
		num += 1;
	}

}

//
void CompareWidgets::on_select_file_btn() {
	selcetFile();

}



/********************************************
* 对比按钮槽函数
*****************************************/
//功能，读取选择的文件
void CompareWidgets::on_compare_btn() {

	//获取listwidget中的选中
	//设置图例
	QwtLegend* legend = new QwtLegend();
	auto selected_file = uicw.selectedList->selectedItems();
	for (auto tmp : selected_file)
	{
		//这里添加获取选中文件的内容
		QString endvalue;
		endvalue = tmp->text().split("/").back();
		QwtPlotCurve* curve = new QwtPlotCurve(endvalue);
		QString file_path = tmp->text();
		uicw.selectedFile->append(file_path);

		//以fast5文件标识符结尾
		if (file_path.endsWith("fast5"))
		{
			readDataFromONT(file_path.toStdString().c_str(), curve);
		}
		else if (file_path.endsWith("h5"))
		{
			readDataAsHdf5(file_path.toStdString().c_str(), curve);
		}
		else if (file_path.endsWith("txt"))
		{
			readDataAsStream(file_path, curve);
		}
		else
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("error！！！"), QString::fromLocal8Bit("错误文件类型"), QMessageBox::Yes);
			return;
		}

		//readDataAsStream(file_path,curve);
		//readDataAsHdf5(file_path.toStdString().c_str(), curve);
		//readDataFromONT(file_path.toStdString().c_str(),curve);

		int ranR, ranG, ranB;
		ranR = rand() % 255;
		ranG = rand() % 255;
		ranB = rand() % 255;

		QColor color = QColor(ranR, ranG, ranB);
		curve->setPen(color, 2);
		curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
		curve->setLegendAttribute(curve->LegendShowLine);
		legend->setDefaultItemMode(QwtLegendData::Checkable);
		//connect(legend, SIGNAL(Checked(const QVariant &,bool,int)),SLOT(showI));
		uicw.signalCompare->insertLegend(legend);
		curve->attach(uicw.signalCompare);
		//curve->setLegendAttribute(QwtPlotCurve::);
	}



	//uicw.signalCompare->replot();
	uicw.signalCompare->setTitle(QwtText("SIGNAL PLOT"));
	uicw.signalCompare->setAxisTitle(QwtPlot::yLeft, "current / mA");
	uicw.signalCompare->setAxisTitle(QwtPlot::xBottom, "time / mS");



	QwtPlotZoomer* zoomer = new QwtPlotZoomer(uicw.signalCompare->canvas());
	//选框颜色设置为红色
	zoomer->setRubberBandPen(QColor(Qt::red));

	zoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
	zoomer->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton);
	zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);

	zoomer->setEnabled(true);

}

void CompareWidgets::on_eraser_Allplot_btn() {
	//uicw.signalCompare->close();
	uicw.signalCompare->detachItems();
	uicw.signalCompare->replot();
}




void CompareWidgets::readDataAsText(QString& read_file, double* time, double* showdata) {
	QFile readfile(read_file);
	readfile.open(QIODevice::ReadOnly);
	QTextStream txtout(&readfile);
	QString linestr;

	for (size_t i = 0; i < 400; i++)
	{
		time[i] = i;
		//linestr = txtout.readLine();
		txtout >> linestr;
		showdata[i] = linestr.toDouble();
	}

}
void CompareWidgets::readDataAsStream(QString& file_path, QwtPlotCurve* curve) {
	FILE* readfile = fopen(file_path.toStdString().c_str(), "r");

	if (readfile == NULL)
	{
		return;
	}
	int count = 0;

	while (!(feof(readfile)) && count < 499)
	{
		double buf[1];
		fread(buf, sizeof(double), 1, readfile);
		/**(showdata + count) = buf[0];
		*(time + count) = count;*/
		val[count] = buf[0];
		time[count] = count;
		count++;
	}

	fclose(readfile);

	curve->setSamples(time, val, 499);

}

void CompareWidgets::readDataAsHdf5(const char* read_file, QwtPlotCurve* curve) {
	Hdf5Read* hdf5read = new Hdf5Read();

	int row;
	int col;
	row = 1;
	col = 500;

	for (int i = 0; i < 500; i++)
	{
		time[i] = i;
	}

	const char* db_name = "Dataset";
	hdf5read->read_file(val, row, col, read_file, db_name);

	qDebug() << val[0] << val[499];

	curve->setSamples(time, val, 499);
}

//可读ONT

void CompareWidgets::readDataFromONT(const char* read_file, QwtPlotCurve* curve) {
	Hdf5Read* hdf5read = new Hdf5Read();

	int  dim;
	

	const char* db_name = "Signal";
	double* data=hdf5read->read_ont_file(dim, read_file, db_name);

	double* x=(double*)malloc(sizeof(double)*dim);
	for (int i = 0; i < dim; i++)
	{
		*(x+i) = (double)i;
	}

	curve->setSamples(x, data, dim);
}

double* CompareWidgets::readDataFromONT(const char* read_file) {
	Hdf5Read* hdf5read = new Hdf5Read();

	int  dim;

	const char* db_name = "Signal";
	double* data = hdf5read->read_ont_file(dim, read_file, db_name);


	double* x = (double*)malloc(sizeof(double) * dim);
	for (int i = 0; i < dim; i++)
	{
		*(x + i) = (double)i;
	}

	//retdata = data;
	return data;
}
