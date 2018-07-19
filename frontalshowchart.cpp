#include "frontalshowchart.h"
#include <QDebug>
FrontalShowChart::FrontalShowChart(QWidget *parent,QString id,int type,HealthReadData *readData) :
    ShowChart(parent,id,type,readData)
{
    connect(readData,&HealthReadData::sendJoinParamKneeAngleToFron,this,&FrontalShowChart::showCharView);
    connect(this,&FrontalShowChart::getKneeAngledata,readData,&HealthReadData::getDataToChart);
}
void FrontalShowChart::showFronChat(QString mId)
{
     getdata("frontalkneeangle",mId);
}

void FrontalShowChart::getdata(QString tag1,QString id)
{   qDebug()<<"ShowChart getdata"<<tag1<<id;
    emit getKneeAngledata(tag1,id,2);
}
