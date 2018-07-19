#include "confirmvideopath.h"
#include "ui_confirmvideopath.h"
#include <QDebug>
#include <QSqlQuery>
#include <QFileInfo>
#include <openfilewidget.h>
ConfirmVideoPath::ConfirmVideoPath(QWidget *parent,QString path,QString id,HealthReadData *readData) :
	QWidget(parent),
	ui(new Ui::ConfirmVideoPath)
{
	ui->setupUi(this);
	if(path!=NULL){
	ui->lineEdit->setText(path);
	}
	mid= id;
	QPixmap pixmap;
	pixmap.load("://image/nativeselectfile.png");
	ui->filepushButton->setGeometry(0,0,45,30);
	ui->filepushButton->setFixedSize(45,30);
	ui->filepushButton->setIcon(pixmap);
	ui->filepushButton->setIconSize(QSize(45,30));
	ui->filepushButton->show();
	QFileInfo fileinfo;
	QString file_name;
	fileinfo = QFileInfo(path);
	file_name = fileinfo.fileName();
	qDebug()<<"ConfirmVideoPath "<<file_name;
	sqltype = "sagfilepath";
	if (file_name.contains("S"))
	{
		sqltype = "sagfilepath";
	}
	else if(file_name.contains("F"))
	{
		sqltype = "fronfilepath";
	}else
	{
		qDebug()<<"the confirm audio name is  nonstandard";
		return;
	}
	connect(this,&ConfirmVideoPath::insertVideoFilePath,readData,&HealthReadData::insertVideoPathToDatabase);
}

ConfirmVideoPath::~ConfirmVideoPath()
{
	delete ui;
}


void ConfirmVideoPath::on_filepushButton_clicked()
{
//     OpenfileWidget *openwidget= new OpenfileWidget(NULL,sname);
//     openwidget->show();
//     close();
}

void ConfirmVideoPath::on_cancelpushButton_clicked()
{
   close();
}

void ConfirmVideoPath::on_surepushButton_clicked()
{
	emit insertVideoFilePath(sqltype,ui->lineEdit->text(),mid);
	close();
}
