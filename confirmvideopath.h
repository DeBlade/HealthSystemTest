#ifndef CONFIRMVIDEOPATH_H
#define CONFIRMVIDEOPATH_H

#include <QWidget>
#include "healthreaddata.h"

namespace Ui {
class ConfirmVideoPath;
}

class ConfirmVideoPath : public QWidget
{
    Q_OBJECT

public:
    explicit ConfirmVideoPath(QWidget *parent = 0,QString path = NULL,QString name = NULL,HealthReadData *readData = NULL);
    ~ConfirmVideoPath();
signals:
    void conSendMessageToAnalysis();
    void conSendmessagetoOpen(QString);
    void insertVideoFilePath(QString,QString,QString);
private slots:

    void on_filepushButton_clicked();

    void on_cancelpushButton_clicked();

    void on_surepushButton_clicked();

private:
    Ui::ConfirmVideoPath *ui;
    QString mid;
    QString sqltype;
};

#endif // CONFIRMVIDEOPATH_H
