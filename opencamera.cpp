#include "opencamera.h"
#include "ui_opencamera.h"
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QDir>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv\cv.h>
#include <QDateTime>
#include <QFileDialog>
OpenCamera::OpenCamera(QWidget *parent,QString id,QString type,HealthReadData *readData) :
    QWidget(parent),
    ui(new Ui::OpenCamera)
{
    ui->setupUi(this);
    setWindowTitle("视频录制");
    ui->label->setHidden(true);
    isRecoder = false;
    tag = true;
    userId = id;
    mType = type;

    playIcon = "://image/recoderbegin.jpg";
    stopIcon = "://image/recoderstop.jpg";

    QPixmap pixmap;
    pixmap.load(playIcon);
    ui->pushButton->setIcon(pixmap);
    ui->pushButton->setIconSize(QSize(pixmap.width(),pixmap.height()));
    pixmap.load(stopIcon);
    ui->pushButton_3->setIcon(pixmap);
    ui->pushButton_3->setIconSize(QSize(pixmap.width(),pixmap.height()));
    ui->pushButton_3->hide();

    videoCap = VideoCapture(0);
    theTimer = new QTimer;
    connect(theTimer,&QTimer::timeout,this,&OpenCamera::updateImage);
    if (!videoCap.isOpened())
    {
        qDebug() << "can not open camera";
    }
    else
    {
        qDebug() << "open camera";
        theTimer->start(900/FPS);
    }
    connect(this,&OpenCamera::destroyed,this,&OpenCamera::onWindowDestory);
    connect(this,&OpenCamera::sendCameraSaveVideoPath,readData,&HealthReadData::insertVideoPathToDatabase);
}

OpenCamera::~OpenCamera()
{
    delete ui;
}

void OpenCamera::onWindowDestory()
{

}
QImage OpenCamera::CV2QtType(Mat pSrc)
{
    IplImage IplImg = pSrc;
    QImage qi = QImage(IplImg.width, IplImg.height, QImage::Format_RGB32);
    for (int i = 0; i < IplImg.height; i++)
    {
        for (int j = 0; j < IplImg.width; j++)
        {
            int r, g, b;
            if (RGB_TYPE == IplImg.nChannels)
            {
                b = (int)CV_IMAGE_ELEM(&IplImg, uchar, i, j * 3 + 0);
                g = (int)CV_IMAGE_ELEM(&IplImg, uchar, i, j * 3 + 1);
                r = (int)CV_IMAGE_ELEM(&IplImg, uchar, i, j * 3 + 2);
            }
            else if (GRAY_TYPE == IplImg.nChannels)
            {
                b = (int)CV_IMAGE_ELEM(&IplImg, uchar, i, j);
                g = b;
                r = b;
            }
            qi.setPixel(j, i, qRgb(r, g, b));
        }
    }
    return qi;
}
void OpenCamera::paintEvent(QPaintEvent *e)
{
    videoCap >> labelFrame;
    if (isRecoder)
    {
        recVid << labelFrame;
        qDebug()<<"recVid";
    }
    qDebug()<<"updateLabel";
    QImage image = CV2QtType(labelFrame);
    ui->label->resize(image.size());
    QPixmap pixTemp = QPixmap::fromImage(image);
    ui->label->setPixmap(pixTemp);
    ui->label->show();
}

void OpenCamera::updateImage()
{
    this->update();
}

QString OpenCamera::getNowTime(){
    QDateTime time = QDateTime::currentDateTime();
    QString path;
    QDir dir;
    path=dir.currentPath();
    QString str = "/video/"+mType;
    str= path+str+ time.toString("yyyyMMddhhmmss")+".avi";
    qDebug()<<"OpenCamera video path"<<str;
    return str;
}

void OpenCamera::on_pushButton_clicked()
{
   // const char win1[] = "win1";
    ui->pushButton->hide();
    ui->pushButton_3->show();
    QByteArray ba = getNowTime().toLatin1();
    char *file_out = ba.data();
    //PLANE_SAGGITAL,		//失状面
    //PLANE_FRONTAL,		//额状面
    if(mType=="S")
    {
        emit sendCameraSaveVideoPath("sagfilepath",getNowTime(), userId);
    }
    else if(mType=="F")
    {
        emit sendCameraSaveVideoPath("fronfilepath",getNowTime(), userId);
    }
    else
    {
        qDebug()<<"opencamera the path is error";
    }

    int width = (int) videoCap.get(CAP_PROP_FRAME_WIDTH);
    int height = (int)videoCap.get(CAP_PROP_FRAME_HEIGHT);
    recVid = VideoWriter(file_out, VideoWriter::fourcc('I','4','2','0'), FPS, Size(width, height));
    if (!recVid.isOpened())
    {
        qDebug()<<"ERROR VIDEO IS NOT OPEN ";
        return;
    }
    isRecoder = true;
   // namedWindow(win1);
//    while (!isRecoder)
//    {
//        videoCap >> inFrame;
//        recVid << labelFrame;
//        imshow(win1, inFrame);
//        waitKey(900/FPS);
//    }
    //destroyWindow(win1);
}


void OpenCamera::on_pushButton_3_clicked()
{
    isRecoder = false;
    recVid.release();
    videoCap.release();
    ui->pushButton->show();
    ui->pushButton_3->hide();
    this->close();
}
