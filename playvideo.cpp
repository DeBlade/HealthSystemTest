#include "playvideo.h"
#include "ui_playvideo.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>

playVideo::playVideo(QWidget *parent,QString path) :
	QWidget(parent),
	ui(new Ui::playVideo)
{
	ui->setupUi(this);
	player=NULL;
	videoWidget=NULL;
	setWindowTitle("视频播放");
	connect(this,&playVideo::destroyed,this,&playVideo::onWindowDestory);
	player = new QMediaPlayer;
	playlist = new QMediaPlaylist(player);
	videoWidget = new QVideoWidget(ui->widget);
	player->setVideoOutput(videoWidget);
	videoWidget->show();
	videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
    videoWidget->setGeometry(0, 0,640,480);
	playlist->setCurrentIndex(0);

	ifFileExists(path);
	playlist->clear();
	playlist->addMedia(QUrl::fromLocalFile(path));
	player->setPlaylist(playlist);
	player->play();

}
void playVideo::ifFileExists(QString filepath){
	if(filepath==NULL){
		return;
		qWarning()<<"video file path is " <<filepath;
	}
	QFile file(filepath);
	qDebug()<<"initVideo file.exists()" <<file.exists();
	if (!file.exists()){
		//QMessageBox::information(this, "提示", "视频不存在", "确认");
		return;
	}
}
void playVideo::onWindowDestory(){
	player->stop();
	playlist->clear();
}
playVideo::~playVideo()
{
	delete ui;
}
void playVideo::resizeTheVideoSize(QSize v_size){
   qDebug()<<"resizeTheVideoSize" <<v_size;
//   if(player!=NULL&&videoWidget!=NULL){
//	   videoWidget->resize(v_size-QSize(20,20));
//   }
}
