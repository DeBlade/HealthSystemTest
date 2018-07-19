#include "showreport.h"
#include "ui_showreport.h"
#include <QPixmap>
#include <QDebug>
#include "saggitalshowchart.h"
#include <QSplineSeries>
#include <QChartView>
#include <QChart>
ShowReport::ShowReport(QWidget *parent, QString id,HealthReadData *readData) :
	QWidget(parent),
	ui(new Ui::ShowReport)
{
	ui->setupUi(this);
    //qDebug()<<"ShowReport    "<<id;
	connect(this,&ShowReport::getUserInformation,readData,&HealthReadData::getShowReportMessage);
	connect(readData,&HealthReadData::sendQueryToShowReport, this, &ShowReport::updateReportUi);
	emit getUserInformation(id);
	sagreportview = NULL;
	fronreporttview = NULL;
	QPixmap pix("://image/report.bmp");
	ui->showpic->setPixmap(pix);




}

void ShowReport::updateReportUi(QStringList list)
{
    //qDebug()<<"updateReportUi   "<<list;
	ui->showname->setText(list[0]);
	ui->showage->setText(list[1]);

	setTextInReport(ui->footAngle,ui->footAngleresult,list[2],6.75,6.75);
	setTextInReport(ui->stepWidth,ui->stepWidthresult,list[3],5,11);
	setTextInReport(ui->cadence,ui->cadenceresult,list[4],110,120);
	setTextInReport(ui->srideLength,ui->srideLengthresult,list[5],100,160);
	setTextInReport(ui->stepLength,ui->stepLengthresult,list[6],70,75);
	setTextInReport(ui->walkingSpeed,ui->walkingSpeedresult,list[7],100,120);
	sagreportview = new QChartView(ui->sagwidget);
	fronreporttview = new QChartView(ui->fronwidget);
	reportShowChartView(sagreportview,list[9],0);
	reportShowChartView(fronreporttview, list[8], 1);
}

void ShowReport::setTextInReport(QLabel *label,QLabel *resultlabel,QString str,float min,float max)
{
	if(str!=NULL)
	{
	   label->setText(str);
	   if(str.toFloat()>max){
           resultlabel->setText("\351\253\230");
	   }else if(str.toFloat()<min)
	   {
           resultlabel->setText("\344\275\216");
	   }
	   else
	   {
           resultlabel->setText("\346\255\243\345\270\270");
	   }
	}else
	{
	   label->setText("");
       resultlabel->setText("\346\227\240\346\257\224\350\276\203\347\273\223\346\236\234");
	}
}

void ShowReport::resizeEvent(QResizeEvent *event)
{
	event = event;
	if(sagreportview!=NULL) sagreportview->resize(ui->sagwidget->size());
	if(fronreporttview!=NULL) fronreporttview->resize(ui->fronwidget->size());
}

QStringList ShowReport::analysisTheString(QString str,int num)
{   QStringList list;
	for(int i = 0;i<=num;i++)
	{
		QString str1 = str.section(',', i, i);
		list<<str1;
	}
	return list;
}

void ShowReport::reportShowChartView(QChartView *chartview,QString str,int type)
{
    if(str==NULL) return ;
    QString title;
    if(type == 0)
    {
        title = "Lower Extremity Kincmatics (Sagittal Plane)";
    }
    else
    {
        title = "Lower Extremity Kincmatics (Horizontal Plane)";
    }

    GaitAnalysis gait;
	JointAngle join;
	gait.init("./config.ini");
	gait.getReferenceParam(join, type);

	chartview->setGeometry(0,0,ui->sagwidget->size().width(),ui->sagwidget->size().height());
	chartview->show();
	chartview->setRenderHint(QPainter::Antialiasing);
	QChart *chart = new QChart();
	chart->legend()->hide();
    chart->setTitle(title);
	float min = join.kneeAngle[0];
	float max = join.kneeAngle[0];
	QSplineSeries *series = new QSplineSeries;
	for (int i = 0; i < join.kneeAngle.size(); i++){
		if(min > join.kneeAngle[i])
		   min = join.kneeAngle[i];

		if(max < join.kneeAngle[i])
		   max = join.kneeAngle[i];
		*series << QPointF(i, join.kneeAngle[i]);

	}
	chart->addSeries(series);

	QStringList paramskneeangle;
	paramskneeangle = analysisTheString(str,99);
	QSplineSeries *seriesref = new QSplineSeries;
	for (int i = 0; i < paramskneeangle.size(); i++){
		*seriesref << QPointF(i, paramskneeangle[i].toFloat());
	}
	chart->addSeries(seriesref);

    QPen pen(QRgb(0xFB9700));
    pen.setWidth(3);
    series->setPen(pen);

    QPen pen1(QRgb(0x002CFB00));
    pen1.setWidth(3);
    seriesref->setPen(pen1);

	// Customize chart title
	QFont font;
	font.setPixelSize(16);
	chart->setTitleFont(font);
	chart->setTitleBrush(QBrush(Qt::black));

	QCategoryAxis *axisX = new QCategoryAxis();
	QCategoryAxis *axisY = new QCategoryAxis();

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
	chart->setAxisX(axisX, seriesref);
	chart->setAxisY(axisY, seriesref);
	chartview->setChart(chart);
}
ShowReport::~ShowReport()
{
	delete ui;
}
