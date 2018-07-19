#ifndef FRONTALSHOWCHART_H
#define FRONTALSHOWCHART_H
#include "healthreaddata.h"
#include "showchart.h"

namespace name {
class FrontalShowChart;
}
class FrontalShowChart  : public ShowChart
{
    Q_OBJECT
public:
     explicit FrontalShowChart(QWidget *parent = 0,QString id = NULL,int type = NULL,HealthReadData *readData =NULL);
public slots:
    void showFronChat(QString );
signals:
    void getKneeAngledata(QString ,QString,int chartId);
private :
    void getdata(QString tag1,QString id);

};

#endif // FRONTALSHOWCHART_H
