#ifndef SHOWCHART_H
#define SHOWCHART_H

#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QString>
#include "bydGaitAnalysis.h"
#include <healthreaddata.h>
QT_CHARTS_USE_NAMESPACE

namespace Ui {
class ShowChart;
}

class ShowChart : public QWidget
{
    Q_OBJECT

public:
    explicit ShowChart(QWidget *parent = 0,QString id = NULL,int type = NULL,HealthReadData *readData =NULL);
    ~ShowChart();
public slots:
    void showCharView(QString params);
    void resetTheChartSize(QSize );
    void removeSeriesFromChart();
private :
    QStringList analysisTheString(QString , int);
    void ShowInitChart(int );

private:
    Ui::ShowChart *ui;
    QChartView *chartview;
    int mtype;
    QChart *chart;
    QSplineSeries *seriesref;
    QCategoryAxis *axisX;
    QCategoryAxis *axisY;
    QString title;
};

#endif // SHOWCHART_H
