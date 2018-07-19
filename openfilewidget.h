#ifndef OPENFILEWIDGET_H
#define OPENFILEWIDGET_H

#include <QWidget>
#include "healthreaddata.h"
namespace Ui {
class OpenfileWidget;
}

class OpenfileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OpenfileWidget(QWidget *parent = 0,QString id = NULL,QString type = NULL,HealthReadData *readData = NULL);
    ~OpenfileWidget();

signals :
    void sendMessageToana(QString type,QString name);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
private:
    Ui::OpenfileWidget *ui;
    QString userId;
    HealthReadData *mReadData;
    QString mType;
};

#endif // OPENFILEWIDGET_H
