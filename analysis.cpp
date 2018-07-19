#include "analysis.h"
#include "ui_analysis.h"
#include <QDebug>
#include <playvideo.h>
#include <QLabel>
#include <openfilewidget.h>
#include <QPushButton>
#include <QTreeWidgetItem>
#include <QSqlQuery>
#include <confirmvideopath.h>
#include <iostream>
#include "bydGaitAnalysis.h"
#include "healthreaddata.h"
#include <QMessageBox>
#include <QFileInfo>
Analysis::Analysis(QWidget *parent,HealthReadData *readData) :
	QWidget(parent),
	ui(new Ui::Analysis)
{
	ui->setupUi(this);

	aReadData = readData;
	sagchartview = NULL;
	fronchartview = NULL;
	initTheUi();
	setButtonDisable();
	initTheChartView();

	this->setParent(parent);
	connect(this,&Analysis::sendIdtoGetFilePath,readData,&HealthReadData::getVideoPath);

	connect(readData,&HealthReadData::sendVideoPathToanaliysis,this,&Analysis::getVideoPathNext);

	connect(sagPushButton,&QPushButton::clicked,this,&Analysis::showSaggitalAlterWindow);

	connect(fronPushButton,&QPushButton::clicked,this,&Analysis::showFrontalAlterWindow);

	connect(this,&Analysis::sendKneeAngleTodataBase,readData,&HealthReadData::insertJoinKneeAngle);

	connect(this,&Analysis::sagButtoClick, sagShowchart, &SaggitalShowChart::showSagChat);

	connect(this,&Analysis::FornButtoClick,fronShowChart,&FrontalShowChart::showFronChat);

	connect(sagVideoPushButton,&QPushButton::clicked,this,&Analysis::sagVideoButtonClick);

	connect(fronVideoPushButton,&QPushButton::clicked,this,&Analysis::fronVideoButtonClick);

	connect(this,&Analysis::sendProntalData,readData,&HealthReadData::updateProntalData);

	connect(this,&Analysis::sendSaggitalData,readData,&HealthReadData::updateSaggitalData);

	connect(this,&Analysis::sendOriginalData,readData,&HealthReadData::updateOriginalData);

	connect(readData, &HealthReadData::sendOriginalDataToAna, this, &Analysis::receiverOriginalData);

	connect(this,&Analysis::getOriginalDataFromDatabase,readData,&HealthReadData::queryOriginalData);

	connect(readData,&HealthReadData::sendVideoPathWhichInsert,this,&Analysis::resetTheUi);

	connect(this,&Analysis::getFilePathName,this,&Analysis::receiveFromopen);
}
void Analysis::initTheUi()
{
	ui->groupBox->setTitle("\346\255\245\346\200\201\345\210\206\346\236\220");
	sagShowchart=NULL;
	fronShowChart=NULL;
	sagPushButton = new QPushButton(this);
	fronPushButton = new QPushButton(this);

	sagVideoPushButton = new QPushButton(this);
	fronVideoPushButton = new QPushButton(this);
	initStackButton(sagPushButton,"://image/recoder.png");
	initStackButton(fronPushButton,"://image/recoder.png");

	initStackButton(sagVideoPushButton,"://image/play_icon.jpg");
	initStackButton(fronVideoPushButton,"://image/play_icon.jpg");
//    initStackButton(ui->sagFileButton,"://image/openfile.png");
//    initStackButton(ui->fronFileButton,"://image/openfile.png");
	ui->sagittallabel->setAlignment(Qt::AlignCenter);
	ui->horizontallabel->setAlignment(Qt::AlignCenter);

	ui->sagStackedWidget->addWidget(sagPushButton);
	ui->sagStackedWidget->addWidget(sagVideoPushButton);

	ui->fronStackedWidget->addWidget(fronPushButton);
	ui->fronStackedWidget->addWidget(fronVideoPushButton);

	ui->groupsagBoxchart->setParent(ui->groupBox);
	ui->groupsagBoxchart->sizeHint();
}

void Analysis::sagVideoButtonClick()
{
	playVideo *playvideo = new playVideo(NULL,sagfilepath);
	playvideo->show();
}

void Analysis::fronVideoButtonClick()
{
	 playVideo *playvideo = new playVideo(NULL,fronfilepath);
	 playvideo->show();
}
void Analysis::initStackButton(QPushButton *button,QString text)
{
	button->setStyleSheet("background: transparent;");
	QPixmap pixmap;
	pixmap.load(text);
	button->setGeometry(0,0,pixmap.width(),pixmap.height());
	button->setIcon(pixmap);
	button->setIconSize(QSize(pixmap.width(),pixmap.height()));
}

void Analysis::resetGroupBoxSize(QSize size)
{
	qDebug()<<"analysis resetGroupBoxSize"<<ui->groupBox->size()<<"hhh  "<<size;
	ui->groupBox->resize(size);
	ui->groupBox->update();
	ui->groupBox->updateGeometry();
	ui->groupBox->adjustSize();

}

void Analysis::resizeEvent(QResizeEvent *event){
	//event = event;
	 qDebug()<<"Analysis resizeEvent groupsagBoxchart "<<ui->groupsagBoxchart->size();
	if(sagShowchart!=NULL){
	  emit sentGroupBoxSizeToSag(ui->groupsagBoxchart->size());
	}
	if(fronShowChart!=NULL){
	  emit sentGroupBoxSizeToFron(ui->groupsagBoxchart->size());
	}

	if(sagchartview!=NULL)
	{
		sagchartview->resize(ui->saggwidget->size()-QSize(5,5));
	}
	if(fronchartview!=NULL)
	{
		fronchartview->resize(ui->fornwidget->size()-QSize(5,5));
	}
}


void Analysis::initButton(){
	 QPixmap deletepixmap,getpixmap,changepixmap,analysispixmap,getreportpixmap;
	 deletepixmap.load("://image/delete.png");
	 getpixmap.load("//image/delete.png");
	 changepixmap.load("//image/delete.png");
	 analysispixmap.load("//image/delete.png");
	 getreportpixmap.load("//image/delete.png");
	 ui->deletebutton->setGeometry(0,0,deletepixmap.width(),deletepixmap.height());
	 ui->deletebutton->setFixedSize(deletepixmap.width(),deletepixmap.height());
	 ui->deletebutton->setIcon(deletepixmap);
	 ui->deletebutton->setIconSize(QSize(deletepixmap.width(),deletepixmap.height()));
}

void Analysis::restShowChartView()
{
	emit resetChartView();
	sagchartview->close();
	fronchartview->close();
	ui->fornButton->setEnabled(true);
	ui->sagbutton->setEnabled(true);
}
void Analysis::initTheChartView()
{   sagShowchart=new SaggitalShowChart(ui->groupsagBoxchart,userId,0,aReadData);
	sagShowchart->show();

	connect(this, &Analysis::sentGroupBoxSizeToSag, sagShowchart, &SaggitalShowChart::resetTheChartSize);
	connect(this, &Analysis::resetChartView, sagShowchart, &SaggitalShowChart::removeSeriesFromChart);

	fronShowChart = new FrontalShowChart(ui->groupBoxFronChart,userId,1,aReadData);
	fronShowChart->show();

	connect(this, &Analysis::sentGroupBoxSizeToFron, fronShowChart, &FrontalShowChart::resetTheChartSize);
	connect(this, &Analysis::resetChartView, fronShowChart, &FrontalShowChart::removeSeriesFromChart);

	sagchartview = new QChartView(ui->saggwidget);
	sagchartview->setGeometry(0, 0, ui->saggwidget->width()-5,ui->saggwidget->height()-5);

	fronchartview = new QChartView(ui->fornwidget);
	fronchartview->setGeometry(0, 0, ui->fornwidget->width()-5,ui->fornwidget->height()-5);
}

bool Analysis::isItemHasParent(QTreeWidgetItem * item)
{

	QTreeWidgetItem * parentItem = item->parent();
	if(!parentItem)
	{
		 return true;
	}
	else
	{
		return false;
	}

}

void Analysis::receiveMessaeFromhealth(QTreeWidgetItem *item)
{
	qDebug()<<"receiveMessaeFromhealth "<<ui->groupBox->size()<<"  "<<ui->groupsagBoxchart->size();
	while(!isItemHasParent(item))
	{
	   item = item->parent();
	   isItemHasParent(item);
	}
	QVariant var1 = item->data(2,Qt::UserRole);
	userId = var1.toString();
	qDebug()<<"receiveMessaeFromhealth userId "<<userId;
	emit sendIdtoGetFilePath(var1.toString());
}

void Analysis::resetTheUi(QString id)
{
	 emit sendIdtoGetFilePath(id);
}

void Analysis::getVideoPathNext(QString sagpath,QString fronpath)
{
	 qDebug()<<"getVideoPathNext "<<sagpath<<" "<<fronpath;
	 sagfilepath.clear();
	 fronfilepath.clear();
	 sagfilepath = sagpath;
	 fronfilepath = fronpath;
	 if(sagpath==NULL)
	 {
		  ui->sagStackedWidget->setCurrentWidget(sagPushButton);
	 }
	 else
	 {
		  ui->sagStackedWidget->setCurrentWidget(sagVideoPushButton);
	 }

	 if(fronpath==NULL)
	 {
		  ui->fronStackedWidget->setCurrentWidget(fronPushButton);
	 }
	 else
	 {
		  ui->fronStackedWidget->setCurrentWidget(fronVideoPushButton);
	 }
	 emit getOriginalDataFromDatabase(userId,(sagfilepath==NULL)?false:true,(fronfilepath==NULL)?false:true);
}

void Analysis::receiveFromopen(QString type,QString filepath)
{
	qDebug()<<"receiveFromopen "<<type<<" "<<filepath;
   if(!filepath.contains(type))
   {
	   QMessageBox::information(this,tr("提示"),tr("请选择与解析步态类型相同的视频文件"));
	   return;
   }
   ConfirmVideoPath *videopath = new ConfirmVideoPath(NULL,filepath,userId,aReadData);

   videopath->show();
}

void Analysis::setButtonDisable()
{
	ui->getbutton->setEnabled(true);
	ui->sagbutton->setEnabled(true);
	ui->fornButton->setEnabled(true);
	ui->getreportbutton->setEnabled(true);
}

void Analysis::showSaggitalAlterWindow(){
	OpenfileWidget *openwidget= new OpenfileWidget(NULL,userId,"S",aReadData);

	connect(openwidget,&OpenfileWidget::sendMessageToana,this,&Analysis::receiveFromopen);

	openwidget->show();
}

void Analysis::showFrontalAlterWindow(){
	OpenfileWidget *openwidget= new OpenfileWidget(NULL,userId,"F",aReadData);

	connect(openwidget,&OpenfileWidget::sendMessageToana,this,&Analysis::receiveFromopen);

	openwidget->show();
}

void Analysis::getData(QString path){

	std::string cstr;
	cstr = string((const char *)path.toLocal8Bit());
	GaitAnalysis gait;
	JointAngle join;
	gait.init("./config.ini");
	//标准曲线
	//PLANE_SAGGITAL,		//失状面   侧面
	//PLANE_FRONTAL,		//额状面   正面
	int plane_type = PLANE_SAGGITAL;
	int num;
	QString sqltype = "paramskneeangle";
    QFileInfo fileinfo;
    QString file_name;
    fileinfo = QFileInfo(path);
    file_name = fileinfo.fileName();
    if (file_name.contains("S"))
	{
		plane_type = PLANE_SAGGITAL;
		sqltype = "paramskneeangle";
		num = 1;
	}
    else if(file_name.contains("F"))
	{
		plane_type = PLANE_FRONTAL;
		sqltype = "frontalkneeangle";
		num = 3;
	}else
	{
		qDebug()<<"the audio name is  nonstandard";
		return;
	}
	gait.getReferenceParam(join, plane_type);
	QString kneestr;
	QString kennstrstandx;
	QString kennstrstandy;
	gait.analysis(path.toStdString().c_str(), params, plane_type, true);

	int oragialsize = params.markPoints[num].size();
	for (int k = 0;k<oragialsize;k++)
	{
		kennstrstandx.append(QString("%1").arg(params.markPoints[num][k].x));
		kennstrstandy.append(QString("%1").arg(params.markPoints[num][k].y));
		if (k<params.markPoints[num].size() - 1)
		{
			kennstrstandx.append(",");
			kennstrstandy.append(",");
		}
	}

	if (plane_type == PLANE_FRONTAL)
	{
		emit sendProntalData(QString("%1").arg(params.timeSpaceDsp.footAngle),
							 QString("%1").arg(params.timeSpaceDsp.stepWidth*100),
							 userId);
		emit sendOriginalData(kennstrstandx,kennstrstandy,1,userId,QString::number(oragialsize));
	}
	else if (plane_type == PLANE_SAGGITAL)
	{

		emit sendSaggitalData(QString("%1").arg(params.timeSpaceDsp.cadence),
							  QString("%1").arg(params.timeSpaceDsp.srideLength*100),
							  QString("%1").arg(params.timeSpaceDsp.stepLength*100),
							  QString("%1").arg(params.timeSpaceDsp.walkingSpeed*100),
							  userId);
		emit sendOriginalData(kennstrstandx,kennstrstandy,0,userId,QString::number(oragialsize));
	}


	for (int i = 0; i < params.jointAngle.kneeAngle.size(); i++)
	{
		kneestr.append(QString("%1").arg(params.jointAngle.kneeAngle[i]));
		if(i<params.jointAngle.kneeAngle.size()-1)
		{
			kneestr.append(",");
		}
	}
	emit sendKneeAngleTodataBase(sqltype,kneestr,userId);
}

void Analysis::receiverOriginalData(QStringList saglist,QStringList fronlist)
{

	if (!saglist.isEmpty())
	{
		qDebug()<<"saglist is not NULL";
		analysisShowChartView(sagchartview,saglist[0],saglist[1],saglist[2].toInt());
	}

	if (!fronlist.isEmpty())
	{
		qDebug()<<"fronlist is not NULL";

		analysisShowChartView(fronchartview,fronlist[0],fronlist[1],fronlist[2].toInt());
	}
}

void Analysis::analysisShowChartView(QChartView *chartview,QString strx,QString stry,int num)
{
	chartview->show();
	chartview->setRenderHint(QPainter::Antialiasing);
	QChart *chart = new QChart();
	chart->legend()->hide();
	QStringList listx;
	QStringList listy;
	listx = analysisTheString(strx,num);
	listy = analysisTheString(stry,num);
	QSplineSeries *series = new QSplineSeries;
	float minX = listx[0].toFloat();
	float maxX = listx[0].toFloat();

	float minY = listy[0].toFloat();
	float maxY = listy[0].toFloat();
	for (int i = 0; i < num; i++){
		*series << QPointF(listx[i].toFloat(), listy[i].toFloat());
		if(minX > listx[i].toFloat())
		{
		   minX = listx[i].toFloat();
		}
		if(maxX < listx[i].toFloat())
		{
		   maxX = listx[i].toFloat();
		}

		if(minY > listy[i].toFloat())
		{
		   minY = listy[i].toFloat();
		}
		if(maxY < listy[i].toFloat())
		{
		   maxY = listy[i].toFloat();
		}
	}

	chart->addSeries(series);
	QCategoryAxis *axisX = new QCategoryAxis();
	QCategoryAxis *axisY = new QCategoryAxis();

	axisX->setStartValue(minX);
	axisX->setRange(minX,maxX);

	axisY->setStartValue(minY);
	axisY->setRange(minY,maxY);

	chart->setAxisX(axisX, series);
	chart->setAxisY(axisY, series);

	chartview->setChart(chart);
}

QStringList Analysis::analysisTheString(QString str,int num)
{   QStringList list;
	for(int i = 0;i<=num;i++)
	{
		QString str1 = str.section(',', i, i);
		list<<str1;
	}
	return list;
}

Analysis::~Analysis()
{
	delete ui;
}

void Analysis::on_getbutton_clicked()
{
	restShowChartView();
	qDebug()<<"on_getbutton_clicked"<<sagfilepath<<" "<<fronfilepath;
	bool sagdata = false,frondata = false;
	if(sagfilepath!=NULL)
	{
		 getData(sagfilepath);
		 sagdata = true;
	}

	if(fronfilepath!=NULL)
	{
		 getData(fronfilepath);
		 frondata = true;
	}
	emit getOriginalDataFromDatabase(userId,sagdata,frondata);
}

void Analysis::on_getreportbutton_clicked()
{
	emit sendMessageToShowReport(userId,true);
}


void Analysis::on_sagbutton_clicked()
{
	qDebug()<<"on_sagbutton_clicked "<<userId;
	ui->sagbutton->setEnabled(false);
	emit sagButtoClick(userId);
}

void Analysis::on_fornButton_clicked()
{
	qDebug()<<"on_fornButton_clicked "<<userId;
	ui->fornButton->setEnabled(false);
	emit FornButtoClick(userId);
}

void Analysis::on_sagFileButton_clicked()
{
	getfilePath("S");
}


void Analysis::on_fronFileButton_clicked()
{
	getfilePath("F");
}

void Analysis::getfilePath(QString type){
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"./video", tr("video(*.avi *.mp4 *.mov *.wmv *.rmvb *.wkv)"));
	if(fileName==NULL) return;
	emit getFilePathName(type,fileName);
}
