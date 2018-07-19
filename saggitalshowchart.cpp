#include "saggitalshowchart.h"

SaggitalShowChart::SaggitalShowChart(QWidget *parent,QString id,int type,HealthReadData *readData) :
    ShowChart(parent,id,type,readData)
{
    connect(readData,&HealthReadData::sendJoinParamKneeAngleToSag,this,&SaggitalShowChart::showCharView);
    connect(this,&SaggitalShowChart::getKneeAngledata,readData,&HealthReadData::getDataToChart);
}

void SaggitalShowChart::showSagChat(QString mId)
{
     getdata("paramskneeangle",mId);
}

void SaggitalShowChart::getdata(QString tag1,QString id)
{   qDebug()<<"ShowChart getdata"<<tag1<<id;
    emit getKneeAngledata(tag1,id,1);
}
