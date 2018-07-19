#include "showchart.h"
#include "ui_showchart.h"
#include "bydGaitAnalysis.h"
#include <QLineSeries>
#include <QDebug>
#include <QChartView>
#include <QChart>
#include <QPen>
#include <QPainter>
#include <healthreaddata.h>
#include "Windows.h"
#include "tchar.h"
ShowChart::ShowChart(QWidget *parent,QString id ,int type,HealthReadData *readData) :
	QWidget(parent),
	ui(new Ui::ShowChart)
{
	ui->setupUi(this);
	readData= readData;
	chartview = new QChartView(parent);
	qDebug()<<"ShowChart setGeometry"<<parent->size();
	chartview->setGeometry(2,2,parent->size().width()-4,parent->size().height()-4);
	chartview->show();
	chartview->setRenderHint(QPainter::Antialiasing);
	mtype =type;
	if(type == 0)
	{
		title = "Lower Extremity Kincmatics (Sagittal Plane)";
	}
	else
	{
		title = "Lower Extremity Kincmatics (Horizontal Plane)";
	}
	ShowInitChart(type);
}

void ShowChart::resetTheChartSize(QSize m_size)
{
   chartview->resize(m_size-QSize(4,4));
   qDebug()<<"ShowChart reset setGeometry"<<m_size;
}

QStringList ShowChart::analysisTheString(QString str,int num)
{   QStringList list;
	for(int i = 0;i<=num;i++)
	{
		QString str1 = str.section(',', i, i);
		list<<str1;
	}
	return list;
}

void  ShowChart::ShowInitChart(int type)
{
	GaitAnalysis gait;
	JointAngle join;
	gait.init("./config.ini");
	gait.getReferenceParam(join, type);


	chart = new QChart();
	chart->legend()->hide();
	chart->setTitle(title);
	float min = join.kneeAngle[0];
	float max = join.kneeAngle[0];
	QSplineSeries *series = new QSplineSeries;
	seriesref = new QSplineSeries;
	for (int i = 0; i < join.kneeAngle.size(); i++){
		if(min > join.kneeAngle[i])
		   min = join.kneeAngle[i];

		if(max < join.kneeAngle[i])
		   max = join.kneeAngle[i];
		*series << QPointF(i, join.kneeAngle[i]);
	}
	chart->addSeries(series);
	// Customize series
	QPen pen(QRgb(0xFB9700));
	pen.setWidth(3);
	series->setPen(pen);

	// Customize chart title
	QFont font;
	font.setPixelSize(12);
	chart->setTitleFont(font);
	chart->setTitleBrush(QBrush(Qt::black));

	axisX = new QCategoryAxis();
	axisY = new QCategoryAxis();

//  Customize axis colors
	QPen axisPen(QRgb(0xd18952));
	axisPen.setWidth(2);
	axisX->setLinePen(axisPen);
	axisY->setLinePen(axisPen);
	axisX->setTitleText("Percent of Gait Cycle");
	axisY->setTitleText("Degrees");
	axisX->setTitleVisible(true);
	axisY->setTitleVisible(true);
	axisX->setLabelFormat("%%");
	axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
	axisY->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

	// Customize axis label colors
	QBrush axisBrush(Qt::black);
	axisX->setLabelsBrush(axisBrush);
	axisY->setLabelsBrush(axisBrush);

//  Customize grid lines and shades
	axisX->setGridLineVisible(true);
	axisY->setGridLineVisible(true);

   int minspace = (int (min/10))*10 -20;
   int maxspace = (int (max)/10)*10 + 20;
   int space = (maxspace-minspace)/5;
   //qDebug()<<"max min"<<minspace<<"   "<<maxspace;
	for(int i =0;i<=10;i++){
	   axisX->append(QString::number(10*i, 10),10*i);
	}
	for(int i =0;i<=5;i++){
	   axisY->append(QString::number(space*i-10, 10),space*i-10);
	}
	axisX->setStartValue(0);
	axisX->setRange(0,100);
	axisX->setTickCount(10);

	axisY->setRange(minspace,maxspace);
	axisY->setTickCount(5);

	chart->setAxisX(axisX, series);
	chart->setAxisY(axisY, series);
	chartview->setChart(chart);
}

void  ShowChart::showCharView(QString parastr)
{
	//qDebug()<<"showCharView "<<parastr;;
	if(parastr==NULL)
	{
	   QMessageBox::information(this,tr("提示"),tr("请先加载视频，并捕获解析"));
	   return;
	}
	QStringList paramskneeangle;
	paramskneeangle = analysisTheString(parastr,99);
	for (int i = 0; i < paramskneeangle.size(); i++){
		*seriesref << QPointF(i, paramskneeangle[i].toFloat());
	}
	QPen pen(QRgb(0x002CFB00));
	pen.setWidth(3);
	seriesref->setPen(pen);
	chart->addSeries(seriesref);
	chart->setAxisX(axisX, seriesref);
	chart->setAxisY(axisY, seriesref);
	chartview->setChart(chart);

}

void ShowChart::removeSeriesFromChart(){

	if(chart!=NULL)
	{
		chart->close();
		ShowInitChart(mtype);
	}

}
ShowChart::~ShowChart()
{
	delete ui;
}
