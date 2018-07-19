#include "openfilewidget.h"
#include "ui_openfilewidget.h"
#include <QPixmap>
#include <QFileDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QCamera>
#include "opencamera.h"

OpenfileWidget::OpenfileWidget(QWidget *parent,QString id,QString type,HealthReadData *readData) :
	QWidget(parent),
	ui(new Ui::OpenfileWidget)
{
	ui->setupUi(this);
	mReadData = readData;
	qDebug()<<"OpenfileWidget name"<<id<<type;
	userId = id;
	mType = type;
	setWindowTitle(tr("打开文件"));
	QPixmap pixmap,pixmap1;
	pixmap.load("://image/recodernow.png");
	pixmap1.load("://image/nativeselectfile.png");
	ui->pushButton->setGeometry(0,0,pixmap.width(),pixmap.height());
	ui->pushButton->setFixedSize(pixmap.width(),pixmap.height());
	ui->pushButton->setIcon(pixmap);
	ui->pushButton->setIconSize(QSize(pixmap.width(),pixmap.height()));
	ui->pushButton->show();
	ui->pushButton_2->setFixedSize(pixmap1.width(),pixmap1.height());
	ui->pushButton_2->setGeometry(0,0,pixmap1.width(),pixmap1.height());
	ui->pushButton_2->setIcon(pixmap1);
	ui->pushButton_2->setIconSize(QSize(pixmap1.width(),pixmap1.height()));
	ui->pushButton_2->show();
}

OpenfileWidget::~OpenfileWidget()
{
	delete ui;
}

void OpenfileWidget::on_pushButton_clicked()
{
	//QMessageBox::information(this, "提示", "摄像头不存在", "确认");
	OpenCamera *opencamera = new OpenCamera(NULL,userId,mType,mReadData);
	opencamera->show();
	close();
}

void OpenfileWidget::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"../", tr("video(*.avi *.mp4 *.mov *.wmv *.rmvb *.wkv)"));
	if(fileName==NULL) return;
    emit sendMessageToana(mType,fileName);
	close();
}
