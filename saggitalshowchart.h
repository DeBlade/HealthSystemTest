#ifndef SAGGITALSHOWCHART_H
#define SAGGITALSHOWCHART_H

#include "showchart.h"
#include "healthreaddata.h"

namespace name {

class SaggitalShowChart;

}
class SaggitalShowChart : public ShowChart
{
	Q_OBJECT
public:
    explicit SaggitalShowChart(QWidget *parent = 0,QString id = NULL,int type=NULL,HealthReadData *readData =NULL);


public slots:
    void showSagChat(QString );
signals:
    void getKneeAngledata(QString ,QString,int );
private :
    void getdata(QString tag1,QString id);
};

#endif // SAGGITALSHOWCHART_H
