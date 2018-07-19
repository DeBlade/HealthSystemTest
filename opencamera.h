#ifndef OPENCAMERA_H
#define OPENCAMERA_H

#include <QWidget>
#include <QImage>
#include <QThread>
#include <healthreaddata.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv\cv.h>
#include <QLabel>
#include <QTimer>

#define  FPS 30
#define RGB_TYPE 3
#define GRAY_TYPE 1
using namespace cv;
namespace Ui {
class OpenCamera;
}

class OpenCamera : public QWidget
{
	Q_OBJECT

public:
	explicit OpenCamera(QWidget *parent = 0,QString id = NULL,QString type = NULL,HealthReadData *readData =NULL);
	~OpenCamera();

signals:
	void sendCameraSaveVideoPath(QString type,QString path,QString id);
private slots:

	void on_pushButton_clicked();
	void on_pushButton_3_clicked();
	void updateImage();
	void onWindowDestory();

private:
	QImage CV2QtType(Mat p);
	QString getNowTime();
	void paintEvent(QPaintEvent *e);
private:
	Ui::OpenCamera *ui;
	VideoCapture videoCap;
	VideoWriter recVid;
	Mat inFrame;
	Mat outFrame;
	Mat labelFrame;
	bool isRecoder;
	bool tag;
	QString userId;
	QString mType;
    QString playIcon;
    QString stopIcon;

	QTimer *theTimer;
};

#endif // OPENCAMERA_H
