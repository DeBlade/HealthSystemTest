#ifndef PLAYVIDEO_H
#define PLAYVIDEO_H

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QResizeEvent>

namespace Ui {
class playVideo;
}

class playVideo : public QWidget
{
    Q_OBJECT

public:
    explicit playVideo(QWidget *parent = 0,QString path =NULL);
    ~playVideo();
private :
    void ifFileExists(QString);

public slots:

    void resizeTheVideoSize(QSize);
    void onWindowDestory();
private:
    Ui::playVideo *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QVideoWidget *videoWidget;
    QString filepaths;
};

#endif // PLAYVIDEO_H
